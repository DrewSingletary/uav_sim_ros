#include "uav_sim_ros/ekf.h"

#include "ros/ros.h"
#include "uav_sim_ros/state.h"
#include "uav_sim_ros/sensor.h"
#include "uav_sim_ros/input.h"
#include "uav_sim_ros/common.hpp"
using namespace Eigen;

ros::NodeHandle *nh_;
ros::NodeHandle *nhParams_;
ros::ServiceServer srv_ui_;
ros::Subscriber sub_sensor_;
ros::Subscriber sub_input_;
ros::Publisher pub_state_;
EKF *ekf_ = nullptr;
uav_sim_ros::input inputCurrent_;
uav_sim_ros::sensor sensorCurrent_;


double dt_ = 0;


// States for physical systems 
// Some of the ro
void inputCallback(const uav_sim_ros::input::ConstPtr msg)
{
	inputCurrent_ = *msg;
}

void sensorCallback(const uav_sim_ros::sensor::ConstPtr msg)
{
	dt_ = (msg->time-sensorCurrent_.time)*1E-6;
	sensorCurrent_ = *msg;

  // Populating the measurements
  // All the sensor values should be used

  std::vector<double> angular_v; 
  std::vector<double> linear_v;
  std::vector<double> pos;


  // Camera euler angles
  Eigen::Quaterniond q;
  Eigen::Vector3d  eul;
  Eigen::Vector3d vec;


  // Populate the measurements

  pos.push_back(msg->data[0]);
  pos.push_back(msg->data[1]);
  pos.push_back(msg->data[2]);

  eul[0] = msg->data[3];
  eul[1] = msg->data[4];
  eul[2] = msg->data[5];


  eul2quatZYX(eul,q);

  q.vec() = eul;



  linear_v.push_back(msg->data[6]);
  linear_v.push_back(msg->data[7]);
  linear_v.push_back(msg->data[8]);

  angular_v.push_back(msg->data[9]);
  angular_v.push_back(msg->data[10]);
  angular_v.push_back(msg->data[11]);

  /// Run the EKF
	if (ekf_->initialized == 0) {
    // We have all the states, convert from euler angles to quaternions
    // state is a quaternion.
    // 0.01
		std::vector<double> state_ekf_ = {pos[0], pos[1], pos[2], q.w(), vec[0], vec[1],vec[2],
    linear_v[0], linear_v[1], linear_v[2], angular_v[0], angular_v[1], angular_v[2],0,0,0,0}; // match the state 
//     std::vector<double> state_ekf_ = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//     state_ekf_[0] = q.w();
// // match the state 
		std::vector<double> P_ekf_ = {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,
      0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01};

		ekf_->init(state_ekf_,P_ekf_);
	}
	else if (ekf_->initialized == 1){
    // 4 inputs for drone
    // calls all the measurements

		ekf_->update(std::vector<double> {inputCurrent_.input[0],inputCurrent_.input[1],
      inputCurrent_.input[2], inputCurrent_.input[3]},
								std::vector<double> {pos[0], pos[1], pos[2], q.w(), vec[0], vec[1],vec[2],
    linear_v[0], linear_v[1], linear_v[2], angular_v[0], angular_v[1], angular_v[2],q.w(), vec[0], vec[1], vec[2]},
								dt_);

	}

  // POPULATE THE STATE MESSAGE
	uav_sim_ros::state state_msg;
	state_msg.header.stamp = ros::Time::now();
	state_msg.time = sensorCurrent_.time;
	state_msg.x = ekf_->x_t_[0];
	state_msg.y = ekf_->x_t_[1];
  state_msg.z = ekf_->x_t_[2];
  // Add quaternions for imu and camera


  state_msg.qw = ekf_->x_t_[3];
  state_msg.qx = ekf_->x_t_[4];
  state_msg.qy = ekf_->x_t_[5];
  state_msg.qz = ekf_->x_t_[6];

  state_msg.vbx = ekf_->x_t_[7];
  state_msg.vby = ekf_->x_t_[8];
  state_msg.vbz = ekf_->x_t_[9];

  state_msg.omegax = ekf_->x_t_[10];
  state_msg.omegay = ekf_->x_t_[11];
  state_msg.omegaz = ekf_->x_t_[12];

  state_msg.omega1 = ekf_->x_t_[13];
  state_msg.omega2 = ekf_->x_t_[14];
  state_msg.omega3 = ekf_->x_t_[15];
  state_msg.omega4 = ekf_->x_t_[16];


// Neeed to change for loop


	pub_state_.publish(state_msg);

}



int main (int argc, char *argv[])
{
  // needs to be right size

	ekf_ = new EKF(std::vector<double> {0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,
      0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01,0.01},
								 std::vector<double> {0.001,0.001,0.001,0.001,0.001,0.001,0.001,0.001,
      0.001,0.001,0.001,0.001,0.001,0.001,0.001,0.001,0.001});
	// Init ros
	ros::init(argc,argv,"ekf_node");

	// Instantiate NodeHandles
	nhParams_ = new ros::NodeHandle("~");
	nh_ = new ros::NodeHandle();

	// Init pubs, subs and srvs
	sub_input_ = nh_->subscribe<uav_sim_ros::input>("uav_input", 1,inputCallback);
	sub_sensor_ = nh_->subscribe<uav_sim_ros::sensor>("sensor", 1,sensorCallback);
	pub_state_ = nh_->advertise<uav_sim_ros::state>("state", 1);

	// Retreive params
	// nhParams_->param<double>("enc_v_variance",enc_v_variance_,0.001);

	// Display node info
	ROS_INFO("EKF node successfully started!");

	ros::spin();

	return 0;
}
