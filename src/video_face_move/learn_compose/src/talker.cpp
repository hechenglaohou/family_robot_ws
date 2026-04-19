#include "learn_compose/talker.hpp"
#include <memory>
#include <cstdint>
#include <opencv2/core/core.hpp>

namespace learn_compose {

using namespace std::chrono_literals;

// 构造函数：关闭进程内通信，配置DDS共享内存的QoS
Talker::Talker(const rclcpp::NodeOptions & options)
: Node("camera_publisher", options),  // 移除use_intra_process_comms(true)
  cv_img_(std::make_shared<cv_bridge::CvImage>())
{
  // ========== 核心：配置DDS共享内存的QoS（和Listener完全匹配） ==========
  rclcpp::QoS qos(10);
  qos.reliable()          // 可靠传输（共享内存推荐）
     .durability_volatile() // 非持久化（实时摄像头数据）
     .keep_last(10);       // 保留最后10帧

  // 创建发布者（绑定QoS）
  pub_ = this->create_publisher<sensor_msgs::msg::Image>("/camera/image_raw", qos);

  // 摄像头初始化（不变）
  cap_.open(0, cv::CAP_V4L2);
  if (!cap_.isOpened()) {
    RCLCPP_FATAL(this->get_logger(), "无法打开摄像头！");
    throw std::runtime_error("摄像头初始化失败");
  }
  cap_.set(cv::CAP_PROP_FRAME_WIDTH, 640);
  cap_.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
  cap_.set(cv::CAP_PROP_BUFFERSIZE, 1);

  // 定时发布（7s≈1帧/7秒）
  timer_ = this->create_wall_timer(7000ms, std::bind(&Talker::timer_callback, this));
  RCLCPP_INFO(this->get_logger(), "摄像头发布节点启动（DDS共享内存模式）...");
}

Talker::~Talker()
{
  if (cap_.isOpened()) {
    cap_.release();
    RCLCPP_INFO(this->get_logger(), "摄像头资源已释放");
  }
}

void Talker::timer_callback()
{
  cv::Mat frame;
  bool ret = cap_.read(frame);
  if (!ret || frame.empty()) {
    RCLCPP_WARN(this->get_logger(), "读取摄像头帧失败！");
    return;
  }

  // 填充图像消息（不变）
  cv_img_->image = frame;
  cv_img_->encoding = "bgr8";
  cv_img_->header.stamp = this->now();
  cv_img_->header.frame_id = "camera_link";

  // 发布消息（DDS共享内存传输）
  sensor_msgs::msg::Image::UniquePtr img_msg = 
      std::make_unique<sensor_msgs::msg::Image>(*cv_img_->toImageMsg());

  try {
    pub_->publish(std::move(img_msg));
    // 打印发布信息（验证）
    RCLCPP_INFO(this->get_logger(), 
                "[发布端] 帧尺寸: %dx%d | 时间戳: (%u, %u) | 图像数据地址: 0x%lX",
                frame.cols, frame.rows,
                cv_img_->header.stamp.sec, cv_img_->header.stamp.nanosec,
                reinterpret_cast<std::uintptr_t>(frame.data));
  } catch (cv_bridge::Exception & e) {
    RCLCPP_ERROR(this->get_logger(), "图像转换失败：%s", e.what());
  }
}

}  // namespace learn_compose

#include <rclcpp_components/register_node_macro.hpp>


// ========== 新增：Talker 独立可执行文件的 main 函数 ==========
int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<learn_compose::Talker>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}

// 保留原有组件注册
#include <rclcpp_components/register_node_macro.hpp>
// #include "rclcpp_components/register_node_macro.hpp"
RCLCPP_COMPONENTS_REGISTER_NODE(learn_compose::Talker)





