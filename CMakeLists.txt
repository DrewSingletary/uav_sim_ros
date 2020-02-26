cmake_minimum_required(VERSION 2.8.3)
project(uav_sim_ros)

## Compile as C++11, supported in ROS Kinetic and newer
add_compile_options(-std=c++17)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

## Find catkin macros and libraries
find_package(catkin REQUIRED COMPONENTS
	roscpp
	message_generation
	std_msgs
	sensor_msgs
	geometry_msgs
	visualization_msgs
	nav_msgs
	tf
	)

## System dependencies are found with CMake's conventions
find_package(Boost REQUIRED)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(asif++ REQUIRED CONFIG)
find_package(rostest REQUIRED)

## Generate messages in the 'msg' folder
add_message_files(
  FILES
  input.msg
  state.msg
  cmd.msg
  filterInfo.msg
)

# Generate services in the 'srv' folder
add_service_files(
  FILES
  ui.srv
)

## Generate added messages and services with any dependencies listed here
generate_messages(DEPENDENCIES std_msgs sensor_msgs nav_msgs geometry_msgs visualization_msgs)

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES ${PROJECT_NAME}
 CATKIN_DEPENDS roscpp
	             message_generation
	             std_msgs
	             sensor_msgs
	             geometry_msgs
	             visualization_msgs
	             nav_msgs
	             tf
	             )

###########
## Build ##
###########
include_directories(
	include codegen/codegen/lib/UAVDynamics codegen/codegen/lib/UAVDynamicsWithGrad codegen/codegen/lib/UAVDynamicsGradient
	${catkin_INCLUDE_DIRS}
)

add_executable(integrator_node src/integrator_node.cpp
               codegen/codegen/lib/UAVDynamics/UAVDynamics.cpp)
add_executable(controller_node src/controller_node.cpp)
add_executable(safety_filter_node src/safety_filter_node.cpp
               codegen/codegen/lib/UAVDynamics/UAVDynamics.cpp
               codegen/codegen/lib/UAVDynamicsGradient/UAVDynamicsGradient.cpp
               codegen/codegen/lib/UAVDynamicsGradient/rt_nonfinite.cpp
               codegen/codegen/lib/UAVDynamicsGradient/rtGetInf.cpp
               codegen/codegen/lib/UAVDynamicsGradient/rtGetNaN.cpp)
add_executable(manual_teleop_node src/manual_teleop_node.cpp src/keyboard.cpp)

# Ensure messages built first
add_dependencies(integrator_node uav_sim_ros_generate_messages_cpp)
add_dependencies(controller_node uav_sim_ros_generate_messages_cpp)
add_dependencies(safety_filter_node uav_sim_ros_generate_messages_cpp)
add_dependencies(manual_teleop_node uav_sim_ros_generate_messages_cpp)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
target_link_libraries(integrator_node
                      ${catkin_LIBRARIES}
                      Eigen3::Eigen
)

target_link_libraries(controller_node
                      ${catkin_LIBRARIES}
                      Eigen3::Eigen
)

target_link_libraries(safety_filter_node
                      ${catkin_LIBRARIES}
                      asif++::asif++
                      asif++::libaffa
                      Eigen3::Eigen
)

target_link_libraries(manual_teleop_node
                      ${catkin_LIBRARIES}
                      Eigen3::Eigen
)
