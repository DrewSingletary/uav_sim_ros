#include "uav_sim_ros/sensor_node.hpp"
#include "uav_sim_ros/common.hpp"
#include <ros/console.h>

using namespace Eigen;
using namespace std;

ros::NodeHandle *nh_;
ros::NodeHandle *nhParams_;
ros::ServiceServer srv_ui_;
ros::Subscriber sub_state_;
ros::Publisher pub_sensor_;


double angular_v_variance_;
double linear_v_variance_;
double pos_variance_;
double eul_variance_;
double eul_imu_variance_;
uav_sim_ros::state state_current_;


// Use the angular velocity see state.msg
// The camera would be using is Realsense T2 65
// assuming you have  a camera that is giving us linear velocitys, positions
// and angles.
// Using the IMU and the camera
// Get random noise for linear v, angles and positions.

// quat2EulZYX - singularity in a convenient part for quadrotors.
// b is body x
// assume camera gives it in world frame.
// Everything has variance as it.

//Copy sensor.msg

// Keep params the same
void stateCallback(const uav_sim_ros::state::ConstPtr msg)
{
  //compute sensor values (no noise)
  state_current_ = *msg;
  std::vector<double> angular_v; 
  std::vector<double> linear_v;
  std::vector<double> pos;

  // Camera euler angles
  Eigen::Quaterniond q;
  Eigen::Vector3d vec;
  Eigen::Vector3d eul;


  angular_v.push_back(state_current_.omegax);
  angular_v.push_back(state_current_.omegay);
  angular_v.push_back(state_current_.omegaz);

  linear_v.push_back(state_current_.vbx);
  linear_v.push_back(state_current_.vby);
  linear_v.push_back(state_current_.vbz);

  pos.push_back(state_current_.x);
  pos.push_back(state_current_.y);
  pos.push_back(state_current_.z);

  vec(0) = state_current_.qx;
  vec(1) = state_current_.qy;
  vec(2) = state_current_.qz;

  q.w() = state_current_.qw;
  q.vec() = vec;

  quat2eulZYX(q,eul);



  // add in random noise
  angular_v[0] += ( ((double)rand()/(double)RAND_MAX) *2-1)*angular_v_variance_;
  angular_v[1] += ( ((double)rand()/(double)RAND_MAX) *2-1)*angular_v_variance_;
  angular_v[2] += ( ((double)rand()/(double)RAND_MAX) *2-1)*angular_v_variance_;

  linear_v[0] += ( ((double)rand()/(double)RAND_MAX) *2-1)*linear_v_variance_;
  linear_v[1] += ( ((double)rand()/(double)RAND_MAX) *2-1)*linear_v_variance_;
  linear_v[2] += ( ((double)rand()/(double)RAND_MAX) *2-1)*linear_v_variance_;

  pos[0] += ( ((double)rand()/(double)RAND_MAX) *2-1)*pos_variance_;
  pos[1] += ( ((double)rand()/(double)RAND_MAX) *2-1)*pos_variance_;
  pos[2] += ( ((double)rand()/(double)RAND_MAX) *2-1)*pos_variance_;

  eul[0] += ( ((double)rand()/(double)RAND_MAX) *2-1)*eul_variance_;
  eul[1] += ( ((double)rand()/(double)RAND_MAX) *2-1)*eul_variance_;
  eul[2] += ( ((double)rand()/(double)RAND_MAX) *2-1)*eul_variance_;



  uav_sim_ros::sensor sensor_msg;

  sensor_msg.header.stamp = ros::Time::now();
  sensor_msg.time = (uint32_t) (state_current_.time*1E6);
  sensor_msg.data.push_back(pos[0]);
  sensor_msg.data.push_back(pos[1]);
  sensor_msg.data.push_back(pos[2]);
  sensor_msg.data.push_back(eul[0]);
  sensor_msg.data.push_back(eul[1]);
  sensor_msg.data.push_back(eul[2]);
  sensor_msg.data.push_back(linear_v[0]);
  sensor_msg.data.push_back(linear_v[1]);
  sensor_msg.data.push_back(linear_v[2]);
  sensor_msg.data.push_back(angular_v[0]);
  sensor_msg.data.push_back(angular_v[1]);
  sensor_msg.data.push_back(angular_v[2]);



  
  // Camera gives euler angles, position and lin vel
  // IMU gives angular velocity and angles. 2 Euler variances - 1 IMU and Camera
  // what comes in slower is more accurate. 

  pub_sensor_.publish(sensor_msg);
}

int main (int argc, char *argv[])
{
  // Init ros
  ros::init(argc,argv,"sensor");
    // Display node info

  // Instantiate NodeHandles
  nhParams_ = new ros::NodeHandle("~");
  nh_ = new ros::NodeHandle();

  // Init pubs, subs and srvs
  sub_state_ = nh_->subscribe<uav_sim_ros::state>("uav_state", 1,stateCallback);
  pub_sensor_ = nh_->advertise<uav_sim_ros::sensor>("sensor", 1);

  // Retreive params
  nhParams_->param<double>("angular_v_variance_",angular_v_variance_,0.001);
  nhParams_->param<double>("linear_v_variance_",linear_v_variance_,0.001);
  nhParams_->param<double>("pos_variance_",pos_variance_,0.001);
  nhParams_->param<double>("eul_variance_",eul_variance_,0.001);
  nhParams_->param<double>("eul_imu_variance_",eul_imu_variance_,0.001);


  // Display node info
  ROS_INFO("Sensor node sucessfully started!");

  ros::spin();

  return 0;
}
