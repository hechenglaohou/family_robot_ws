# 家庭守护机器人・Family Guardian Robot
基于 ROS2 Humble 的全功能家庭智能机器人平台

**家人关怀 | 永久记忆 | 自我成长 | 昼夜守护 | 视觉感知 | 设备联动 | 异常监测**

---

## 🧠 项目简介
**家庭守护机器人 Head 节点**  
是整个机器人系统的核心大脑，采用高性能 C++ 开发，具备真正的家人关怀能力、永久记忆、自我成长进化、昼夜节律感知，可联动视觉、语音、传感器与智能家居，24 小时守护家人安全与健康。

### 核心特性
✅ **非自我中心设计** —— 一切围绕家人，而非机械交互  
✅ **时空感知能力** —— 遵循昼夜节律，白天服务 / 夜间守护  
✅ **永久记忆系统** —— 家人习惯、交互、异常事件终身存储不丢失  
✅ **自我成长进化** —— 交互越多越智能，等级自动升级  
✅ **共享内存视觉** —— 超低延迟读取摄像头数据  
✅ **异常智能检测** —— 熬夜、久坐、摔倒、长时间未出现自动提醒  
✅ **智能家居联动** —— 灯光、空调、家电一体化控制  
✅ **ROS2 全生态兼容** —— 可对接导航、SLAM、YOLO、语音合成  

---

## 🚀 一键启动命令（最常用）
```bash
# 1. 消息收发测试
source install/setup.bash
ros2 run learn_compose talker_node

source install/setup.bash
ros2 run learn_compose listener

# 2. 运动检测
source install/setup.bash
ros2 run learn_compose motion_detector_node

# 3. YOLOv8 视觉识别
source /opt/.yolov8_venv/bin/activate
source install/setup.bash
ros2 run yolov8_ros2 yolov8_ros2

# 4. 机器人核心大脑（主节点）
source install/setup.bash
ros2 run family_robot_head head_node

# 5. 网页消息推送
source install/setup.bash
ros2 run web_wenxun_sender_node web_wenxun_sender

# 6. 家庭信息监控
source install/setup.bash
ros2 run stock_monitor_node stock_monitor
