cmake_minimum_required(VERSION 3.5)
project(slam_interfaces)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

if(ROS_EDITION STREQUAL "ROS1")
  find_package(catkin REQUIRED COMPONENTS
    message_generation
    message_runtime
    std_msgs
  )

  add_message_files(
    DIRECTORY msg
    FILES
    VehiclePose.msg
    InsPvax.msg
  )

  add_service_files(
    FILES
    SlamMode.srv
  )

  generate_messages(
    DEPENDENCIES
    std_msgs
  )

  catkin_package()

  include_directories(
    ${catkin_INCLUDE_DIRS}
  )

else(ROS_EDITION STREQUAL "ROS2")
  # find dependencies
  find_package(ament_cmake REQUIRED)
  find_package(std_msgs REQUIRED)
  find_package(rosidl_default_generators REQUIRED)

  if(BUILD_TESTING)
    find_package(ament_lint_auto REQUIRED)
    set(ament_cmake_copyright_FOUND TRUE)
    set(ament_cmake_cpplint_FOUND TRUE)
    ament_lint_auto_find_test_dependencies()
  endif()

  rosidl_generate_interfaces(${PROJECT_NAME}
    "srv/SlamMode.srv"
    "msg/VehiclePose.msg"
    "msg/InsPvax.msg"
    DEPENDENCIES builtin_interfaces std_msgs
  )
  ament_package()
endif()
