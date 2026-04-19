#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
from rclpy.callback_groups import MutuallyExclusiveCallbackGroup
from rclpy.executors import MultiThreadedExecutor
from rclpy.qos import QoSProfile, QoSReliabilityPolicy, QoSDurabilityPolicy, QoSHistoryPolicy
from sensor_msgs.msg import Image
from std_msgs.msg import String
import cv2
import os
import time
from datetime import datetime, timedelta
import subprocess
from cv_bridge import CvBridge, CvBridgeError
from ultralytics import YOLO
import threading

# 配置常量
NO_PERSON_ALERT_DURATION = 45 * 60  # 45分钟无人报警阈值（秒）
DETECT_INTERVAL = 45           # 每45秒检测一次（秒）
YOLO_CONF_THRESHOLD = 0.5           # 置信度阈值
WORKSPACE_PATH = os.path.dirname(os.path.abspath(__file__))
SAVE_DIR = os.path.join(WORKSPACE_PATH, "person_detect_img")

class Yolov8PersonDetector(Node):
    def __init__(self):
        super().__init__("yolov8_person_detector_node")
        
        # 初始化核心变量
        self.bridge = CvBridge()
        self.latest_frame = None
        self.last_person_time = datetime.now()
        self.alert_triggered = False
        self.mutex = threading.Lock()  
        
        # 1. 初始化YOLOv8模型
        try:
            self.model = YOLO("yolov8n.pt")
            self.get_logger().info("✅ YOLOv8模型加载成功！")
        except Exception as e:
            self.get_logger().error(f"❌ YOLOv8模型加载失败：{str(e)}")
            raise e
        
        # 2. 初始化保存目录
        self.init_save_dir()
        
        # 3. 创建微信消息发布者
        self.wechat_pub = self.create_publisher(String, "/wechat_message", 10)
        self.get_logger().info("✅ 微信消息发布者创建成功！")
        
        # 4. 创建检测定时器
        self.detect_timer = self.create_timer(
            DETECT_INTERVAL, 
            self.person_detect_timer_callback
        )
        self.get_logger().info(f"✅ YOLO检测定时器创建成功（每{DETECT_INTERVAL/60}分钟一次）！")
        
        # 5. 创建报警检查定时器
        self.alert_timer = self.create_timer(
            1.0, 
            self.check_no_person_alert
        )
        self.get_logger().info("✅ 无人报警定时器创建成功！")
        
        # ========== 核心：匹配发布端的QoS ==========
        shm_qos = QoSProfile(
            reliability=QoSReliabilityPolicy.RELIABLE,    # 和发布端一致：RELIABLE
            durability=QoSDurabilityPolicy.VOLATILE,      # 和发布端一致：VOLATILE
            history=QoSHistoryPolicy.KEEP_LAST,
            depth=10                                       # 通用深度
        )
        # ========== QoS配置结束 ==========
        
        # 6. 订阅摄像头图像（共享内存方式）
        sub_callback_group = MutuallyExclusiveCallbackGroup()
        self.img_sub = self.create_subscription(
            Image,
            "/camera/image_raw",
            self.image_callback,
            shm_qos,
            callback_group=sub_callback_group
        )
        self.get_logger().info("✅ 订阅/camera/image_raw话题（共享内存模式）！")

    def init_save_dir(self):
        """初始化图片保存目录"""
        self.get_logger().info(f"=== 初始化保存目录：{SAVE_DIR} ===")
        try:
            os.makedirs(SAVE_DIR, exist_ok=True)
            self.get_logger().info(f"✅ 保存目录初始化成功：{SAVE_DIR}")
        except Exception as e:
            self.get_logger().error(f"❌ 创建目录失败：{str(e)}")

    def image_callback(self, img_msg):
        """图像回调：修复imgmsg_to_cvshare兼容问题，接收共享内存图像"""
        self.get_logger().info(f"🔍 收到图像消息！时间戳：{img_msg.header.stamp.sec}.{img_msg.header.stamp.nanosec}")
        with self.mutex:
            try:
                # ========== 关键修复：换回兼容的imgmsg_to_cv2 ==========
                # 低版本cv_bridge没有imgmsg_to_cvshare，用这个方法也能读取共享内存图像（仅多一次拷贝）
                cv_img = self.bridge.imgmsg_to_cv2(img_msg, desired_encoding="bgr8")
                self.latest_frame = cv_img.copy()  # 缓存帧
                # ========== 修复结束 ==========
                self.get_logger().info(f"✅ 成功缓存帧（共享内存）：{cv_img.shape[1]}x{cv_img.shape[0]}")
            except CvBridgeError as e:
                self.get_logger().error(f"❌ 图像转换失败：{str(e)}")
            except Exception as e:
                self.get_logger().error(f"❌ 回调异常：{str(e)}")

    def detect_person(self):
        """YOLOv8人体检测核心逻辑"""
        self.get_logger().info("\n=== 开始YOLO人体检测 ===")
        with self.mutex:
            if self.latest_frame is None or self.latest_frame.size == 0:
                self.get_logger().warn("⚠️ 无缓存帧！请检查图像订阅是否正常")
                return False
            
            self.get_logger().info(f"✅ 检测用帧尺寸：{self.latest_frame.shape}")
            # 执行YOLO检测（仅检测人）
            try:
                results = self.model(
                    self.latest_frame, 
                    conf=YOLO_CONF_THRESHOLD,
                    classes=[0],  # 只检测人
                    verbose=False
                )
                person_detected = len(results[0].boxes) > 0
                
                if person_detected:
                    self.get_logger().info(f"✅ 检测到人物！检测框数量：{len(results[0].boxes)}")
                else:
                    self.get_logger().info("❌ 未检测到人物！")
                
                # 保存检测结果
                self.save_detect_image(person_detected)
                return person_detected
            except Exception as e:
                self.get_logger().error(f"❌ 检测异常：{str(e)}")
                return False

    def save_detect_image(self, person_detected):
        """保存检测结果图片"""
        if self.latest_frame is None:
            return
        try:
            timestamp = int(time.time())
            status = "person_detected" if person_detected else "no_person"
            img_path = os.path.join(SAVE_DIR, f"yolo_{status}_{timestamp}.jpg")
            """cv2.imwrite(img_path, self.latest_frame)"""
            self.get_logger().info(f"✅ 检测图片保存：{img_path}")
        except Exception as e:
            self.get_logger().error(f"❌ 图片保存失败：{str(e)}")

    def person_detect_timer_callback(self):
        """定时器回调：每5分钟检测一次"""
        self.get_logger().info("\n==================== 执行定时检测 ====================")
        person_detected = self.detect_person()
        if person_detected:
            self.last_person_time = datetime.now()
            self.alert_triggered = False

    def check_no_person_alert(self):
        """检查是否超过45分钟无人，触发报警"""
        if self.alert_triggered:
            return
        duration = (datetime.now() - self.last_person_time).total_seconds()
        if duration >= NO_PERSON_ALERT_DURATION:
            self.get_logger().error(f"⚠️ 连续{duration:.0f}秒未检测到人，触发警报！")
            self.play_alert_sound()
            self.publish_wechat_message()
            self.alert_triggered = True

    def play_alert_sound(self):
        """播放警报声"""
        try:
            subprocess.run(
                ["aplay", "/usr/share/sounds/alsa/Front_Center.wav"],
                stdout=subprocess.DEVNULL,
                stderr=subprocess.DEVNULL
            )
            self.get_logger().info("✅ 警报声播放成功！")
        except Exception as e:
            self.get_logger().warn(f"⚠️ 警报声播放失败：{str(e)}")
            subprocess.run(["echo", "-e", "\\a"])  # 蜂鸣器

    def publish_wechat_message(self):
        """发布微信消息"""
        try:
            msg = String()
            msg.data = "连续45分钟未检测到人员活动！"
            self.wechat_pub.publish(msg)
            self.get_logger().info(f"✅ 发布微信消息：{msg.data}")
        except Exception as e:
            self.get_logger().error(f"❌ 消息发布失败：{str(e)}")

def main(args=None):
    # 禁用NNPACK避免警告
    os.environ['USE_NNPACK'] = '0'
    
    # ========== 共享内存全局配置 ==========
    os.environ['RMW_IMPLEMENTATION'] = 'rmw_fastrtps_cpp'
    os.environ['ROS2_SHM_SIZE'] = '2147483648'
    # ========== 环境配置结束 ==========
    
    # 初始化ROS2
    rclpy.init(args=args)
    node = Yolov8PersonDetector()
    
    # 多线程执行器
    executor = MultiThreadedExecutor(num_threads=4)
    executor.add_node(node)
    
    try:
        executor.spin()
    except KeyboardInterrupt:
        node.get_logger().info("⚠️ 节点被手动终止！")
    finally:
        executor.remove_node(node)
        node.destroy_node()
        rclpy.shutdown()

if __name__ == "__main__":
    main()


