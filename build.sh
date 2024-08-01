#!/bin/bash

readonly VERSION_ROS1="ROS1"
readonly VERSION_ROS2="ROS2"

pushd `pwd` > /dev/null
cd `dirname $0`
echo "Working Path: "`pwd`

ROS_VERSION=""

# Set working ROS version
if [ "$1" = "ROS2" ]; then
    ROS_VERSION=${VERSION_ROS2}
elif [ "$1" = "ROS1" ]; then
    ROS_VERSION=${VERSION_ROS1}
else
    echo "Invalid Argument"
    exit
fi
echo "ROS version is: "$ROS_VERSION

# clear `build/` folder.
# TODO: Do not clear these folders, if the last build is based on the same ROS version.
if [ "$2" = "clean" ]; then
    rm -rf ../../build/
    rm -rf ../../devel/
    rm -rf ../../install/
    rm -rf ../../log/
    # clear src/CMakeLists.txt if it exists.
    if [ -f ../CMakeLists.txt ]; then
        rm -f ../CMakeLists.txt
    fi
    echo "Clean build folder"
fi

# substitute the files/folders: CMakeList.txt, package.xml(s)
if [ ${ROS_VERSION} = ${VERSION_ROS1} ]; then
    rm fast_lio/CATKIN_IGNORE
    rm point_lio/CATKIN_IGNORE
    cp -f fast_lio_sam/package_ROS1.xml fast_lio_sam/package.xml
    cp -f backend_optimization/package_ROS1.xml backend_optimization/package.xml
    cp -f fastlio_localization/package_ROS1.xml fastlio_localization/package.xml
    rm robot_pose_ekf/CATKIN_IGNORE
    cp -f slam_interfaces/package_ROS1.xml slam_interfaces/package.xml
    cp -f slam_service/package_ROS1.xml slam_service/package.xml
elif [ ${ROS_VERSION} = ${VERSION_ROS2} ]; then
    touch fast_lio/CATKIN_IGNORE
    touch point_lio/CATKIN_IGNORE
    cp -f fast_lio_sam/package_ROS2.xml fast_lio_sam/package.xml
    cp -f backend_optimization/package_ROS2.xml backend_optimization/package.xml
    cp -f fastlio_localization/package_ROS2.xml fastlio_localization/package.xml
    touch robot_pose_ekf/CATKIN_IGNORE
    cp -f slam_interfaces/package_ROS2.xml slam_interfaces/package.xml
    cp -f slam_service/package_ROS2.xml slam_service/package.xml
    # rm -rf launch/
    # cp -rf launch_ROS2/ launch/
fi

# build
pushd `pwd` > /dev/null
if [ $ROS_VERSION = ${VERSION_ROS1} ]; then
    cd ../../
    catkin_make -DROS_EDITION=${VERSION_ROS1} -DPYTHON_EXECUTABLE=/usr/bin/python3
    # catkin_make install -DROS_EDITION=${VERSION_ROS1} -DPYTHON_EXECUTABLE=/usr/bin/python3
elif [ $ROS_VERSION = ${VERSION_ROS2} ]; then
    cd ../../
    colcon build --cmake-args -DROS_EDITION=${VERSION_ROS2} -DPYTHON_EXECUTABLE=/usr/bin/python3
fi
