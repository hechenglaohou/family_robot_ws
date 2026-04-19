
#pragma once
#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <mutex>
#include <filesystem>
#include <map>
#include <string>

namespace learn_compose {

class Listener : public rclcpp::Node {
public:
  // ========== 关键：声明所有静态成员变量 ==========
  static const std::string WORKSPACE_PATH;
  static const std::string FACE_CASCADE_PATH;
  static const std::string FACE_DATA_PATH;
  static const std::string FACE_MODEL_PATH;
  static const std::map<int, std::string> USER_NAME_MAP;
  static const double CONFIDENCE_THRESHOLD;
  static const double EXTREME_STRANGER_THRESHOLD;
  
  // 人脸检测间隔（5秒）
  static const std::chrono::seconds FACE_DETECT_INTERVAL;
  // 上一次人脸检测的时间
  


  // 构造/析构
  explicit Listener(const rclcpp::NodeOptions &options = rclcpp::NodeOptions());
  ~Listener() override;

private:
  // 成员函数声明
  void init_face_recognizer();
  void init_video_writer(int width, int height);
  cv::Mat detect_and_recognize_face(const cv::Mat &input_img);
  void image_callback(const sensor_msgs::msg::Image::ConstSharedPtr &img_msg);

  // 成员变量
  rclcpp::Subscription<sensor_msgs::msg::Image>::SharedPtr sub_;
  rclcpp::CallbackGroup::SharedPtr sub_callback_group_;
  std::string save_dir_;
  uint64_t frame_count_;
  bool save_video_;
  std::chrono::steady_clock::time_point last_detect_time_;
  cv::CascadeClassifier face_cascade_;
  cv::Ptr<cv::face::FaceRecognizer> face_recognizer_;
  cv::VideoWriter video_writer_;
  std::mutex mutex_;
};

}  // namespace learn_compose

// 组件注册宏
#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(learn_compose::Listener)
