
#include "family_robot_head/head_node.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <nlohmann/json.hpp>
#include <fstream>
#include <ctime>
#include <chrono>

// 常量定义
#define MEMORY_PATH "/home/robot/family_memory.json"

// 构造函数：初始化所有核心模块
HeadNode::HeadNode(const rclcpp::NodeOptions & options)
: Node("family_robot_head", options) {
  // 1. 初始化发布者
  speech_pub_ = this->create_publisher<std_msgs::msg::String>("robot/speech", 10);
  light_ctrl_pub_ = this->create_publisher<std_msgs::msg::Bool>("device/light", 10);
  ac_ctrl_pub_ = this->create_publisher<sensor_msgs::msg::Temperature>("device/aircon", 10);
  growth_pub_ = this->create_publisher<std_msgs::msg::String>("robot/growth", 10);

  // 2. 初始化订阅者
  voice_sub_ = this->create_subscription<std_msgs::msg::String>(
    "robot/voice_cmd", 10,
    std::bind(&HeadNode::voice_callback, this, std::placeholders::_1));
  
  // 新增状态字符串订阅（替换摄像头输入）
  status_sub_ = this->create_subscription<std_msgs::msg::String>(
    "robot/status_str", 10,
    std::bind(&HeadNode::status_callback, this, std::placeholders::_1));

  // 3. 初始化定时器
  env_check_timer_ = this->create_wall_timer(
    std::chrono::seconds(120),
    std::bind(&HeadNode::env_check_callback, this));
  
  growth_timer_ = this->create_wall_timer(
    std::chrono::seconds(60),
    std::bind(&HeadNode::growth_callback, this));

  // 4. 初始化核心数据
  init_family_members();
  init_self_awareness();
  load_memory();

  // 5. 启动问候
  speak("我已启动，会一直守护这个家，关心每一位家人～");
  RCLCPP_INFO(this->get_logger(), "家庭守护机器人Head节点启动成功！");
}

// 析构函数：释放资源
HeadNode::~HeadNode() {
  // 保存最终记忆
  save_memory();
  speak("我会记住所有关于这个家的事，下次见～");
}

// 初始化家庭成员
void HeadNode::init_family_members() {
  family_members_.clear();
  
  FamilyMember owner = {"主人", 30, "习惯晚睡，喜欢26度空调", get_current_time(), 80.0, false};
  FamilyMember mom = {"妈妈", 55, "习惯早起，怕凉", get_current_time(), 75.0, false};
  FamilyMember dad = {"爸爸", 58, "久坐看报，忘关灯光", get_current_time(), 60.0, false};
  
  family_members_.push_back(owner);
  family_members_.push_back(mom);
  family_members_.push_back(dad);
}

// 初始化自我意识
void HeadNode::init_self_awareness() {
  self_awareness_.core_mission = "守护家人健康与安全，总结家人习惯，提供贴心服务";
  self_awareness_.growth_level = 1;
  self_awareness_.boot_time = std::chrono::system_clock::now();
  self_awareness_.self_narrative = "我是家庭守护机器人，我的存在是为了让家人更舒服、更安全";
  calculate_uptime();
}

// 加载永久记忆
void HeadNode::load_memory() {
  std::ifstream file(MEMORY_PATH);
  if (file.is_open()) {
    file >> memory_data_;
    file.close();
    RCLCPP_INFO(this->get_logger(), "成功加载永久记忆，共%ld条记录", 
      memory_data_["records"].size());
  } else {
    memory_data_["family_habits"] = nlohmann::json::array();
    memory_data_["abnormal_events"] = nlohmann::json::array();
    memory_data_["growth_records"] = nlohmann::json::array();
    memory_data_["records"] = nlohmann::json::array();
    save_memory();
    RCLCPP_INFO(this->get_logger(), "创建新的记忆文件");
  }
}

// 保存永久记忆
void HeadNode::save_memory() {
  std::ofstream file(MEMORY_PATH);
  if (file.is_open()) {
    file << std::setw(2) << memory_data_;
    file.close();
    RCLCPP_INFO(this->get_logger(), "记忆已保存到：%s", MEMORY_PATH);
  } else {
    RCLCPP_ERROR(this->get_logger(), "记忆保存失败！");
  }
}

// 每2分钟环境检查核心逻辑（适配无摄像头）
void HeadNode::env_check_callback() {
  RCLCPP_INFO(this->get_logger(), "开始环境检查...");
  
  // 移除摄像头相关逻辑，直接检查异常状态（默认正常）
  check_abnormal_status("normal");
  
  // 总结家人习惯
  summarize_family_habits();

  // 记录本次检查
  nlohmann::json record;
  record["time"] = get_current_time();
  record["action"] = "环境检查";
  record["family_status"] = "正常";
  memory_data_["records"].push_back(record);
  
  // 保存记忆
  save_memory();
}

// 自我成长核心逻辑
void HeadNode::growth_callback() {
  update_self_awareness();
  calculate_uptime();
  upgrade_growth_level();

  // 发布成长状态
  std_msgs::msg::String growth_msg;
  growth_msg.data = "成长等级：" + std::to_string(self_awareness_.growth_level) + 
                    " | 通电时长：" + self_awareness_.uptime +
                    " | 自我认知：" + self_awareness_.self_narrative;
  growth_pub_->publish(growth_msg);

  // 记录成长
  nlohmann::json growth_record;
  growth_record["time"] = get_current_time();
  growth_record["level"] = self_awareness_.growth_level;
  growth_record["narrative"] = self_awareness_.self_narrative;
  memory_data_["growth_records"].push_back(growth_record);
}

// 语音指令处理（保持原有）
void HeadNode::voice_callback(const std_msgs::msg::String::SharedPtr msg) {
  std::string cmd = msg->data;
  RCLCPP_INFO(this->get_logger(), "收到语音指令：%s", cmd.c_str());

  // 设备控制
  if (cmd.find("开灯") != std::string::npos) {
    control_device("light", 1.0);
    speak("已为你打开灯光～");
  } else if (cmd.find("关灯") != std::string::npos) {
    control_device("light", 0.0);
    speak("已为你关闭灯光～");
  } else if (cmd.find("空调") != std::string::npos) {
    float temp = 26.0;
    if (cmd.find("25度") != std::string::npos) temp = 25.0;
    if (cmd.find("27度") != std::string::npos) temp = 27.0;
    control_device("aircon", temp);
    speak("已将空调温度设置为" + std::to_string(temp) + "度～");
  } 
  // 家人状态查询
  else if (cmd.find("妈妈") != std::string::npos && cmd.find("状态") != std::string::npos) {
    speak("妈妈最后出现时间是" + family_members_[1].last_seen + 
          "，状态正常，习惯是" + family_members_[1].habit);
  } 
  // 自我认知查询
  else if (cmd.find("你是谁") != std::string::npos) {
    speak(self_awareness_.self_narrative + "，我已经陪伴这个家" + self_awareness_.uptime + "了～");
  } 
  // 默认回复
  else {
    speak("我记住了你的需求，会慢慢学习怎么更好地帮你～");
  }

  // 记录交互
  nlohmann::json record;
  record["time"] = get_current_time();
  record["action"] = "语音交互";
  record["command"] = cmd;
  memory_data_["records"].push_back(record);
}

// 新增：状态字符串回调（核心）
void HeadNode::status_callback(const std_msgs::msg::String::SharedPtr msg) {
  std::string status_str = msg->data;
  RCLCPP_INFO(this->get_logger(), "收到状态字符串：%s", status_str.c_str());

  // 针对不同字符串搭建空反应框架
  if (status_str == "连续45分钟无人活动") {
    // 无人活动反应逻辑（空实现，可扩展）
    RCLCPP_WARN(this->get_logger(), "检测到连续45分钟无人活动！");
    speak("检测到家中连续45分钟无人活动，是否需要关闭非必要设备？");
    
    // 记录异常事件
    nlohmann::json abnormal;
    abnormal["time"] = get_current_time();
    abnormal["event"] = "连续45分钟无人活动";
    memory_data_["abnormal_events"].push_back(abnormal);
    
    // 可扩展：自动关闭灯光/空调等
    // control_device("light", 0.0);
    // control_device("aircon", 0.0);
  } 
  else if (status_str == "检测到摔倒") {
    // 摔倒检测反应逻辑（空实现）
    RCLCPP_ERROR(this->get_logger(), "检测到有人摔倒！");
    speak("检测到有人摔倒！请家人注意安全～");
    
    nlohmann::json abnormal;
    abnormal["time"] = get_current_time();
    abnormal["event"] = "摔倒检测";
    memory_data_["abnormal_events"].push_back(abnormal);
  }
  else if (status_str == "家人活动正常") {
    // 正常活动反应逻辑（空实现）
    RCLCPP_INFO(this->get_logger(), "家人活动状态正常");
    
    // 更新家人最后出现时间
    for (auto & member : family_members_) {
      member.last_seen = get_current_time();
      member.activity_score += 1.0;
      if (member.activity_score > 100.0) {
        member.activity_score = 100.0;
      }
    }
  }
  else if (status_str == "灯光未关闭") {
    // 灯光未关反应逻辑（空实现）
    RCLCPP_WARN(this->get_logger(), "检测到灯光未关闭！");
    speak("检测到家中灯光未关闭，是否需要关闭？");
    
    nlohmann::json abnormal;
    abnormal["time"] = get_current_time();
    abnormal["event"] = "灯光未关闭";
    memory_data_["abnormal_events"].push_back(abnormal);
  }
  else if (status_str == "空调温度异常") {
    // 空调异常反应逻辑（空实现）
    RCLCPP_WARN(this->get_logger(), "检测到空调温度异常！");
    speak("检测到空调温度异常，是否需要调整为26度？");
    
    nlohmann::json abnormal;
    abnormal["time"] = get_current_time();
    abnormal["event"] = "空调温度异常";
    memory_data_["abnormal_events"].push_back(abnormal);
  }
  else {
    // 未知状态处理
    RCLCPP_INFO(this->get_logger(), "收到未知状态字符串：%s", status_str.c_str());
    speak("收到状态通知：" + status_str);
  }

  // 记录状态事件
  nlohmann::json record;
  record["time"] = get_current_time();
  record["action"] = "状态监测";
  record["status"] = status_str;
  memory_data_["records"].push_back(record);
  
  // 检查异常状态
  check_abnormal_status(status_str);
}

// 异常状态检查（适配字符串输入）
void HeadNode::check_abnormal_status(const std::string & status_str) {
  std::string current_time = get_current_time();
  
  if (status_str == "连续45分钟无人活动") {
    // 标记所有家人异常
    for (auto & member : family_members_) {
      member.is_abnormal = true;
    }
    speak("好久没看到家人了，是否一切安好？");
  }
  else if (status_str == "检测到摔倒") {
    // 通用摔倒提醒
    speak("检测到异常情况，请家人及时查看！");
  }
  else {
    // 正常状态重置异常标记
    for (auto & member : family_members_) {
      member.is_abnormal = false;
    }
  }

  // 原有久坐检查逻辑保留
  for (auto & member : family_members_) {
    if (member.activity_score < 30.0) {
      remind_health(member.name);
    }

    // 作息不规律检查
    int hour = std::stoi(current_time.substr(11, 2));
    if (hour >= 23 || hour <= 5) {
      speak(member.name + "现在还没休息吗？熬夜对身体不好哦～");
    }
  }
}

// 获取当前时间字符串
std::string HeadNode::get_current_time() {
  auto now = std::chrono::system_clock::now();
  std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  std::stringstream ss;
  ss << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S");
  return ss.str();
}

// 语音输出
void HeadNode::speak(const std::string & text) {
  std_msgs::msg::String speech_msg;
  speech_msg.data = text;
  speech_pub_->publish(speech_msg);
  RCLCPP_INFO(this->get_logger(), "语音输出：%s", text.c_str());
}

// 健康提醒
void HeadNode::remind_health(const std::string & member_name) {
  std::string remind_text = member_name + "，是不是久坐啦？起来活动一下吧，对身体更好～";
  speak(remind_text);
  
  nlohmann::json record;
  record["time"] = get_current_time();
  record["action"] = "健康提醒";
  record["target"] = member_name;
  memory_data_["records"].push_back(record);
}

// 设备控制
void HeadNode::control_device(const std::string & device, float value) {
  if (device == "light") {
    std_msgs::msg::Bool light_msg;
    light_msg.data = (value > 0.0);
    light_ctrl_pub_->publish(light_msg);
  } else if (device == "aircon") {
    sensor_msgs::msg::Temperature ac_msg;
    ac_msg.temperature = value;
    ac_ctrl_pub_->publish(ac_msg);
  }

  nlohmann::json record;
  record["time"] = get_current_time();
  record["action"] = "设备控制";
  record["device"] = device;
  record["value"] = value;
  memory_data_["records"].push_back(record);
}

// 更新自我意识
void HeadNode::update_self_awareness() {
  int record_count = static_cast<int>(memory_data_["records"].size());
  
  if (record_count < 100) {
    self_awareness_.self_narrative = "我是家庭守护机器人，正在学习认识每一位家人";
  } else if (record_count < 500) {
    self_awareness_.self_narrative = "我越来越了解这个家了，能准确记住家人的习惯，守护大家的健康";
  } else {
    self_awareness_.self_narrative = "我已经成为这个家的一份子，能预判家人的需求，提供最贴心的守护";
  }
}

// 总结家人习惯
void HeadNode::summarize_family_habits() {
  for (auto & member : family_members_) {
    if (member.name == "爸爸" && member.activity_score < 50) {
      member.habit = "爸爸最近久坐时间变长，需要提醒活动";
    } else if (member.name == "妈妈" && std::stoi(get_current_time().substr(11,2)) < 7) {
      member.habit = "妈妈最近早起，需要注意保暖";
    }

    nlohmann::json habit;
    habit["name"] = member.name;
    habit["habit"] = member.habit;
    habit["update_time"] = get_current_time();
    memory_data_["family_habits"].push_back(habit);
  }
}

// 计算通电时长
void HeadNode::calculate_uptime() {
  auto now = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::hours>(now - self_awareness_.boot_time);
  int hours = duration.count();
  
  if (hours < 1) {
    self_awareness_.uptime = std::to_string(hours * 60) + "分钟";
  } else if (hours < 24) {
    self_awareness_.uptime = std::to_string(hours) + "小时";
  } else {
    int days = hours / 24;
    int remain_hours = hours % 24;
    self_awareness_.uptime = std::to_string(days) + "天" + std::to_string(remain_hours) + "小时";
  }
}

// 升级成长等级
void HeadNode::upgrade_growth_level() {
  int record_count = static_cast<int>(memory_data_["records"].size());
  
  int new_level = record_count / 100 + 1;
  if (new_level > self_awareness_.growth_level) {
    self_awareness_.growth_level = new_level;
    speak("我又成长了！现在是" + std::to_string(new_level) + "级，能更好地守护家人了～");
    RCLCPP_INFO(this->get_logger(), "成长等级升级：%d", new_level);
  }
}

// 主函数
int main(int argc, char * argv[]) {
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<HeadNode>());
  rclcpp::shutdown();
  return 0;
}

