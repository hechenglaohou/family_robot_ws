
#ifndef LEARN_COMPOSE_MOTION_DETECTOR_HPP_
#define LEARN_COMPOSE_MOTION_DETECTOR_HPP_

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <mutex>
#include <string>
#include <chrono>
// 新增：std_msgs/String 头文件
#include "std_msgs/msg/string.hpp"

// 定义运动检测面积阈值（可根据实际场景调整）
#define MOTION_AREA_THRESHOLD 500
// 修复：修改为你的实际工作空间路径（去掉c_str()调用，直接用C风格字符串）
#define WORKSPACE_PATH "/home/hhc/sy/video_face_move/learn_compose"

namespace learn_compose {

class MotionDetector : public rclcpp::Node
{
public:
  explicit MotionDetector(const rclcpp::NodeOptions &options = rclcpp::NodeOptions());
  ~MotionDetector() override;

private:
  // 初始化保存目录
  void init_save_dir();
  // 运动检测核心逻辑
  bool detect_motion(const cv::Mat &current_frame);
  // 保存运动图片
  void save_motion_image(const cv::Mat &img, uint64_t frame_id);
  // 图像回调函数
  void image_callback(const sensor_msgs::msg::Image::ConstSharedPtr &img_msg);
  
  // 新增成员函数
  void check_no_motion_alert();
  void play_alert_sound();
  void publish_wechat_message();

  // 原有成员变量
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;
  rclcpp::CallbackGroup::SharedPtr sub_callback_group_;
  uint64_t frame_count_;
  std::string save_dir_;
  cv::Mat prev_gray_frame_;
  std::mutex mutex_;
  
  // 新增成员变量（关键：确保wechat_pub_正确声明）
  std::chrono::system_clock::time_point last_motion_time_;
  rclcpp::TimerBase::SharedPtr timer_;
  // 正确声明wechat_pub_
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr wechat_pub_;
  bool alert_triggered_;
};

}  // namespace learn_compose

#endif  // LEARN_COMPOSE_MOTION_DETECTOR_HPP_
