
 代码关键说明

    核心流程
        节点启动后，会同时订阅 wechat_message 话题和提供 send_wechat_message 服务
        当收到话题消息 / 服务请求时，先检查是否有有效 Token，无则自动调用登录接口获取
        对消息内容做校验和截断处理后，调用问讯发送 API 转发给目标网站
        返回发送结果（服务调用会返回结构化响应，话题订阅仅记录日志）
    关键优化点
        异常细分：将通用异常拆分为超时、连接错误等具体类型，便于定位问题
        配置化：将内容最大长度等硬编码值改为配置项，便于维护
        日志增强：添加更清晰的日志标识（✅/❌/🔑等），便于调试
        健壮性提升：增加内容空值校验、首尾空格去除等预处理逻辑
    使用前置条件
        确保已创建 wechat_interfaces 功能包，并包含 WeChatMessage 服务类型（服务定义需包含 content 和 msg_type 请求字段，success 和 message 响应字段）
        安装依赖：pip install requests urllib3
        确保 ROS2 环境已配置，节点所在功能包已正确编译
    测试方法
        启动节点：ros2 run  web_wenxun_sender_node  web_wenxun_sender
        发布话题测试：ros2 topic pub /wechat_message std_msgs/msg/String "{data: '测试问讯内容'}"  --once
        调用服务测试：ros2 service call /send_wechat_message wechat_interfaces/srv/WeChatMessage "{content: '服务测试内容', msg_type: 'test'}"

总结

    该节点的核心功能是ROS2 消息接收 + WEB API 转发，同时支持话题订阅和服务调用两种接收方式；
    核心逻辑包含 Token 自动获取、内容校验、API 请求发送三个关键步骤，确保消息能可靠转发到目标网站；
    使用时需重点检查 CONFIG 配置项（API 地址、用户名）、target_id 以及自定义服务类型的正确性，这是节点正常运行的关键。
    
股票监控    
# 安装ROS2基础依赖（如果未安装）
sudo apt update && sudo apt install ros-humble-std-msgs
# 安装股票数据请求依赖
pip install requests
