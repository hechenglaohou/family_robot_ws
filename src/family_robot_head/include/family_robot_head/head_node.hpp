#ifndef FAMILY_ROBOT_HEAD_HEAD_NODE_HPP_
#define FAMILY_ROBOT_HEAD_HEAD_NODE_HPP_

// ROS 核心依赖
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/bool.hpp"
#include "sensor_msgs/msg/temperature.hpp"

// 标准库依赖
#include <vector>
#include <string>
#include <chrono>
#include <nlohmann/json.hpp>  // 新增：包含 json 库头文件

// 只在头文件中定义结构体
struct FamilyMember {
  std::string name;
  int age;
  std::string habit;
  std::string last_seen;
  double activity_score;
  bool is_abnormal;
};

struct SelfAwareness {
  std::string core_mission;
  int growth_level;
  std::chrono::system_clock::time_point boot_time;
  std::string self_narrative;
  std::string uptime;
};

class HeadNode : public rclcpp::Node {
public:
  HeadNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());
  ~HeadNode();

private:
  // 发布者
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr speech_pub_;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr light_ctrl_pub_;
  rclcpp::Publisher<sensor_msgs::msg::Temperature>::SharedPtr ac_ctrl_pub_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr growth_pub_;

  // 订阅者
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr voice_sub_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr status_sub_;

  // 定时器
  rclcpp::TimerBase::SharedPtr env_check_timer_;
  rclcpp::TimerBase::SharedPtr growth_timer_;

  // 数据成员
  std::vector<FamilyMember> family_members_;
  SelfAwareness self_awareness_;
  nlohmann::json memory_data_;  // 现在能正确识别

  // 核心方法声明
  void init_family_members();
  void init_self_awareness();
  void load_memory();
  void save_memory();
  void env_check_callback();
  void growth_callback();
  void voice_callback(const std_msgs::msg::String::SharedPtr msg);
  void status_callback(const std_msgs::msg::String::SharedPtr msg);
  void check_abnormal_status(const std::string & status_str);
  std::string get_current_time();
  void speak(const std::string & text);
  void control_device(const std::string & device, float value);
  void update_self_awareness();
  void summarize_family_habits();
  void calculate_uptime();
  void upgrade_growth_level();
  void remind_health(const std::string & member_name);
};

#endif  // FAMILY_ROBOT_HEAD_HEAD_NODE_HPP_

