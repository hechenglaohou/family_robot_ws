

#include "learn_compose/motion_detector.hpp"
#include <cstdint>
#include <chrono>
#include <filesystem>
#include <sys/stat.h>
#include <unistd.h>
#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>
// 新增：音频播放相关头文件
#include <cstdlib>

namespace learn_compose {
// 新增：定义45分钟的时间常量（单位：秒）
constexpr uint64_t NO_MOTION_ALERT_DURATION = 45* 60;

// 构造函数：初始化节点、订阅者、QoS、保存目录
MotionDetector::MotionDetector(const rclcpp::NodeOptions &options)
: Node("motion_detector_node", options),
  frame_count_(0),
  save_dir_(std::string(WORKSPACE_PATH) + "/motion_img"),  // 修复：转为std::string拼接
  last_motion_time_(std::chrono::system_clock::now()),
  alert_triggered_(false)
{
  // 初始化保存目录
  init_save_dir();

  // 创建微信消息发布者
  rclcpp::QoS qos_pub(10);
  qos_pub.reliable()
         .durability_volatile()
         .keep_last(10);
  wechat_pub_ = this->create_publisher<std_msgs::msg::String>(
      "/wechat_message", qos_pub);
  RCLCPP_INFO(this->get_logger(), "微信消息发布者创建成功 ✅");

  // 创建定时器（每30秒检查一次是否超时）
  timer_ = this->create_wall_timer(
      std::chrono::seconds(30),
      std::bind(&MotionDetector::check_no_motion_alert, this));
  RCLCPP_INFO(this->get_logger(), "无运动检测定时器创建成功 ✅");

  // 创建多线程回调组（互斥型）
  sub_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

  // 配置QoS
  rclcpp::QoS qos(10);
  qos.reliable()
     .durability_volatile()
     .keep_last(10);

  // 创建订阅者+绑定回调组
  RCLCPP_INFO(this->get_logger(), "=== 订阅 /camera/image_raw 话题（移动物体检测+保存）===");
  rclcpp::SubscriptionOptions sub_opt;
  sub_opt.callback_group = sub_callback_group_;
  sub_ = this->create_subscription<sensor_msgs::msg::Image>(
      "/camera/image_raw", qos,
      std::bind(&MotionDetector::image_callback, this, std::placeholders::_1),
      sub_opt);
  RCLCPP_INFO(this->get_logger(), "订阅者创建成功 ✅，等待接收摄像头帧...");
}

// 析构函数
MotionDetector::~MotionDetector()
{
  RCLCPP_INFO(this->get_logger(), "移动物体检测节点退出");
}

// 初始化保存目录
void MotionDetector::init_save_dir()
{
  RCLCPP_INFO(this->get_logger(), "=== 初始化移动物体图片保存目录 ===");
  // 修复：WORKSPACE_PATH是C风格字符串，直接传入（去掉.c_str()）
  RCLCPP_INFO(this->get_logger(), "工作空间路径: %s", WORKSPACE_PATH);
  RCLCPP_INFO(this->get_logger(), "图片保存目录: %s", save_dir_.c_str());
  
  // 检查工作空间是否存在且有写入权限
  if (!std::filesystem::exists(WORKSPACE_PATH)) {
    // 修复：去掉.c_str()
    RCLCPP_ERROR(this->get_logger(), "工作空间路径不存在！%s", WORKSPACE_PATH);
    return;
  }
  if (access(WORKSPACE_PATH, W_OK) != 0) {
    // 修复：去掉.c_str()
    RCLCPP_ERROR(this->get_logger(), "工作空间无写入权限！%s", WORKSPACE_PATH);
    return;
  }
  
  // 创建保存目录
  try {
    if (std::filesystem::create_directory(save_dir_)) {
      RCLCPP_INFO(this->get_logger(), "保存目录创建成功 ✅: %s", save_dir_.c_str());
    } else if (std::filesystem::exists(save_dir_)) {
      RCLCPP_INFO(this->get_logger(), "保存目录已存在 ✅: %s", save_dir_.c_str());
    } else {
      RCLCPP_ERROR(this->get_logger(), "保存目录创建失败 ❌: %s", save_dir_.c_str());
    }
  } catch (const std::exception &e) {
    RCLCPP_ERROR(this->get_logger(), "创建目录异常: %s", e.what());
  }
}

// 运动检测核心逻辑
bool MotionDetector::detect_motion(const cv::Mat &current_frame)
{
  if (current_frame.empty()) {
    RCLCPP_WARN(this->get_logger(), "输入图像为空，跳过运动检测");
    return false;
  }
  
  cv::Mat current_gray;
  cv::cvtColor(current_frame, current_gray, cv::COLOR_BGR2GRAY);
  cv::GaussianBlur(current_gray, current_gray, cv::Size(5, 5), 0);
  
  if (prev_gray_frame_.empty()) {
    current_gray.copyTo(prev_gray_frame_);
    return false;
  }
  
  cv::Mat frame_diff, thresh;
  cv::absdiff(prev_gray_frame_, current_gray, frame_diff);
  cv::threshold(frame_diff, thresh, 25, 255, cv::THRESH_BINARY);
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
  cv::dilate(thresh, thresh, kernel, cv::Point(-1, -1), 2);
  
  std::vector<std::vector<cv::Point>> contours;
  cv::findContours(thresh, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
  bool has_motion = false;
  for (const auto &contour : contours) {
    if (cv::contourArea(contour) > MOTION_AREA_THRESHOLD) {
      has_motion = true;
      break;
    }
  }
  
  // 检测到运动时更新时间并重置警报标记
  if (has_motion) {
    last_motion_time_ = std::chrono::system_clock::now();
    alert_triggered_ = false;
    RCLCPP_DEBUG(this->get_logger(), "检测到运动，更新最后运动时间");
  }
  
  current_gray.copyTo(prev_gray_frame_);
  return has_motion;
}

// 保存运动图片
void MotionDetector::save_motion_image(const cv::Mat &img, uint64_t frame_id)
{
  if (img.empty()) {
    RCLCPP_WARN(this->get_logger(), "图像为空，无法保存");
    return;
  }
  if (!std::filesystem::exists(save_dir_) || access(save_dir_.c_str(), W_OK) != 0) {
    RCLCPP_ERROR(this->get_logger(), "保存目录无效/无写入权限，无法保存图片");
    return;
  }
  
  uint64_t timestamp = std::chrono::duration_cast<std::chrono::seconds>(
      std::chrono::system_clock::now().time_since_epoch()).count();
  std::string img_path = save_dir_ + "/motion_frame_" + 
                         std::to_string(frame_id) + "_" + 
                         std::to_string(timestamp) + ".jpg";
  
  // if (cv::imwrite(img_path, img)) {
  //   RCLCPP_WARN(this->get_logger(), "✅ 移动物体图片保存成功: %s", img_path.c_str());
  // } else {
  //   RCLCPP_ERROR(this->get_logger(), "❌ 移动物体图片保存失败: %s", img_path.c_str());
  // }
}

// 检查无运动超时并触发警报
void MotionDetector::check_no_motion_alert()
{
  if (alert_triggered_) {
    return;
  }

  auto now = std::chrono::system_clock::now();
  uint64_t duration_since_last_motion = std::chrono::duration_cast<std::chrono::seconds>(
      now - last_motion_time_).count();

  if (duration_since_last_motion >= NO_MOTION_ALERT_DURATION && rclcpp::ok()) {
    RCLCPP_ERROR(this->get_logger(), "⚠️ 连续%lu秒未检测到移动物体，触发警报！", duration_since_last_motion);
    
    play_alert_sound();
    publish_wechat_message();
    alert_triggered_ = true;
  }
}

// 播放警报声
void MotionDetector::play_alert_sound()
{
  try {
    // Linux下音频播放（需安装alsa-utils：sudo apt install alsa-utils）
    int ret = system("aplay /usr/share/sounds/alsa/Front_Center.wav 2>/dev/null");
    if (ret != 0) {
      system("echo -e '\a'");
      RCLCPP_WARN(this->get_logger(), "警报声播放失败，使用蜂鸣器提示");
    } else {
      RCLCPP_INFO(this->get_logger(), "警报声播放成功 ✅");
    }
  } catch (const std::exception &e) {
    RCLCPP_ERROR(this->get_logger(), "播放警报声异常: %s", e.what());
  }
}

// 发布微信消息话题
void MotionDetector::publish_wechat_message()
{
  try {
    std_msgs::msg::String msg;
    msg.data = "连续45分钟无人活动";
    wechat_pub_->publish(msg);  // 现在能正确识别wechat_pub_
    RCLCPP_INFO(this->get_logger(), "✅ 发布微信消息话题: /wechat_message -> %s", msg.data.c_str());
  } catch (const std::exception &e) {
    RCLCPP_ERROR(this->get_logger(), "发布微信消息异常: %s", e.what());
  }
}

// 图像回调函数
void MotionDetector::image_callback(const sensor_msgs::msg::Image::ConstSharedPtr &img_msg)
{
  std::lock_guard<std::mutex> lock(mutex_);
  RCLCPP_INFO(this->get_logger(), "\n=== 收到第 %lu 帧摄像头数据 ===", frame_count_);
  RCLCPP_INFO(this->get_logger(), "图像尺寸: %dx%d | 时间戳: (%u, %u)",
              img_msg->width, img_msg->height,
              img_msg->header.stamp.sec, img_msg->header.stamp.nanosec);
  try {
    cv_bridge::CvImageConstPtr cv_img = cv_bridge::toCvShare(img_msg, "bgr8");
    
    bool motion_detected = detect_motion(cv_img->image);
    if (motion_detected) {
      RCLCPP_WARN(this->get_logger(), "==================== 发现移动物体 ====================");
      save_motion_image(cv_img->image, frame_count_);
    } else {
      RCLCPP_INFO(this->get_logger(), "未检测到移动物体");
    }
    frame_count_++;
  } catch (const cv_bridge::Exception &e) {
    RCLCPP_ERROR(this->get_logger(), "ROS2图像转OpenCV失败: %s", e.what());
  } catch (const std::exception &e) {
    RCLCPP_ERROR(this->get_logger(), "运动检测/保存异常: %s", e.what());
  }
}
}  // namespace learn_compose

// main函数
int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<learn_compose::MotionDetector>();
  rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(), 2);
  executor.add_node(node);
  executor.spin();
  executor.remove_node(node);
  rclcpp::shutdown();
  return 0;
}

// 组件注册
#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(learn_compose::MotionDetector)







