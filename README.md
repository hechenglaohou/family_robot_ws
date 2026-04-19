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


🧩 系统架构与功能模块
视觉感知：YOLOv8 目标检测 + 运动检测 + 共享内存低延迟读图
核心大脑：家人关怀、记忆系统、成长系统、昼夜节律、异常判断
消息系统：ROS2 话题通信、网页推送、远程提醒
信息监控：家庭信息、天气、日程、状态监控
设备控制：灯光、空调、智能家居自动 / 语音控制
数据存储：JSON 永久记忆存储
📦 环境要求
系统：Ubuntu 22.04 LTS
ROS2：Humble Hawksbill
语言：C++（核心节点）+ Python（视觉 / 工具）
编译：ament_cmake / colcon
内存：支持共享内存高速图像传输
依赖安装
bash
运行
sudo apt install -y ros-humble-desktop
sudo apt install -y ros-humble-cv-bridge libopencv-dev
sudo apt install -y nlohmann-json3-dev librt-dev
pip install opencv-python numpy
📂 项目文件结构
plaintext
family_robot_ws/
├── src/
│   └── family_robot_head/   # 机器人核心大脑（C++）
│       ├── include/
│       ├── src/
│       ├── CMakeLists.txt
│       └── package.xml
├── camera_shm_sim.py       # 摄像头共享内存模拟
├── camera_shm_real.py      # 真实摄像头采集
└── README.md
🔧 编译与部署
bash
运行
cd ~/family_robot_ws
colcon build --packages-select family_robot_head
source install/setup.bash
🧪 使用方法
启动核心大脑
bash
运行
ros2 run family_robot_head head_node
启动视觉（可选）
bash
运行
source /opt/.yolov8_venv/bin/activate
ros2 run yolov8_ros2 yolov8_ros2
语音 / 指令控制
bash
运行
ros2 topic pub /robot/voice_cmd std_msgs/String "data: 开灯" --once
ros2 topic pub /robot/voice_cmd std_msgs/String "data: 开空调25度" --once
🌗 昼夜守护模式
白天（06:00–22:00）：主动服务、交互、提醒、设备控制
夜晚（22:00–06:00）：安静守护、异常检测、紧急提醒
🚨 智能异常检测
家人长时间未出现
夜间熬夜不睡
久坐不动
摔倒风险监测
陌生人 / 异常画面监测
📡 ROS2 话题清单
发布
/robot/speech 机器人语音输出
/robot/growth 成长等级状态
/device/light 灯光控制
/device/aircon 空调控制
订阅
/robot/voice_cmd 语音指令
/camera/image 视觉图像
📌 永久记忆存储
记忆文件路径：
plaintext
/home/robot/family_memory.json
包含：家人习惯、交互记录、活动时间、异常事件、偏好设置。

### 📜 开源声明 & 第三方组件版权说明
1.  本项目**自定义业务源码**基于 **Apache 2.0** 协议开源，仅用于个人学习、嵌入式机器人技术研究。
2.  项目依赖所有第三方开源组件版权归原作者所有：
    - STM32 HAL库/CMSIS内核：版权归 STMicroelectronics 官方所有
    - FreeRTOS：版权归 Amazon Real-Time Engineers 所有，基于MIT协议
    - ROS2 Humble 生态组件：基于 Apache 2.0 开源协议
    - OpenCV、nlohmann_json：基于原生开源宽松协议
    - YOLOv8：版权归 Ultralytics 所有，本项目仅用于**个人非商用学习研究**
3.  所有底层库、官方固件仅作为开发依赖调用，未修改原生版权声明，不用于任何商业盈利行为。
4.  禁止将本项目全部/部分源码用于未经授权的商业售卖、产品商用。
5.  开源来源声明
   本项目底层基础框架思路来源于 **github** 开源项目，基于 **MIT 协议**二次开发。
