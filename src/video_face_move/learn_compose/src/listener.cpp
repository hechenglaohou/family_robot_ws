
#include "learn_compose/listener.hpp"
#include <cstdint>
#include <chrono>
#include <filesystem>
#include <sys/stat.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/face.hpp>
#include <map>
#include <vector>

namespace learn_compose {
// ========== 关键：静态成员变量初始化（绑定到Listener类） ==========
const std::string Listener::WORKSPACE_PATH = "/opt/2026_product/family_robot_ws/src/video_face_move";
const std::string Listener::FACE_CASCADE_PATH = Listener::WORKSPACE_PATH + "/config/haarcascade_frontalface_default.xml";
const std::string Listener::FACE_DATA_PATH = Listener::WORKSPACE_PATH + "/face_data";
const std::string Listener::FACE_MODEL_PATH = Listener::WORKSPACE_PATH + "/face_model.yml";
const std::map<int, std::string> Listener::USER_NAME_MAP = {
    {1, "laohou"},
    {2, "pan"},
    {3, "kid"}
};
const double Listener::CONFIDENCE_THRESHOLD = 80.0;
const double Listener::EXTREME_STRANGER_THRESHOLD = 95.0;
// 新增：人脸检测间隔（53秒）
const std::chrono::seconds Listener::FACE_DETECT_INTERVAL = std::chrono::seconds(53);

// 构造函数（新增：初始化上一次检测时间）
Listener::Listener(const rclcpp::NodeOptions &options)
: Node("listener", options),
  save_dir_(WORKSPACE_PATH + "/camera_data"),
  frame_count_(0),
  save_video_(false),
  last_detect_time_(std::chrono::steady_clock::now() - FACE_DETECT_INTERVAL)  // 初始值设为提前5秒，确保第一帧能检测
{
  // 1. 初始化人脸检测器
  if (!face_cascade_.load(FACE_CASCADE_PATH)) {
    RCLCPP_ERROR(this->get_logger(), "人脸检测器加载失败！路径: %s", FACE_CASCADE_PATH.c_str());
  } else {
    RCLCPP_INFO(this->get_logger(), "人脸检测器加载成功 ✅");
  }

  // 2. 初始化人脸识别器
  init_face_recognizer();

  // 3. 打印路径+检查权限
  RCLCPP_INFO(this->get_logger(), "=== 初始化保存目录 ===");
  RCLCPP_INFO(this->get_logger(), "工作空间路径: %s", WORKSPACE_PATH.c_str());
  RCLCPP_INFO(this->get_logger(), "保存目录: %s", save_dir_.c_str());
  RCLCPP_INFO(this->get_logger(), "人脸数据路径: %s", FACE_DATA_PATH.c_str());
  RCLCPP_INFO(this->get_logger(), "人脸模型路径: %s", FACE_MODEL_PATH.c_str());
  RCLCPP_INFO(this->get_logger(), "人脸检测间隔: %ld 秒", FACE_DETECT_INTERVAL.count());
  
  if (!std::filesystem::exists(WORKSPACE_PATH)) {
    RCLCPP_ERROR(this->get_logger(), "工作空间路径不存在！%s", WORKSPACE_PATH.c_str());
  } else {
    if (access(WORKSPACE_PATH.c_str(), W_OK) != 0) {
      RCLCPP_ERROR(this->get_logger(), "工作空间无写入权限！%s", WORKSPACE_PATH.c_str());
    } else {
      RCLCPP_INFO(this->get_logger(), "工作空间路径存在且有写入权限 ✅");
    }
  }

  try {
    if (std::filesystem::create_directory(save_dir_)) {
      RCLCPP_INFO(this->get_logger(), "保存目录创建成功: %s ✅", save_dir_.c_str());
    } else {
      if (std::filesystem::exists(save_dir_)) {
        RCLCPP_INFO(this->get_logger(), "保存目录已存在: %s ✅", save_dir_.c_str());
      } else {
        RCLCPP_ERROR(this->get_logger(), "保存目录创建失败！%s", save_dir_.c_str());
      }
    }
    if (std::filesystem::create_directory(FACE_DATA_PATH)) {
      RCLCPP_INFO(this->get_logger(), "人脸数据目录创建成功: %s ✅", FACE_DATA_PATH.c_str());
    }
  } catch (const std::exception &e) {
    RCLCPP_ERROR(this->get_logger(), "创建目录时异常: %s", e.what());
  }

  // 4. 多线程回调组
  sub_callback_group_ = this->create_callback_group(rclcpp::CallbackGroupType::MutuallyExclusive);

  // 5. 配置QoS
  rclcpp::QoS qos(10);
  qos.reliable()
     .durability_volatile()
     .keep_last(10);

  // 6. 创建订阅者
  RCLCPP_INFO(this->get_logger(), "=== 订阅 /camera/image_raw 话题 ===");
  rclcpp::SubscriptionOptions sub_opt;
  sub_opt.callback_group = sub_callback_group_;
  sub_ = this->create_subscription<sensor_msgs::msg::Image>(
      "/camera/image_raw", qos,
      std::bind(&Listener::image_callback, this, std::placeholders::_1),
      sub_opt);
  RCLCPP_INFO(this->get_logger(), "订阅者创建成功 ✅，等待接收消息...");
}

// 析构函数
Listener::~Listener()
{
  if (video_writer_.isOpened()) {
    video_writer_.release();
    RCLCPP_INFO(this->get_logger(), "视频写入器已释放");
  }
  RCLCPP_INFO(this->get_logger(), "Listener节点退出");
}

// 初始化人脸识别器
void Listener::init_face_recognizer()
{
  face_recognizer_ = cv::face::LBPHFaceRecognizer::create();
  
  if (std::filesystem::exists(FACE_MODEL_PATH)) {
    try {
      face_recognizer_->read(FACE_MODEL_PATH);
      RCLCPP_INFO(this->get_logger(), "人脸识别模型加载成功 ✅");
    } catch (const std::exception &e) {
      RCLCPP_ERROR(this->get_logger(), "人脸识别模型加载失败: %s", e.what());
    }
  } else {
    RCLCPP_WARN(this->get_logger(), "未找到人脸识别模型: %s，请先训练模型", FACE_MODEL_PATH.c_str());
  }
}

// 初始化视频写入器
void Listener::init_video_writer(int width, int height)
{
  std::lock_guard<std::mutex> lock(mutex_);
  if (video_writer_.isOpened()) return;
  std::string video_path = save_dir_ + "/camera_recording_with_face_recognize.mp4";
  RCLCPP_INFO(this->get_logger(), "=== 初始化视频写入器 ===");
  RCLCPP_INFO(this->get_logger(), "视频路径: %s", video_path.c_str());
  RCLCPP_INFO(this->get_logger(), "视频参数: %dx%d, 30 FPS, MP4编码", width, height);
  int fourcc = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
  double fps = 30.0;
  video_writer_.open(video_path, fourcc, fps, cv::Size(width, height), true);
  if (!video_writer_.isOpened()) {
    RCLCPP_ERROR(this->get_logger(), "视频写入器初始化失败！原因可能：");
    RCLCPP_ERROR(this->get_logger(), "1. 路径无写入权限: %s", video_path.c_str());
    RCLCPP_ERROR(this->get_logger(), "2. OpenCV不支持MP4编码（换AVI试试）");
    RCLCPP_ERROR(this->get_logger(), "3. 尺寸错误: %dx%d", width, height);
    save_video_ = false;
  } else {
    RCLCPP_INFO(this->get_logger(), "视频写入器初始化成功 ✅");
  }
}

// 人脸检测+识别核心函数
cv::Mat Listener::detect_and_recognize_face(const cv::Mat &input_img)
{
  cv::Mat frame = input_img.clone();
  if (frame.empty()) {
    RCLCPP_WARN(this->get_logger(), "输入图像为空，跳过人脸检测");
    return frame;
  }
  if (face_cascade_.empty()) {
    RCLCPP_WARN(this->get_logger(), "人脸检测器未加载，跳过人脸检测");
    return frame;
  }

  cv::Mat gray;
  cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
  cv::equalizeHist(gray, gray);
  std::vector<cv::Rect> faces;
  face_cascade_.detectMultiScale(gray, faces, 1.1, 3, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
  RCLCPP_INFO(this->get_logger(), "检测到 %lu 个人脸", faces.size());

  if (face_recognizer_.empty()) {
    for (size_t i = 0; i < faces.size(); i++) {
      cv::Point center(faces[i].x + faces[i].width/2, faces[i].y + faces[i].height/2);
      cv::ellipse(frame, center, cv::Size(faces[i].width/2, faces[i].height/2), 0, 0, 360, cv::Scalar(255, 0, 255), 4);
      cv::putText(frame, "Unknown Face " + std::to_string(i+1), 
                  cv::Point(faces[i].x, faces[i].y - 10), 
                  cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(0, 0, 255), 2);
    }
    return frame;
  }

  for (size_t i = 0; i < faces.size(); i++) {
    cv::Rect face_rect = faces[i];
    cv::Mat face_roi = gray(face_rect);
    
    int user_id = -1;
    double confidence = 0.0;
    face_recognizer_->predict(face_roi, user_id, confidence);
    std::string label;
    cv::Scalar color;

    if (confidence < CONFIDENCE_THRESHOLD) {
      auto it = USER_NAME_MAP.find(user_id);
      std::string user_name = (it != USER_NAME_MAP.end()) ? it->second : "unknown_" + std::to_string(user_id);
      label = user_name + " (" + std::to_string(static_cast<int>(100 - confidence)) + "%)";
      color = cv::Scalar(0, 255, 0);
      RCLCPP_INFO(this->get_logger(), "识别结果: %s, 置信度: %.2f", label.c_str(), confidence);
    } else {
      if (confidence > EXTREME_STRANGER_THRESHOLD) {
        RCLCPP_DEBUG(this->get_logger(), "过滤极端Stranger：置信度%.2f（阈值95）", confidence);
        continue;
      }
      label = "Stranger (" + std::to_string(static_cast<int>(100 - confidence)) + "%)";
      color = cv::Scalar(0, 0, 255);
      RCLCPP_INFO(this->get_logger(), "识别结果: %s, 置信度: %.2f", label.c_str(), confidence);
    }

    cv::Point center(face_rect.x + face_rect.width/2, face_rect.y + face_rect.height/2);
    cv::ellipse(frame, center, cv::Size(face_rect.width/2, face_rect.height/2), 0, 0, 360, color, 4);
    cv::putText(frame, label, 
                cv::Point(face_rect.x, face_rect.y - 10), 
                cv::FONT_HERSHEY_SIMPLEX, 0.8, color, 2);
  }
  return frame;
}

// 回调函数
void Listener::image_callback(const sensor_msgs::msg::Image::ConstSharedPtr &img_msg)
{
  std::lock_guard<std::mutex> lock(mutex_);
  RCLCPP_INFO(this->get_logger(), "\n=== 收到第 %lu 帧消息 ===", frame_count_);
  RCLCPP_INFO(this->get_logger(), "消息时间戳: (%u, %u)",
              img_msg->header.stamp.sec, img_msg->header.stamp.nanosec);
  RCLCPP_INFO(this->get_logger(), "消息地址: 0x%lX",
              reinterpret_cast<std::uintptr_t>(img_msg.get()));
  RCLCPP_INFO(this->get_logger(), "图像尺寸: %dx%d", img_msg->width, img_msg->height);
  
  try {
    cv_bridge::CvImageConstPtr cv_img = cv_bridge::toCvShare(img_msg, "bgr8");
    RCLCPP_INFO(this->get_logger(), "图像数据地址: 0x%lX",
                reinterpret_cast<std::uintptr_t>(cv_img->image.data));
    
    cv::Mat frame_with_recognize;
    // ========== 核心修改：5秒检测间隔逻辑 ==========
    auto now = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = now - last_detect_time_;
    
    if (elapsed >= FACE_DETECT_INTERVAL) {
      // 达到检测间隔，执行人脸检测和识别
      frame_with_recognize = detect_and_recognize_face(cv_img->image);
      last_detect_time_ = now;  // 更新最后检测时间
      RCLCPP_INFO(this->get_logger(), "执行人脸检测（间隔%.1f秒）", elapsed.count());
    } else {
      // 未达到检测间隔，直接使用原始图像
      frame_with_recognize = cv_img->image.clone();
      RCLCPP_INFO(this->get_logger(), "跳过人脸检测（距离上次检测%.1f秒，需满%ld秒）", 
                  elapsed.count(), FACE_DETECT_INTERVAL.count());
    }
    
    // 视频/图片保存逻辑保持不变
    if (save_video_) {
      if (!video_writer_.isOpened()) {
        init_video_writer(frame_with_recognize.cols, frame_with_recognize.rows);
      }
      if (video_writer_.isOpened()) {
        video_writer_.write(frame_with_recognize);
        RCLCPP_INFO(this->get_logger(), "第 %lu 帧（含人脸识别）写入视频 ✅", frame_count_);
      } else {
        RCLCPP_WARN(this->get_logger(), "视频写入器未初始化，切换为保存图片");
        save_video_ = false;
      }
    }
    if (!save_video_) {
      std::string img_path = save_dir_ + "/frame_" + std::to_string(frame_count_) + "_with_face_recognize.jpg";
      // 补充：实际执行图片保存（原代码缺少imwrite调用）
      // if (cv::imwrite(img_path, frame_with_recognize)) {
      //   RCLCPP_INFO(this->get_logger(), "保存带人脸识别的图片到: %s ✅", img_path.c_str());
      // } else {
      //   RCLCPP_ERROR(this->get_logger(), "保存图片失败: %s", img_path.c_str());
      // }
    }
    frame_count_++;
  } catch (const cv_bridge::Exception &e) {
    RCLCPP_ERROR(this->get_logger(), "图像转换失败: %s", e.what());
  } catch (const std::exception &e) {
    RCLCPP_ERROR(this->get_logger(), "保存数据时异常: %s", e.what());
  }
}
}  // namespace learn_compose

// 需要在listener.hpp头文件中添加以下成员变量声明：
// // 人脸检测间隔（5秒）
// static const std::chrono::seconds FACE_DETECT_INTERVAL;
// // 上一次人脸检测的时间
// std::chrono::steady_clock::time_point last_detect_time_;

// main函数
int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<learn_compose::Listener>();
  rclcpp::executors::MultiThreadedExecutor executor(rclcpp::ExecutorOptions(), 2);
  executor.add_node(node);
  executor.spin();
  executor.remove_node(node);
  rclcpp::shutdown();
  return 0;
}

// 组件注册
#include <rclcpp_components/register_node_macro.hpp>
RCLCPP_COMPONENTS_REGISTER_NODE(learn_compose::Listener)

