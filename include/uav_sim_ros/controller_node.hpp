#ifndef CONTROLLER_NODE_H
#define CONTROLLER_NODE_H

#include "ros/ros.h"
#include "uav_sim_ros/common.hpp"
#include "uav_sim_ros/input.h"
#include "uav_sim_ros/state.h"
#include "uav_sim_ros/cmd.h"
#include "visualization_msgs/Marker.h"

enum class STATUS : uint8_t
{
	FAILURE = 0,
	RUNNING = 1
};

#endif
