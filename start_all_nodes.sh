#!/bin/bash

# 定义工作空间路径
WORKSPACE="/home/hhc/family_robot_ws"

# 检查工作空间是否存在
if [ ! -d "$WORKSPACE" ]; then
    echo "错误：工作空间目录 $WORKSPACE 不存在！"
    exit 1
fi

# 切换到工作空间目录
cd "$WORKSPACE" || exit 1

# 定义启动节点的函数（复用逻辑）
start_ros2_node() {
    local title="$1"    # 终端窗口标题
    local command="$2"  # 要执行的命令
    
    # 使用 gnome-terminal 打开新终端并执行命令
    # 如果你的终端不是 gnome-terminal，请替换为 konsole/xterm 等
    gnome-terminal --title="$title" -- bash -c "
        cd $WORKSPACE;
        $command;
        exec bash;  # 执行完命令后保持终端打开
    "
}

# 1. 启动 talker_node
start_ros2_node "Talker Node" "source install/setup.bash && ros2 run learn_compose talker_node"

# 2. 启动 listener
start_ros2_node "Listener Node" "source install/setup.bash && ros2 run learn_compose listener"

# 3. 启动 motion_detector_node
start_ros2_node "Motion Detector Node" "source install/setup.bash && ros2 run learn_compose motion_detector_node"

# 4. 启动 yolov8_ros2（需要激活虚拟环境）
start_ros2_node "YOLOv8 ROS2 Node" "source /opt/.yolov8_venv/bin/activate && source install/setup.bash && ros2 run yolov8_ros2 yolov8_ros2"

# 5. 启动 head_node
start_ros2_node "Family Robot Head Node" "source install/setup.bash && ros2 run family_robot_head head_node"

# 6. 启动 web_wenxun_sender
start_ros2_node "Web Wenxun Sender Node" "source install/setup.bash && ros2 run web_wenxun_sender_node web_wenxun_sender"

# 7. 启动 stock_monitor
start_ros2_node "Stock Monitor Node" "source install/setup.bash && ros2 run stock_monitor_node stock_monitor"

echo "所有 ROS2 节点终端已启动！"

