#include "math.h"
#include "ros/ros.h"
#include "sensor_msgs/Joy.h"
#include "std_msgs/Float32MultiArray.h"
#include "gazebo_msgs/ModelState.h"
#include <tf/transform_broadcaster.h>
#include <nav_msgs/Odometry.h>
#include "uav_sim_ros/state.h"

uav_sim_ros::state states;

ros::Publisher gazebo_pub;
geometry_msgs::Pose pose;
geometry_msgs::Twist twist;
gazebo_msgs::ModelState modelstate;

void uav_states_cb(const uav_sim_ros::state::ConstPtr msg) {
  states = *msg;

  pose.position.x = states.x;
  pose.position.y = states.y;
  pose.position.z = states.z;

  geometry_msgs::Quaternion odom_quat;
  odom_quat.w = states.qw;
  odom_quat.x = states.qx;
  odom_quat.y = states.qy;
  odom_quat.z = states.qz;
  pose.orientation = odom_quat;

  twist.linear.x = 0.0;
  twist.linear.y = 0.0;
  twist.linear.z = 0.0;
  twist.angular.x = 0.0;
  twist.angular.y = 0.0;
  twist.angular.z = 0.0;

  modelstate.model_name = (std::string) "uav";
  modelstate.reference_frame = (std::string) "world";
  modelstate.pose = pose;
  modelstate.twist = twist;
  gazebo_pub.publish(modelstate);
}

int main(int argc, char **argv) {

  ros::init(argc, argv, "gazebo_interface");

  ros::NodeHandle n;

  ros::Subscriber uav_states_sub = n.subscribe<uav_sim_ros::state>("uav_state", 1, uav_states_cb);

  gazebo_pub = n.advertise<gazebo_msgs::ModelState>("/gazebo/set_model_state", 1000);

  while (ros::ok()) {
    ros::spinOnce();
  }

  return 0;
}
