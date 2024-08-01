cmake_minimum_required(VERSION 3.5)
project(slam_service)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(PCL 1.8 REQUIRED)

if(ROS_EDITION STREQUAL "ROS1")
  find_package(catkin REQUIRED COMPONENTS
    roscpp
    rospy
    std_msgs
    message_runtime
    slam_interfaces
  )

  include_directories(
    ${catkin_INCLUDE_DIRS}
    ${PCL_INCLUDE_DIRS}
    include)

  catkin_package()

  add_executable(${PROJECT_NAME} src/slam_service_ros1.cpp)
  target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} ${PCL_LIBRARIES})

else(ROS_EDITION STREQUAL "ROS2")
  find_package(ament_cmake REQUIRED)
  find_package(rclcpp REQUIRED)
  find_package(slam_interfaces REQUIRED)

  ament_package()

  add_executable(${PROJECT_NAME} src/slam_service_ros2.cpp)
  target_link_libraries(${PROJECT_NAME} ${PCL_LIBRARIES})
  ament_target_dependencies(${PROJECT_NAME} rclcpp slam_interfaces)

  install(TARGETS ${PROJECT_NAME}
    DESTINATION lib/${PROJECT_NAME})
endif()
