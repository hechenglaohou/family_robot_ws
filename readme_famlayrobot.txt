source install/setup.bash 
ros2 run learn_compose talker_node 


source install/setup.bash 
ros2 run learn_compose listener

source install/setup.bash 
ros2 run learn_compose motion_detector_node 

source /opt/.yolov8_venv/bin/activate
source install/setup.bash 
 ros2 run yolov8_ros2 yolov8_ros2 
 
source install/setup.bash 
ros2 run family_robot_head head_node

source install/setup.bash 
ros2 run  web_wenxun_sender_node  web_wenxun_sender

source install/setup.bash 
ros2 run stock_monitor_node stock_monitor


家庭守护机器人 Head 节点
Family Guardian Robot Head Node - 非自我中心、具备家人关怀、自我成长、永久记忆能力的 ROS2 机器人核心节点，采用 C++ 实现保证高性能，支持共享内存读取摄像头数据，专注于守护家庭成员健康与安全。
项目简介
该节点是家庭陪伴机器人的核心大脑，具备以下核心特性：

    非自我中心：核心逻辑围绕家庭成员展开，关心家人健康、作息、安全，而非机械的自我描述
    时空感知：遵循昼夜节律（白天主动服务，夜间安静守护），感知时间、环境变化
    自我成长：基于交互 / 记忆数量自动升级，自我意识随使用逐步进化
    永久记忆：存储家人习惯、交互记录、异常事件，生命周期内记忆不丢失
    高性能：C++ 实现 + 共享内存读取摄像头，毫秒级响应
    设备控制：支持灯光、空调等智能家居控制
    异常检测：监测家人长时间未出现、久坐、熬夜、摔倒等异常状态并主动提醒

目录

    环境要求
    文件清单
    安装步骤
    使用方法
    核心功能测试
    扩展开发
    常见问题

环境要求
系统环境

    操作系统：Ubuntu 22.04 LTS
    ROS2 版本：Humble Hawksbill（推荐）
    编译器：GCC 11+ / Clang 14+

依赖库

    ROS2 核心依赖：rclcpp、std_msgs、sensor_msgs、cv_bridge
    系统依赖：libopencv-dev、nlohmann-json3-dev、librt-dev（共享内存）
    Python 依赖（可选，摄像头模拟）：opencv-python、numpy

文件清单
plaintext

family_robot_ws/                     # 工作空间
├── src/
│   └── family_robot_head/           # 功能包目录
│       ├── include/
│       │   └── family_robot_head/
│       │       └── head_node.hpp    # 核心头文件（类定义、结构体）
│       ├── src/
│       │   └── head_node.cpp        # 核心源文件（业务逻辑实现）
│       ├── CMakeLists.txt           # CMake 编译配置
│       └── package.xml              # ROS2 包配置
├── camera_shm_sim.py                # 摄像头共享内存模拟脚本
├── camera_shm_real.py               # 真实摄像头共享内存脚本
└── README.md                        # 项目说明文档

安装步骤
1. 基础环境安装
1.1 安装 ROS2 Humble（若未安装）
bash
运行

# 设置语言
sudo locale-gen en_US en_US.UTF-8
sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8
export LANG=en_US.UTF-8

# 添加 ROS2 源
sudo apt update && sudo apt install -y curl gnupg lsb-release
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(source /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

# 安装 ROS2 Humble 基础包
sudo apt update && sudo apt install -y ros-humble-desktop
sudo apt install -y ros-humble-dev-tools

# 初始化环境
source /opt/ros/humble/setup.bash
echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc

1.2 安装项目依赖
bash
运行

# ROS2 依赖
sudo apt install -y ros-humble-cv-bridge

# 系统依赖
sudo apt install -y libopencv-dev python3-opencv nlohmann-json3-dev librt-dev

# Python 依赖（摄像头模拟/真实摄像头）
pip install opencv-python numpy

2. 项目部署
2.1 克隆 / 创建项目
bash
运行

# 创建工作空间
mkdir -p ~/family_robot_ws/src
cd ~/family_robot_ws/src

# 创建功能包
ros2 pkg create family_robot_head \
  --build-type ament_cmake \
  --dependencies rclcpp std_msgs sensor_msgs cv_bridge nlohmann_json

# 替换功能包文件（将项目文件复制到对应目录）
# 复制 head_node.hpp 到 ~/family_robot_ws/src/family_robot_head/include/family_robot_head/
# 复制 head_node.cpp 到 ~/family_robot_ws/src/family_robot_head/src/
# 替换 CMakeLists.txt 和 package.xml

2.2 创建记忆目录
bash
运行

# 创建记忆存储目录（权限配置）
sudo mkdir -p /home/robot
sudo chown $USER:$USER /home/robot
chmod 755 /home/robot

2.3 编译项目
bash
运行

# 进入工作空间
cd ~/family_robot_ws

# 编译功能包
colcon build --packages-select family_robot_head --cmake-args -DCMAKE_BUILD_TYPE=Release

# 激活环境
source install/setup.bash
echo "source ~/family_robot_ws/install/setup.bash" >> ~/.bashrc

使用方法
1. 启动核心节点
1.1 方式 1：仅启动 Head 节点（无摄像头）
bash
运行

# 激活环境
source ~/family_robot_ws/install/setup.bash

# 启动 Head 节点
ros2 run family_robot_head head_node

    注：此时会提示 “共享内存打开失败”，但不影响核心功能（语音交互、记忆、成长）

1.2 方式 2：启动摄像头模拟 + Head 节点（推荐测试用）
bash
运行

# 终端1：启动摄像头共享内存模拟
cd ~/family_robot_ws
python3 camera_shm_sim.py

# 终端2：启动 Head 节点
source ~/family_robot_ws/install/setup.bash
ros2 run family_robot_head head_node

1.3 方式 3：启动真实摄像头 + Head 节点（实际部署用）
bash
运行

# 终端1：启动真实摄像头共享内存
cd ~/family_robot_ws
python3 camera_shm_real.py

# 终端2：启动 Head 节点
source ~/family_robot_ws/install/setup.bash
ros2 run family_robot_head head_node

2. 核心指令使用
2.1 语音指令交互
bash
运行

# 开灯
ros2 topic pub /robot/voice_cmd std_msgs/String "data: 开灯" --once

# 关灯
ros2 topic pub /robot/voice_cmd std_msgs/String "data: 关灯" --once

# 开空调（25度/26度/27度）
ros2 topic pub /robot/voice_cmd std_msgs/String "data: 开空调25度" --once

# 查询家人状态
ros2 topic pub /robot/voice_cmd std_msgs/String "data: 妈妈状态" --once

# 查询机器人自我认知
ros2 topic pub /robot/voice_cmd std_msgs/String "data: 你是谁" --once

2.2 查看节点输出
bash
运行

# 查看语音输出（机器人“说话”内容）
ros2 topic echo /robot/speech

# 查看成长状态（自我成长/意识进化）
ros2 topic echo /robot/growth

# 查看灯光控制指令
ros2 topic echo /device/light

# 查看空调控制指令
ros2 topic echo /device/aircon

# 查看节点日志
ros2 logs | grep family_robot_head

2.3 查看永久记忆
bash
运行

# 查看记忆文件（JSON格式）
cat /home/robot/family_memory.json

# 实时监控记忆文件变化
tail -f /home/robot/family_memory.json

核心功能测试
1. 家人关怀测试

    熬夜提醒：夜间（23:00-5:00）发布任意指令，节点会提醒 “XX 现在还没休息吗？熬夜对身体不好哦～”
    久坐提醒：模拟家人活动量过低（修改代码中activity_score为 < 30），节点会主动提醒活动
    长时间未出现提醒：修改代码中last_seen为 4 小时前，节点会关心 “好久没看到 XX 了，家人是否一切安好？”

2. 自我成长测试

    多次发布语音指令（≥100 次），节点会自动升级成长等级，并语音提示 “我又成长了！现在是 X 级...”
    查看/robot/growth话题，自我叙事会从 “学习认识家人”→“了解家人习惯”→“成为家庭一份子”

3. 记忆持久化测试

    发布若干指令后，停止节点并重启，查看/home/robot/family_memory.json，所有记录均未丢失

4. 设备控制测试

    发布 “开灯” 指令后，/device/light话题会输出data: true
    发布 “开空调 25 度” 指令后，/device/aircon话题会输出temperature: 25.0

扩展开发
1. 对接真实硬件

    灯光控制：将/device/light话题对接 ESP32/Arduino 等硬件，实现真实灯光开关
    空调控制：将/device/aircon话题对接空调红外遥控模块，实现温度调节
    语音合成：将/robot/speech话题对接 TTS 引擎（如百度 TTS、科大讯飞），实现语音播放

2. AI 能力扩展

    家人识别：集成 YOLO/OpenCV 人脸识别，替换模拟的 “家人状态检测”
    摔倒检测：集成姿态识别算法（如 MediaPipe），实现真实摔倒检测
    大模型对接：修改voice_callback函数，对接豆包 / 文心一言 API，实现自然语言聊天

3. 记忆系统优化

    将 JSON 记忆文件替换为 SQLite 数据库，支持更复杂的习惯分析和记忆检索
    添加记忆抽象化逻辑，自动总结家人行为模式（如 “爸爸每天 8 点看报”）

4. 昼夜节律优化

    修改代码中check_abnormal_status函数，夜间降低检测频率，仅保留危险检测（摔倒、火灾等）

常见问题
1. 编译错误：Temperature is not a member of std_msgs::msg

    原因：Temperature消息类型属于sensor_msgs/msg，而非std_msgs/msg
    解决：将所有std_msgs::msg::Temperature替换为sensor_msgs::msg::Temperature

2. 编译错误：opencv_vendor not found

    原因：opencv_vendor并非系统默认安装包
    解决：改用系统版 OpenCV，修改CMakeLists.txt为find_package(OpenCV REQUIRED)

3. 共享内存打开失败

    原因：未创建/robot_camera_shm共享内存
    解决：运行camera_shm_sim.py或camera_shm_real.py创建共享内存

4. 记忆文件写入失败

    原因：/home/robot目录权限不足
    解决：执行sudo chown $USER:$USER /home/robot赋予当前用户权限

5. 节点启动后无响应

    原因：ROS2 环境未激活或编译缓存残留
    解决：
    bash
    运行

    source ~/family_robot_ws/install/setup.bash
    rm -rf build/ install/ log/
    colcon build --packages-select family_robot_head

许可证
本项目基于 Apache 2.0 许可证开源，可自由修改、分发和商用，保留原作者声明即可。
维护者

    核心功能：家庭守护机器人 Head 节点开发
    联系方式：（可补充个人 / 团队联系方式）

更新日志

    v1.0：基础版本，实现家人关怀、自我成长、永久记忆、设备控制核心功能
    v1.1：修复编译错误，优化共享内存逻辑，完善记忆持久化

Package                              Version
------------------------------------ ----------------
ackermann-msgs                       2.0.2
action-msgs                          1.2.2
action-tutorials-interfaces          0.20.8
action-tutorials-py                  0.20.8
actionlib-msgs                       4.9.1
ament-cmake-test                     1.3.13
ament-copyright                      0.12.15
ament-cppcheck                       0.12.15
ament-cpplint                        0.12.15
ament-flake8                         0.12.15
ament-index-python                   1.4.1
ament-lint                           0.12.15
ament-lint-cmake                     0.12.15
ament-package                        0.14.2
ament-pep257                         0.12.15
ament-uncrustify                     0.12.15
ament-xmllint                        0.12.15
angles                               1.15.0
appdirs                              1.4.4
apturl                               0.5.2
argcomplete                          1.8.1
attrs                                21.2.0
Babel                                2.8.0
bcrypt                               3.2.0
beautifulsoup4                       4.10.0
beniget                              0.4.1
blinker                              1.4
bond                                 4.1.2
Brlapi                               0.8.3
Brotli                               1.0.9
builtin-interfaces                   1.2.2
catkin-pkg-modules                   1.1.0
certifi                              2020.6.20
chardet                              4.0.0
click                                8.0.3
colcon-argcomplete                   0.3.3
colcon-bash                          0.5.0
colcon-cd                            0.2.1
colcon-cmake                         0.2.29
colcon-common-extensions             0.3.0
colcon-core                          0.20.1
colcon-defaults                      0.2.9
colcon-devtools                      0.3.0
colcon-installed-package-information 0.2.1
colcon-library-path                  0.2.1
colcon-metadata                      0.2.5
colcon-notification                  0.3.0
colcon-output                        0.2.13
colcon-override-check                0.0.1
colcon-package-information           0.4.0
colcon-package-selection             0.2.10
colcon-parallel-executor             0.3.0
colcon-pkg-config                    0.1.0
colcon-powershell                    0.4.0
colcon-python-setup-py               0.2.9
colcon-recursive-crawl               0.2.3
colcon-ros                           0.5.0
colcon-test-result                   0.3.8
colcon-zsh                           0.5.0
colorama                             0.4.4
coloredlogs                          15.0.1
command-not-found                    0.3
composition-interfaces               1.2.2
control-msgs                         4.8.0
controller-manager                   2.53.1
controller-manager-msgs              2.53.1
cov-core                             1.15.0
coverage                             6.2
cryptography                         3.4.8
cuda-bindings                        12.9.4
cuda-pathfinder                      1.3.4
cupshelpers                          1.0
cv-bridge                            3.2.1
cycler                               0.11.0
dbus-python                          1.2.18
decorator                            4.4.2
defer                                1.0.6
demo-nodes-py                        0.20.8
diagnostic-msgs                      4.9.1
diagnostic-updater                   4.0.6
distlib                              0.3.4
distro                               1.7.0
distro-info                          1.1+ubuntu0.2
dlib                                 20.0.0
docutils                             0.17.1
domain-coordinator                   0.10.0
duplicity                            0.8.21
dwb-msgs                             1.1.20
empy                                 3.3.4
espeakng                             1.0.3
example-interfaces                   0.9.3
examples-rclpy-executors             0.15.5
examples-rclpy-minimal-action-client 0.15.5
examples-rclpy-minimal-action-server 0.15.5
examples-rclpy-minimal-client        0.15.5
examples-rclpy-minimal-publisher     0.15.5
examples-rclpy-minimal-service       0.15.5
examples-rclpy-minimal-subscriber    0.15.5
face-recognition                     1.3.0
face-recognition-models              0.3.0
fasteners                            0.14.1
filelock                             3.24.0
flake8                               4.0.1
flatbuffers                          25.12.19
fonttools                            4.29.1
fs                                   2.4.12
fsspec                               2026.2.0
future                               0.18.2
gast                                 0.5.2
gazebo-model-attachment-plugin       1.0.3
gazebo-model-attachment-plugin-msgs  1.0.3
gazebo-msgs                          3.9.0
gazebo-video-monitor-interfaces      0.8.1
gazebo-video-monitor-utils           0.8.1
generate-parameter-library-py        0.6.0
geometry-msgs                        4.9.1
html5lib                             1.1
httplib2                             0.20.2
humanfriendly                        10.0
idna                                 3.3
importlib-metadata                   4.6.4
iniconfig                            1.1.1
interactive-markers                  2.3.2
jeepney                              0.7.1
Jinja2                               3.0.3
joint-state-publisher                2.4.0
keyring                              23.5.0
kiwisolver                           1.3.2
language-selector                    0.1
lark                                 1.1.1
laser-geometry                       2.4.1
launch                               1.0.14
launch-ros                           0.19.13
launch-testing                       1.0.14
launch-testing-ros                   0.19.13
launch-xml                           1.0.14
launch-yaml                          1.0.14
launchpadlib                         1.10.16
lazr.restfulclient                   0.14.4
lazr.uri                             1.0.6
lifecycle-msgs                       1.2.2
lockfile                             0.12.2
logging-demo                         0.20.8
louis                                3.20.0
lxml                                 4.8.0
lz4                                  3.1.3+dfsg
macaroonbakery                       1.3.1
Mako                                 1.1.3
map-msgs                             2.1.0
MarkupSafe                           2.0.1
matplotlib                           3.5.1
mccabe                               0.6.1
message-filters                      4.3.14
ml_dtypes                            0.5.4
monotonic                            1.6
more-itertools                       8.10.0
mpi4py                               3.1.3
mpmath                               1.3.0
nav-2d-msgs                          1.1.20
nav-msgs                             4.9.1
nav2-common                          1.1.20
nav2-msgs                            1.1.20
nav2-simple-commander                1.0.0
netifaces                            0.11.0
networkx                             3.4.2
nose2                                0.9.2
notify2                              0.3
numpy                                1.26.4
nvidia-cublas-cu12                   12.8.4.1
nvidia-cuda-cupti-cu12               12.8.90
nvidia-cuda-nvrtc-cu12               12.8.93
nvidia-cuda-runtime-cu12             12.8.90
nvidia-cudnn-cu12                    9.10.2.21
nvidia-cufft-cu12                    11.3.3.83
nvidia-cufile-cu12                   1.13.1.3
nvidia-curand-cu12                   10.3.9.90
nvidia-cusolver-cu12                 11.7.3.90
nvidia-cusparse-cu12                 12.5.8.93
nvidia-cusparselt-cu12               0.7.1
nvidia-nccl-cu12                     2.27.5
nvidia-nvjitlink-cu12                12.8.93
nvidia-nvshmem-cu12                  3.4.5
nvidia-nvtx-cu12                     12.8.90
oauthlib                             3.2.0
olefile                              0.46
onnx                                 1.20.1
onnxruntime                          1.23.2
opencv-python                        4.13.0.92
osrf-pycommon                        2.1.6
packaging                            21.3
paramiko                             2.9.3
pcl-msgs                             1.0.0
pendulum-msgs                        0.20.8
pexpect                              4.8.0
Pillow                               9.0.1
pip                                  22.0.2
pluggy                               0.13.0
ply                                  3.11
protobuf                             6.33.5
psutil                               5.9.0
ptyprocess                           0.7.0
py                                   1.10.0
pycairo                              1.20.1
pycodestyle                          2.8.0
pycups                               2.0.1
pydocstyle                           6.1.1
pydot                                1.4.2
pyflakes                             2.4.0
Pygments                             2.11.2
PyGObject                            3.42.1
pygraphviz                           1.7
PyJWT                                2.3.0
pymacaroons                          0.13.0
PyNaCl                               1.5.0
pyparsing                            2.4.7
PyQt5                                5.15.6
PyQt5-sip                            12.9.1
pyRFC3339                            1.1
pytest                               6.2.5
pytest-cov                           3.0.0
python-apt                           2.4.0+ubuntu4.1
python-dateutil                      2.8.1
python-debian                        0.1.43+ubuntu1.1
python-qt-binding                    1.1.3
pythran                              0.10.0
pytz                                 2022.1
pyxdg                                0.27
PyYAML                               5.4.1
qt-dotgraph                          2.2.4
qt-gui                               2.2.4
qt-gui-cpp                           2.2.4
qt-gui-py-common                     2.2.4
quality-of-service-demo-py           0.20.8
rcl-interfaces                       1.2.2
rclpy                                3.3.20
rcutils                              5.1.8
reportlab                            3.6.8
requests                             2.25.1
resource-retriever                   3.1.3
rmw-dds-common                       1.6.0
roman                                3.3
ros2action                           0.18.17
ros2bag                              0.15.16
ros2cli                              0.18.17
ros2component                        0.18.17
ros2controlcli                       2.53.1
ros2doctor                           0.18.17
ros2interface                        0.18.17
ros2launch                           0.19.13
ros2lifecycle                        0.18.17
ros2multicast                        0.18.17
ros2node                             0.18.17
ros2param                            0.18.17
ros2pkg                              0.18.17
ros2plugin                           5.1.3
ros2run                              0.18.17
ros2service                          0.18.17
ros2topic                            0.18.17
rosbag2-interfaces                   0.15.16
rosbag2-py                           0.15.16
rosdep                               0.26.0
rosdep-modules                       0.26.0
rosdistro-modules                    1.0.1
rosgraph-msgs                        1.2.2
rosidl-adapter                       3.1.8
rosidl-cli                           3.1.8
rosidl-cmake                         3.1.8
rosidl-generator-c                   3.1.8
rosidl-generator-cpp                 3.1.8
rosidl-generator-py                  0.14.6
rosidl-parser                        3.1.8
rosidl-runtime-py                    0.9.3
rosidl-typesupport-c                 2.0.2
rosidl-typesupport-cpp               2.0.2
rosidl-typesupport-fastrtps-c        2.2.4
rosidl-typesupport-fastrtps-cpp      2.2.4
rosidl-typesupport-introspection-c   3.1.8
rosidl-typesupport-introspection-cpp 3.1.8
rospkg-modules                       1.6.1
rpyutils                             0.2.2
rqt-action                           2.0.1
rqt-bag                              1.1.6
rqt-bag-plugins                      1.1.6
rqt-console                          2.0.3
rqt-graph                            1.3.2
rqt-gui                              1.1.9
rqt-gui-py                           1.1.9
rqt-msg                              1.2.0
rqt-plot                             1.1.5
rqt-publisher                        1.5.0
rqt-py-common                        1.1.9
rqt-py-console                       1.0.2
rqt-reconfigure                      1.1.3
rqt-service-caller                   1.0.5
rqt-shell                            1.0.2
rqt-srv                              1.0.3
rqt-topic                            1.5.1
scipy                                1.8.0
scripts                              3.9.0
SecretStorage                        3.3.1
sensor-msgs                          4.9.1
sensor-msgs-py                       4.9.1
setuptools                           59.6.0
shape-msgs                           4.9.1
six                                  1.16.0
slam-toolbox                         2.6.10
smclib                               4.1.2
snowballstemmer                      2.2.0
soupsieve                            2.3.1
sros2                                0.10.9
ssh-import-id                        5.11
statistics-msgs                      1.2.2
std-msgs                             4.9.1
std-srvs                             4.9.1
stereo-msgs                          4.9.1
sympy                                1.14.0
systemd-python                       234
teleop-twist-keyboard                2.4.1
tf-transformations                   1.1.1
tf2-geometry-msgs                    0.25.19
tf2-kdl                              0.25.19
tf2-msgs                             0.25.19
tf2-py                               0.25.19
tf2-ros-py                           0.25.19
tf2-sensor-msgs                      0.25.19
tf2-tools                            0.25.19
toml                                 0.10.2
topic-monitor                        0.20.8
torch                                2.10.0
torchvision                          0.25.0
trajectory-msgs                      4.9.1
transforms3d                         0.3.1
triton                               3.6.0
turtlesim                            1.4.3
typeguard                            2.2.2
typing_extensions                    4.15.0
ubuntu-drivers-common                0.0.0
ubuntu-pro-client                    8001
ufoLib2                              0.13.1
ufw                                  0.36.1
unattended-upgrades                  0.1
unicodedata2                         14.0.0
unique-identifier-msgs               2.2.1
urllib3                              1.26.5
usb-creator                          0.3.7
visualization-msgs                   4.9.1
wadllib                              1.3.6
webencodings                         0.5.1
wheel                                0.37.1
xacro                                2.1.1
xdg                                  5
xkit                                 0.0.0
zipp                                 1.0.0
