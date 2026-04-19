cmake_minimum_required(VERSION 3.8)
project(learn_compose)

# 保留原有编译选项（避免报错）
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \
  -Wno-error=placement-new \
  -Wno-error=deprecated-declarations \
  -Wno-error=nonnull \
  -Wno-error=invalid-offsetof \
  -Wno-error=class-memaccess \
  -Wno-error=deprecated-copy \
  -Wno-error=unused-parameter \
  -Wno-error=pointer-arith \
  -fPIC")  # 动态库必需
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 依赖包：包含人脸识别+运动检测所有必需组件
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(rclcpp_components REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED COMPONENTS core videoio imgcodecs imgproc highgui objdetect face)  

# ========== 适配根目录 config（核心） ==========
set(WORKSPACE_PATH "/home/hhc/family_robot_ws/src/video_face_move")
set(CONFIG_SOURCE_PATH "${WORKSPACE_PATH}/config")
# 安装根目录 config 到 ROS2 安装目录
if(EXISTS ${CONFIG_SOURCE_PATH})
  install(DIRECTORY ${CONFIG_SOURCE_PATH}/
          DESTINATION share/${PROJECT_NAME}/config
          FILES_MATCHING PATTERN "*.xml")
  message(STATUS "Config目录找到：${CONFIG_SOURCE_PATH}，已加入安装规则")
else()
  message(WARNING "根目录Config未找到：${CONFIG_SOURCE_PATH}，运行时会自动创建")
endif()

# 包含头文件目录
include_directories(
  include
  include/learn_compose
  ${OpenCV_INCLUDE_DIRS}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# ============== 1. Talker节点 ==============
add_library(talker_component SHARED src/talker.cpp)
ament_target_dependencies(talker_component 
  std_msgs 
  sensor_msgs
  rclcpp 
  rclcpp_components
  cv_bridge
)
target_link_libraries(talker_component ${OpenCV_LIBS})
rclcpp_components_register_nodes(talker_component "learn_compose::Talker")

# 可执行文件（修正错误命令）
add_executable(talker_node src/talker.cpp)
ament_target_dependencies(talker_node 
  std_msgs 
  sensor_msgs
  rclcpp_components
  rclcpp 
  cv_bridge
)
target_link_libraries(talker_node ${OpenCV_LIBS})

# ============== 2. Listener节点（人脸识别） ==============
add_library(listener_component SHARED src/listener.cpp)
ament_target_dependencies(listener_component 
  std_msgs 
  sensor_msgs
  rclcpp 
  rclcpp_components
  cv_bridge
)
target_link_libraries(listener_component ${OpenCV_LIBS})
rclcpp_components_register_nodes(listener_component "learn_compose::Listener")

# 可执行文件（修正错误命令）
add_executable(listener_node src/listener.cpp)
ament_target_dependencies(listener_node 
  std_msgs 
  sensor_msgs
  rclcpp_components
  rclcpp 
  cv_bridge
)
target_link_libraries(listener_node ${OpenCV_LIBS})

# ============== 3. MotionDetector节点 ==============
add_library(motion_detector_component SHARED src/motion_detector.cpp)
ament_target_dependencies(motion_detector_component 
  std_msgs 
  sensor_msgs
  rclcpp 
  rclcpp_components
  cv_bridge
)
target_link_libraries(motion_detector_component ${OpenCV_LIBS})
rclcpp_components_register_nodes(motion_detector_component "learn_compose::MotionDetector")

# 可执行文件（修正错误命令）
add_executable(motion_detector_node src/motion_detector.cpp)
ament_target_dependencies(motion_detector_node 
  std_msgs 
  sensor_msgs
  rclcpp_components
  rclcpp 
  cv_bridge
)
target_link_libraries(motion_detector_node ${OpenCV_LIBS})

# ============== 安装配置 ==============
# 安装可执行文件
install(TARGETS talker_node listener_node motion_detector_node
  DESTINATION lib/${PROJECT_NAME})

# 安装组件库
install(TARGETS talker_component listener_component motion_detector_component
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
)

# 安装头文件
install(DIRECTORY include/
  DESTINATION include/${PROJECT_NAME}
)

# 测试配置
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

# 打印编译信息（调试用）
message(STATUS "项目编译路径: ${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "安装路径: ${CMAKE_INSTALL_PREFIX}")

