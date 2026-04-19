#!#!/usr/bin/env python3
import rclpy
from rclpy.node import Node
import requests
import urllib3
from std_msgs.msg import String
from datetime import datetime, timedelta  # 新增：用于时间判断
# 导入自定义服务类型（根据实际功能包名调整）
from wechat_interfaces.srv import WeChatMessage  

# 消除SSL警告
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

# 配置项（新增10分钟发送间隔配置）
CONFIG = {
    "login_url": "https://www.laofu001.cn/emos-wx-two-api/user/login",
    "sent_wenxun_url": "https://www.laofu001.cn/emos-wx-two-api/meeting/sentWenxun",
    "username": "12345678909000",
    "timeout": 10,
    "max_content_length": 200,  # 内容最大长度配置化
    "min_send_interval": 10 * 60.0  # 核心：10分钟最小发送间隔（秒）
}

class WebWenxunSenderNode(Node):
    """
    ROS2节点：WEB问讯发送节点（适配10分钟发送一次无运动消息场景）
    功能：订阅话题/提供服务接收消息 → 频率控制 → 调用WEB API发送问讯
    """
    
    def __init__(self):
        super().__init__('web_wenxun_sender_node')
        
        # 核心属性
        self.token = None
        self.target_id = 544  # 目标ID（根据实际业务调整）
        self.token_expire_time = None  # 可扩展：记录token过期时间
        self.last_send_time = None  # 新增：记录上次成功发送的时间
        
        # ROS2通信架构
        # 1. 订阅话题：接收字符串消息
        self.subscription = self.create_subscription(
            String,
            'wechat_message',
            self.message_callback,
            10)
        
        # 2. 提供服务：接收自定义格式的消息请求
        self.srv = self.create_service(
            WeChatMessage,
            'send_wechat_message',
            self.send_message_callback)
        
        # 日志初始化信息
        self.get_logger().info('✅ WEB问讯发送节点已启动（45分钟间隔限制）')
        self.get_logger().info(f'📌 订阅话题: /wechat_message')
        self.get_logger().info(f'📌 提供服务: /send_wechat_message')
        self.get_logger().info(f'📌 目标ID: {self.target_id}')
        self.get_logger().info(f'📌 最小发送间隔: {CONFIG["min_send_interval"]/60:.0f}分钟')

    def get_token(self):
        """
        获取登录Token
        返回：bool - 获取是否成功
        """
        self.get_logger().debug(f'🔑 尝试从 {CONFIG["login_url"]} 获取Token')
        
        try:
            # 构造登录请求
            login_data = {"code": CONFIG["username"]}
            headers = {"Content-Type": "application/json"}
            
            resp = requests.post(
                CONFIG["login_url"],
                json=login_data,
                headers=headers,
                timeout=CONFIG["timeout"],
                verify=False  # 跳过SSL证书验证（生产环境建议启用）
            )
            
            # 响应校验
            if resp.status_code == 200:
                resp_json = resp.json()
                self.token = resp_json.get("token")
                
                if self.token:
                    self.get_logger().info('🔑 Token获取成功')
                    return True
                else:
                    self.get_logger().error('❌ Token获取失败：响应中无token字段')
                    return False
            else:
                self.get_logger().error(f'❌ Token请求失败，响应码: {resp.status_code}, 响应内容: {resp.text}')
                return False
                
        except requests.exceptions.Timeout:
            self.get_logger().error('❌ Token请求超时')
            return False
        except requests.exceptions.ConnectionError:
            self.get_logger().error('❌ Token请求连接失败：请检查网络或API地址')
            return False
        except Exception as e:
            self.get_logger().error(f'❌ Token获取异常：{str(e)}')
            return False

    def send_wenxun(self, content):
        """
        核心功能：发送问讯到WEB接口
        参数：content - 要发送的问讯内容
        返回：bool - 发送是否成功
        """
        # 1. 前置校验：Token有效性
        if not self.token:
            self.get_logger().info('🔄 Token不存在，尝试重新登录...')
            if not self.get_token():
                self.get_logger().error('❌ 登录失败，无法发送问讯')
                return False
        
        # 2. 内容预处理
        if not content or content.strip() == "":
            self.get_logger().warn('⚠️ 问讯内容为空，跳过发送')
            return False
        
        # 截断超长内容
        content = content.strip()[:CONFIG["max_content_length"]]
        if len(content) >= CONFIG["max_content_length"]:
            self.get_logger().warn(f'⚠️ 内容超长，已截断至 {CONFIG["max_content_length"]} 字符')
        
        # 3. 构造发送请求
        send_data = {
            "id": self.target_id,
            "doingwenxuntext": content
        }
        headers = {
            "token": self.token,
            "Content-Type": "application/json"
        }
        
        try:
            self.get_logger().debug(f'📤 发送问讯到 {CONFIG["sent_wenxun_url"]}, 数据: {send_data}')
            
            resp = requests.post(
                CONFIG["sent_wenxun_url"],
                json=send_data,
                headers=headers,
                timeout=CONFIG["timeout"],
                verify=False
            )
            
            # 响应解析与校验
            if resp.status_code == 200:
                resp_json = resp.json()
                if resp_json.get("code") == 200:
                    self.get_logger().info(f'✅ 问讯发送成功：{content}')
                    self.last_send_time = datetime.now()  # 新增：记录成功发送时间
                    return True
                else:
                    self.get_logger().error(f'❌ 问讯发送失败：API返回错误码 {resp_json.get("code")}, 消息: {resp_json.get("msg")}')
                    # Token可能失效，清空以便下次重新获取
                    self.token = None
                    return False
            else:
                self.get_logger().error(f'❌ 问讯请求失败，响应码: {resp.status_code}, 内容: {resp.text}')
                return False
                
        except requests.exceptions.Timeout:
            self.get_logger().error('❌ 问讯发送请求超时')
            return False
        except requests.exceptions.ConnectionError:
            self.get_logger().error('❌ 问讯发送连接失败：请检查网络或API地址')
            return False
        except Exception as e:
            self.get_logger().error(f'❌ 问讯发送异常：{str(e)}')
            return False

    def check_send_permission(self):
        """
        新增：检查是否满足发送间隔要求
        返回：bool - 是否允许发送，str - 原因
        """
        # 首次发送直接允许
        if self.last_send_time is None:
            return True, "首次发送，允许执行"
        
        # 计算时间差（秒）
        time_diff = (datetime.now() - self.last_send_time).total_seconds()
        if time_diff >= CONFIG["min_send_interval"]:
            return True, f"距离上次发送{time_diff/60:.1f}分钟，满足间隔要求"
        else:
            return False, f"距离上次发送仅{time_diff/60:.1f}分钟（需满45分钟），跳过发送"

    def message_callback(self, msg):
        """
        优化后：先检查发送间隔，再执行发送逻辑
        """
        content = msg.data
        self.get_logger().info(f'📥 收到话题消息: {content}')
        
        # 检查是否允许发送
        can_send, reason = self.check_send_permission()
        if not can_send:
            self.get_logger().info(f'⏳ {reason}')
            return
        
        # 满足条件则发送
        self.send_wenxun(content)

    def send_message_callback(self, request, response):
        """
        服务回调函数：同样加入间隔检查
        request: WeChatMessage.Request → 包含content和msg_type字段
        response: WeChatMessage.Response → 包含success和message字段
        """
        content = request.content
        msg_type = request.msg_type
        
        self.get_logger().info(f'📥 收到服务请求 - 类型: {msg_type}, 内容: {content}')
        
        # 检查是否允许发送
        can_send, reason = self.check_send_permission()
        if not can_send:
            self.get_logger().info(f'⏳ {reason}')
            response.success = False
            response.message = f"❌ 发送失败：{reason}"
            return response
        
        # 执行发送逻辑
        success = self.send_wenxun(content)
        
        # 构造响应
        response.success = success
        if success:
            response.message = f"✅ 问讯发送成功（类型：{msg_type}）"
        else:
            response.message = f"❌ 问讯发送失败（类型：{msg_type}）"
        
        return response


def main(args=None):
    """节点主函数"""
    # 初始化ROS2
    rclpy.init(args=args)
    
    # 创建节点实例
    node = WebWenxunSenderNode()
    
    try:
        # 启动时发送测试消息（可选）
        node.get_logger().info('📝 发送启动测试消息...')
        test_success = node.send_wenxun("[WEB问讯] ROS2节点启动成功，测试消息发送！")
        if test_success:
            node.get_logger().info('✅ 测试消息发送成功')
        else:
            node.get_logger().warn('⚠️ 测试消息发送失败，请检查配置')
        
        # 自旋节点（保持运行）
        rclpy.spin(node)
        
    except KeyboardInterrupt:
        node.get_logger().info('🛑 节点被用户手动停止')
    except Exception as e:
        node.get_logger().error(f'💥 节点运行异常：{str(e)}')
    finally:
        # 资源清理
        node.destroy_node()
        rclpy.shutdown()
        node.get_logger().info('🔚 节点已关闭')


if __name__ == '__main__':
    main()














