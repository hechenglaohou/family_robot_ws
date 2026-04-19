#!/bin/bash
# 功能：精准定位并强制关闭所有 ROS2 节点
# 用法：chmod +x kill_all_ros2_nodes.sh && ./kill_all_ros2_nodes.sh

# 定义颜色输出（可选，方便看结果）
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # 重置颜色

# 步骤1：获取所有与 ROS2 相关的进程 PID（多维度匹配，避免漏杀）
echo -e "${GREEN}正在查找所有 ROS2 相关进程...${NC}"
# 匹配特征：包含 rcl/ros2/ROS_ 关键词、排除脚本自身和grep进程
ROS2_PIDS=$(ps -ef | grep -E 'rcl|ros2|ROS_' | grep -v grep | grep -v "$0" | awk '{print $2}')

# 步骤2：如果找到进程，逐个强制杀死
if [ -z "$ROS2_PIDS" ]; then
    echo -e "${GREEN}未找到任何 ROS2 节点进程${NC}"
else
    echo -e "${RED}找到以下 ROS2 进程 PID：${NC} $ROS2_PIDS"
    for pid in $ROS2_PIDS; do
        # 强制杀死进程（-9 是强制终止，确保杀死）
        kill -9 "$pid" > /dev/null 2>&1
        if [ $? -eq 0 ]; then
            echo -e "${GREEN}成功杀死进程 PID: $pid${NC}"
        else
            echo -e "${RED}杀死进程 PID: $pid 失败（可能已退出）${NC}"
        fi
    done
fi

# 步骤3：重启 ROS2 守护进程，避免后续命令报错
echo -e "${GREEN}重启 ROS2 守护进程...${NC}"
ros2 daemon stop > /dev/null 2>&1
ros2 daemon start > /dev/null 2>&1

# 步骤4：验证结果
echo -e "\n${GREEN}当前剩余 ROS2 节点列表：${NC}"
ros2 node list

echo -e "\n${GREEN}操作完成！${NC}"

