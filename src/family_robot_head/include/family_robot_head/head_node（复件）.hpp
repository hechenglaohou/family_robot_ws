#ifndef FAMILY_ROBOT_HEAD__HEAD_NODE_HPP_
#define FAMILY_ROBOT_HEAD__HEAD_NODE_HPP_

#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <std_msgs/msg/bool.hpp>
#include <std_msgs/msg/float32.hpp>
#include <sensor_msgs/msg/temperature.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <chrono>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <mutex>

// 记忆存储路径（统一在头文件定义）
#define MEMORY_PATH "/home/robot/family_memory.json"

// 家庭成员结构体
struct FamilyMember {
  std::string name;
  int age;
  std::string habit;          
  std::string last_seen;      
  float activity_score;       
  bool is_abnormal;           
};

// 机器人自我认知结构体
struct SelfAwareness {
  std::string core_mission;   
  std::string uptime;         
  int growth_level;           
  std::string self_narrative; 
  std::chrono::system_clock::time_point boot_time;
};

class HeadNode : public rclcpp::Node {
public:
  explicit HeadNode(const rclcpp::NodeOptions & options = rclcpp::NodeOptions());
  ~HeadNode();

private:
  // 核心定时器
  rclcpp::TimerBase::SharedPtr env_check_timer_;
  rclcpp::TimerBase::SharedPtr growth_timer_;

  // 发布者
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr speech_pub_;
  rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr light_ctrl_pub_;
  rclcpp::Publisher<sensor_msgs::msg::Temperature>::SharedPtr ac_ctrl_pub_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr growth_pub_;

  // 订阅者
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr voice_sub_;
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr image_sub_;

  // 图像相关（线程安全）
  cv::Mat latest_frame_;
  std::mutex frame_mutex_;

  // 核心数据
  std::vector<FamilyMember> family_members_;
  SelfAwareness self_awareness_;
  nlohmann::json memory_data_;

  // 初始化函数
  void init_family_members();
  void init_self_awareness();
  void load_memory();
  void save_memory();

  // 核心业务逻辑
  void env_check_callback();
  void growth_callback();
  void voice_callback(const std_msgs::msg::String::SharedPtr msg);
  void image_callback(const sensor_msgs::msg::Image::SharedPtr msg);
  
  // 感知相关
  cv::Mat get_latest_frame();
  void detect_family_status(const cv::Mat & frame);
  void check_abnormal_status();
  std::string get_current_time();

  // 交互相关
  void speak(const std::string & text);
  void remind_health(const std::string & member_name);
  void control_device(const std::string & device, float value);

  // 自我成长相关
  void update_self_awareness();
  void summarize_family_habits();
  void calculate_uptime();
  void upgrade_growth_level();

  // 新增：兼容旧代码的共享内存读取函数声明（逐步替换用）
  cv::Mat read_camera_from_shm();
};

#endif  // FAMILY_ROBOT_HEAD__HEAD_NODE_HPP_

