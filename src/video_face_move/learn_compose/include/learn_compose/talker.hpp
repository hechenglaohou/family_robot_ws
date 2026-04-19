#ifndef LEARN_COMPOSE_TALKER_HPP_
#define LEARN_COMPOSE_TALKER_HPP_

#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/image.hpp>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/videoio/videoio.hpp>
#include <memory>

namespace learn_compose {

class Talker : public rclcpp::Node {
public:
  explicit Talker(const rclcpp::NodeOptions &options = rclcpp::NodeOptions());
  ~Talker() override;

private:
  void timer_callback();

  rclcpp::Publisher<sensor_msgs::msg::Image>::SharedPtr pub_;
  rclcpp::TimerBase::SharedPtr timer_;
  cv::VideoCapture cap_;
  std::shared_ptr<cv_bridge::CvImage> cv_img_;
};

}  // namespace learn_compose

#endif  // LEARN_COMPOSE_TALKER_HPP_
