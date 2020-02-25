#include "uav_sim_ros/safety_filter_node.hpp"

ros::NodeHandle *nh_;
ros::NodeHandle *nhParams_;

ros::Subscriber sub_cmdDes_;
ros::Subscriber sub_state_;

ros::Publisher pub_cmd_;
ros::Publisher pub_info_;

uav_sim_ros::cmd cmdDes_;
uav_sim_ros::cmd cmdAct_;
uav_sim_ros::state stateCurrent_;
uav_sim_ros::filterInfo filter_info_;

double backup_Tmax_;
double terminalVelMax_;
int32_t passTrough_;
double tau_safety_;
double tau_backup_;
double safety_buffer_;

double integration_dt_;
uint32_t integration_steps_;

// Controller gains
double KiVz_;
double KpVz_;
double KpVxy_;
double KpAttitude_;
double KdAttitude_;
double KpOmegaz_;
double maxInclination_;
double hoverThrust_;
double uzInt_;

void safetySet(const double x[STATE_LENGTH],
                         double &h)
{

}

void backupSet(const double x[STATE_LENGTH],
                     double &h)
{
	const Vector3d v(x[7],x[8],x[9]);
	const Vector3d omega(x[10],x[11],x[12]);
	const Vector4d q(x[3],x[4],x[5],x[6]);
	const Vector4d qUnit(1.0,0.0,0.0,0.0);
	filter_info_.vBackup = v.norm();
	const double tmp = (terminalVelMax_*terminalVelMax_);
	h = 1 - v.squaredNorm()/tmp; - omega.squaredNorm()/tmp - (q-qUnit).squaredNorm()/tmp;
}

void backupController(const double t,
                      const double x[STATE_LENGTH],
                            double u[INPUT_LENGTH],
                            double vDes[CMD_LENGTH])
{
	//Compute orientations from quaternion
	Quaterniond q(x[3],x[4],x[5],x[6]);

	Matrix3d rotBody2World;
	quat2rotm(q,rotBody2World);

	Vector3d eul;
	quat2eulZYX(q,eul);
	Vector3d zBodyInWorld = rotBody2World*(Vector3d() << 0, 0, 1).finished();

	// Compute body frame velocity
	Vector3d v(x[7],x[8],x[9]);
	Vector3d vWorldNoYaw = rotz(-eul(2))*v;

	// Compute desired velocity
	Vector3d vDesVec(0.0,0.0,0.0);
	double yawDotDes = 0.0;

	vDes[0] = vDesVec(0);
	vDes[1] = vDesVec(1);
	vDes[2] = vDesVec(2);
	vDes[3] = yawDotDes;

	// Compute error
	const double vxError = saturate(KpVxy_*(vDesVec(0) - vWorldNoYaw(0)),-deg2rad(maxInclination_),deg2rad(maxInclination_));
	const double vyError = saturate(KpVxy_*(vDesVec(1) - vWorldNoYaw(1)),-deg2rad(maxInclination_),deg2rad(maxInclination_));
	const double vzError = vDesVec(2) - vWorldNoYaw(2);

	const double rollError = -vyError-eul(0);
	const double pitchError = vxError-eul(1);
	const double rollErrorDot = -x[10];
	const double pitchErrorDot = -x[11];
	const double yawErrorDot = yawDotDes - x[12];

	// Compute vz PI output
	const double uz = saturate(KpVz_*vzError + uzInt_/zBodyInWorld(2),hoverThrust_-0.3,hoverThrust_+0.3);
	const double uroll = saturate(KpAttitude_*rollError + KdAttitude_*rollErrorDot,-0.5,0.5);
	const double upitch = saturate(KpAttitude_*pitchError + KdAttitude_*pitchErrorDot,-0.5,0.5);
	const double uomegaz = saturate(KpOmegaz_*yawErrorDot,-0.2,0.2);

	// Fill up message
	u[0] = uz - uroll - upitch - uomegaz;
	u[1] = uz - uroll + upitch + uomegaz;
	u[2] = uz + uroll + upitch - uomegaz;
	u[3] = uz + uroll - upitch + uomegaz;

	// Clamp inputs
	saturateInPlace(u,0.0,1.0,INPUT_LENGTH);
}

void dynamicsCL(const state_t &x,
                      state_t &xDot,
                const double t)
{
	xDot = x;
	double inputBackup[INPUT_LENGTH];
	double cmdBackup[CMD_LENGTH];
	backupController(t,x.data(),inputBackup,cmdBackup);


  #ifdef CODEGEN
  UAVDynamics(x.data(),inputBackup,xDot.data());
  #else
  uavDynamics(x.data(),inputBackup,xDot.data(),t);
  #endif
}

void filterInput(void)
{
  cmdAct_ = cmdDes_;

  pub_cmd_.publish(cmdAct_);
}

void inputCallback(const uav_sim_ros::cmd::ConstPtr msg)
{
	cmdDes_ = *msg;

  filterInput();
}

void stateCallback(const uav_sim_ros::state::ConstPtr msg)
{
	stateCurrent_ = *msg;
}

int main(int argc, char *argv[])
{

	// Init ros
	ros::init(argc,argv,"safety_filter");

	// Instanciate NodeHandles
	nhParams_ = new ros::NodeHandle("~");
	nh_ = new ros::NodeHandle();

	// Init pubs, subs and srvs
	sub_state_ = nh_->subscribe<uav_sim_ros::state>("uav_state", 1, stateCallback);
	sub_cmdDes_ = nh_->subscribe<uav_sim_ros::cmd>("uav_cmd_des", 1, inputCallback);

	pub_cmd_ = nh_->advertise<uav_sim_ros::cmd>("uav_cmd", 1);
	pub_info_ = nh_->advertise<uav_sim_ros::filterInfo>("safety_filter_info", 1);

	// Retreive params
	nhParams_->param<int32_t>("pass_through",passTrough_,0);
	nhParams_->param<double>("integration_dt",integration_dt_,0.01);
	nhParams_->param<double>("backup_Tmax",backup_Tmax_,1.0);
	nhParams_->param<double>("safety_buffer",safety_buffer_,0.3);
	nhParams_->param<double>("terminal_vel_max",terminalVelMax_,0.05);
	nhParams_->param<double>("tau_backup",tau_backup_,0.1);

	nhParams_->param<double>("hoverThrust",hoverThrust_,0.52);
	nhParams_->param<double>("KiVz",KiVz_,5.0);
	nhParams_->param<double>("KpVz",KpVz_,1.0);
	nhParams_->param<double>("KpVxy",KpVxy_,0.7);
	nhParams_->param<double>("KpAttitude",KpAttitude_,10.0);
	nhParams_->param<double>("KdAttitude",KdAttitude_,1.0);
	nhParams_->param<double>("KpOmegaz",KpOmegaz_,2.0);
	nhParams_->param<double>("maxInclination",maxInclination_,30.0);

	// Display node info
	ROS_INFO("Safety Filter node successfuly started with:");
	ROS_INFO("___pass_through=%u",passTrough_);
	ROS_INFO("___integration_dt=%.3f",integration_dt_);
	ROS_INFO("___backup_Tmax=%.3f",backup_Tmax_);
	ROS_INFO("___safety_buffer=%.3f",safety_buffer_);
	ROS_INFO("___terminal_vel_max=%.3f",terminalVelMax_);
	ROS_INFO("___tau_safety=%.3f",tau_safety_);
	ROS_INFO("___tau_backup=%.3f",tau_backup_);

	ROS_INFO("___hoverThrust=%f",hoverThrust_);
	ROS_INFO("___KiVz=%f",KiVz_);
	ROS_INFO("___KpVz=%f",KpVz_);
	ROS_INFO("___KpVxy=%f",KpVxy_);
	ROS_INFO("___KpAttitude=%f",KpAttitude_);
	ROS_INFO("___KdAttitude=%f",KdAttitude_);
	ROS_INFO("___KpOmegaz=%f",KpOmegaz_);
	ROS_INFO("___maxInclination=%f",maxInclination_);

	// Take it for a spin
	ros::spin();

	return 0;
}
