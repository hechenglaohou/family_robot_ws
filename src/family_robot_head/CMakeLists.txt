cmake_minimum_required(VERSION 3.8)
project(family_robot_head)

# 设置 C++ 标准
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 查找依赖包
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(nlohmann_json REQUIRED)  # 新增：查找 json 库

# 包含头文件目录
include_directories(
  include
  ${nlohmann_json_INCLUDE_DIRS}  # 新增：添加 json 库头文件目录
)

# 构建可执行文件
add_executable(head_node src/head_node.cpp)

# 设置编译依赖
ament_target_dependencies(
  head_node
  rclcpp
  std_msgs
  sensor_msgs
  nlohmann_json  # 新增：链接 json 库
)

# 安装可执行文件
install(TARGETS
  head_node
  DESTINATION lib/${PROJECT_NAME}
)

# 安装头文件
install(DIRECTORY include/
  DESTINATION include/
)

# 测试（可选）
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

