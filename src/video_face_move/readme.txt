项目 README.md（完整使用说明）
创建路径：learn_compose_hhc/learn_compose/README.md（包内根目录），内容如下：
markdown

# learn_compose 项目使用说明
## 项目简介
本项目是基于ROS2的人脸检测+识别节点，包含Talker（图像发布）、Listener（人脸识别）、MotionDetector（运动检测）三个组件，核心功能是订阅摄像头图像话题，实时检测并识别指定人员（laohou/pan/kid）。

## 一、环境依赖
1. ROS2 Humble/Iron（其他版本需适配）
2. OpenCV 4.x（需包含contrib模块，支持人脸检测/识别）
3. 依赖包安装：
   ```bash
   sudo apt install ros-humble-rclcpp-components ros-humble-cv-bridge libopencv-contrib-dev

二、目录结构说明
plaintext

learn_compose/
├── config/                # 配置文件目录（核心！必须放在此路径）
│   └── haarcascade_frontalface_default.xml  # OpenCV人脸检测器配置文件
├── include/learn_compose/ # 头文件目录
│   ├── listener.hpp       # 人脸识别节点头文件
│   ├── talker.hpp         # 图像发布节点头文件
│   └── motion_detector.hpp # 运动检测节点头文件
├── src/                   # 源文件目录
│   ├── listener.cpp       # 人脸识别核心逻辑
│   ├── talker.cpp         # 图像发布逻辑
│   └── motion_detector.cpp # 运动检测逻辑
├── CMakeLists.txt         # 编译配置
├── package.xml            # ROS2包配置
└── README.md              # 使用说明

三、快速上手
1. 配置文件准备
方式 1：自动下载（推荐）
bash
运行

# 进入包内config目录
cd /home/hhc/sy/learn_compose_hhc/learn_compose/config
# 下载人脸检测器配置文件
wget https://raw.githubusercontent.com/opencv/opencv/master/data/haarcascades/haarcascade_frontalface_default.xml

方式 2：本地复制（若已安装 OpenCV）
bash
运行

# 查找本地配置文件
find /usr -name "haarcascade_frontalface_default.xml"
# 复制到包内config目录（替换<路径>为实际找到的路径）
cp <路径> /home/hhc/sy/learn_compose_hhc/learn_compose/config/

2. 编译项目
bash
运行

# 进入工作空间根目录
cd /home/hhc/sy/learn_compose_hhc
# 清理旧编译产物
rm -rf build/ install/ log/
# 编译learn_compose包
colcon build --packages-select learn_compose
# 加载环境变量
source install/setup.bash

3. 运行节点
运行人脸识别节点（Listener）
bash
运行

ros2 run learn_compose listener_node

运行图像发布节点（Talker）
bash
运行

ros2 run learn_compose talker_node

运行运动检测节点（MotionDetector）
bash
运行

ros2 run learn_compose motion_detector_node

4. 验证功能
运行 Listener 节点后，控制台会输出：

    ✅ 人脸检测器加载成功
    ✅ Config 目录存在且有写入权限
    收到图像帧时，输出 “检测到 X 个人脸”+ 识别结果（如：laohou (95%)）

四、关键注意事项
1. 路径问题（核心！）

    config 文件夹必须放在 learn_compose 包内（/learn_compose/config），不可放在工作空间根目录；
    若修改了工作空间路径，需同步修改 listener.cpp 中的WORKSPACE_PATH常量：
    cpp
    运行

    const std::string Listener::WORKSPACE_PATH = "你的工作空间根目录";

2. 人脸识别模型准备

    首次运行会提示 “未找到人脸识别模型”，需先训练模型并放在face_model.yml路径：
    plaintext

    /home/hhc/sy/learn_compose_hhc/face_model.yml

    支持识别的人员 ID 与名称对应：
        ID 1 → laohou
        ID 2 → pan
        ID 3 → kid

3. 常见问题解决
问题 1：编译报错 “找不到 config 目录”

    检查：是否创建了 learn_compose/config 目录；
    修复：执行mkdir -p /home/hhc/sy/learn_compose_hhc/learn_compose/config。

问题 2：运行时 “人脸检测器加载失败”

    检查：config 目录下是否有haarcascade_frontalface_default.xml文件；
    修复：重新下载配置文件（参考步骤 1）。

问题 3：权限不足

    修复：给目录添加权限
    bash
    运行

    chmod -R 775 /home/hhc/sy/learn_compose_hhc/learn_compose
    
    
    方案 1：强制 ROS2 使用虚拟环境的 Python（最直接）
在运行节点前，先将虚拟环境的 Python 路径添加到系统最优先位置：
bash
运行

# 1. 激活虚拟环境（确保路径正确）
source ~/sy/find_face/face_find_venv/bin/activate

# 2. 导出 Python 路径（强制 ROS2 使用此 Python）
export PYTHONPATH=/home/hhc/sy/find_face/face_find_venv/lib/python3.10/site-packages:$PYTHONPATH
export PATH=/home/hhc/sy/find_face/face_find_venv/bin:$PATH

# 3. 重新加载 ROS2 环境
cd ~/sy/video_face_move
source install/setup.bash

# 4. 运行节点（关键：在同一终端执行）
ros2 run yolov8_ros2 yolov8_ros2



