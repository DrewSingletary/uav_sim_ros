#include "uav_sim_ros/safety_filter_node.hpp"

#include "asif++.h"
#include <Eigen/Dense>
#include <algorithm>
#include "UAVDynamicsGradient.h"
#include "BackupController.h"
#include "backupU.h"

ros::NodeHandle *nh_;
ros::NodeHandle *nhParams_;

ros::Subscriber sub_cmdDes_;
ros::Subscriber sub_state_;
ros::Subscriber sub_input_;


ros::Publisher pub_cmd_;
ros::Publisher pub_info_;
ros::Publisher pub_backupTraj_;

uav_sim_ros::input inputDes_;
uav_sim_ros::cmd cmdDes_;
uav_sim_ros::cmd cmdAct_;
uav_sim_ros::state stateCurrent_;
uav_sim_ros::filterInfo filter_info_;
nav_msgs::Path backTrajMsg_;

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

static const uint32_t nx = 17;
static const uint32_t nu = 4;
static const uint32_t npSS = 1;
static const uint32_t npBS = 1;
static const uint32_t npBTSS = 2;

static const double lb[nu] = {0};
static const double ub[nu] = {1};

ASIF::ASIFimplicit *asif;

CyberTimer filterTimer(10000);

void safetySet(const double x[nx], double h[npSS], double Dh[npSS*nx])
{
  h[0] = 5-x[0];
  for (int i = 0; i < npSS*nx; i++) {
    Dh[i] = 0;
  }
  Dh[0] = -1;
}

void backupSet(const double x[nx], double h[1], double Dh[nx])
{
  const Vector3d v(x[7],x[8],x[9]);
  const Vector3d omega(x[10],x[11],x[12]);
  const Vector4d q(x[3],x[4],x[5],x[6]);
  const Vector4d qUnit(1.0,0.0,0.0,0.0);
  filter_info_.vBackup = v.norm();
  const double tmp = (terminalVelMax_*terminalVelMax_);
  h[0] = 1 - v.squaredNorm()/tmp;
  for (int i = 0; i < nx; i++) {
    Dh[i] = 0;
  }
  Dh[7] = -2*x[7]/tmp;
  Dh[8] = -2*x[8]/tmp;
  Dh[9] = -2*x[9]/tmp;
}

void backupController(const double x[nx], double u[nu], double Du[nu*nx])
{
  creal_T Dutemp[68];
  double model[6] = {KpVxy_,KpVz_,KpAttitude_,KdAttitude_,KpOmegaz_,hoverThrust_};
  BackupController(x,model,u,Dutemp);
  for (int i = 0; i < nu*nx; i++) {
    Du[i] = Dutemp[i].re;
  }
  ROS_INFO("state for backup ctrl---------");
  for (int i = 0; i < nx; i++) {
    cout << x[i] << " ";
  }
  cout << endl;
  ROS_INFO("------------------------------");
  cout << endl;
  ROS_INFO("backup control action---------");
  ROS_INFO("u: (%f,%f,%f,%f)",u[0],u[1],u[2],u[3]);
  ROS_INFO("------------------------------");
	// saturateInPlace(u,0.0,1.0,INPUT_LENGTH);
}

void dynamics(const double X[nx], double f[nx], double g[nu*nx])
{
  UAVDynamics(X,f,g);
  double xDot[17];
  for (int i = 0; i < 17; i++){
    double tmp = 0;
    for (int j = 0; j < 4; j++){
      tmp += g[i+j*STATE_LENGTH]*hoverThrust_;
    }
    xDot[i] = f[i]+tmp;
  }
  ROS_INFO("xdot at hover thrust----------");
  for (int i = 0; i < nx; i++) {
    cout << xDot[i] << " ";
  }
  cout << endl;
  ROS_INFO("------------------------------");
}

void dynamicsGradients(const double x[nx], double Df[nx*nx], double Dg[nx*nu*nx])
{
  UAVDynamicsGradient(x,Df,Dg);
}

void filterInput(void)
{

  double xNow[nx] = {stateCurrent_.x,stateCurrent_.y,stateCurrent_.z,
                    stateCurrent_.qw,stateCurrent_.qx,stateCurrent_.qy,stateCurrent_.qz,
                    stateCurrent_.vx,stateCurrent_.vy,stateCurrent_.vz,
                    stateCurrent_.omegax,stateCurrent_.omegay,stateCurrent_.omegaz,
                    stateCurrent_.omega1,stateCurrent_.omega2,stateCurrent_.omega3,stateCurrent_.omega4};
	double uDesNow[nu] = {inputDes_.input[0],inputDes_.input[1],inputDes_.input[2],inputDes_.input[3]};
	double uActNow[nu] = {0.0};
	double tNow = 0.0;
	double relax[2];

	filterTimer.tic();
	int32_t rc = asif->filter(xNow,uDesNow,uActNow,relax);
	filterTimer.toc();

  // ROS_INFO("u: (%f,%f,%f,%f)",uActNow[0],uActNow[1],uActNow[2],uActNow[3]);
  for (int i = 0; i < 5; i ++){
    ROS_INFO("backup traj(%i) omegas: (%f,%f,%f,%f)",i,(*asif).backTraj_[i].second[13],(*asif).backTraj_[i].second[14],(*asif).backTraj_[i].second[15],(*asif).backTraj_[i].second[16]);
  }
  exit(1);

  // //test backup controller
  // creal_T Dutemp[68];
  // double model[6] = {KpVxy_,KpVz_,KpAttitude_,KdAttitude_,KpOmegaz_,hoverThrust_};
  // BackupController(xNow,model,uActNow,Dutemp);
  // ROS_INFO("testing backup controller");
  // ROS_INFO("u: (%f,%f,%f,%f)",uActNow[0],uActNow[1],uActNow[2],uActNow[3]);
  // for (int i = 0; i < 68; i++) {
  //   ROS_INFO("DU(%i): %f",i,Dutemp[i].re);
  // }
  //
  // //test dynamics and gradient
  // double Df[nx*nx];
  // double Dg[nx*nu*nx];
  // double f[nx];
  // double g[nx*nu];
  // dynamics(xNow,f,g);
  // dynamicsGradients(xNow,Df,Dg);
  // ROS_INFO("testing dynamics and gradient");
  // for (int i = 0; i < nx; i++) {
  //   ROS_INFO("f(%i): %f",i,f[i]);
  // }
  // for (int i = 0; i < nx*nu; i++) {
  //   ROS_INFO("g(%i): %f",i,g[i]);
  // }
  // for (int i = 0; i < nx*nx; i++) {
  //   ROS_INFO("df(%i): %f",i,Df[i]);
  // }
  // for (int i = 0; i < nx*nx*nu; i++) {
  //   ROS_INFO("dg(%i): %f",i,Dg[i]);
  // }
  //
  // //test safety filter and backup set
  // double Dh[npSS*nx];
  // double h[npSS];
  // safetySet(xNow,h,Dh);
  // ROS_INFO("safety");
  // for (int i = 0; i < npSS; i++) {
  //   ROS_INFO("h(%i): %f",i,h[i]);
  // }
  // for (int i = 0; i < nx*npSS; i++) {
  //   ROS_INFO("Dh(%i): %f",i,Dh[i]);
  // }
  // backupSet(xNow,h,Dh);
  // ROS_INFO("backup");
  // for (int i = 0; i < 1; i++) {
  //   ROS_INFO("h(%i): %f",i,h[i]);
  // }
  // for (int i = 0; i < nx*1; i++) {
  //   ROS_INFO("Dh(%i): %f",i,Dh[i]);
  // }
  //
  // exit(1);

	if (rc < 0) {
		ROS_INFO("QP FAILED");
	}


  // ROS_INFO("Average filter time: %f", filterTimer.getAverage()*1.0e6);

	// filter_info_.hBackupEnd = asif->hBackupEnd_;
	// filter_info_.filterTimerUs = filterTimer.getAverage()*1.0e6;

	// filter_info_.BTorthoBS = asif->BTorthoBS_;
	// filter_info_.TTS = asif->TTS_;
	// filter_info_.hSafetyNow = asif->hSafetyNow_;
	// filter_info_.asifStatus = ASIF::ASIFimplicit::filterErrorMsgString(rc);
	// filter_info_.relax1 = relax[0];
	// filter_info_.relax2 = relax[1];

  cmdAct_ = cmdDes_;

  pub_cmd_.publish(cmdAct_);

  int i =i  = 0;
  for(auto & pose : backTrajMsg_.poses)
	{
    pose.pose.position.x = (*asif).backTraj_[i].second[0];
		pose.pose.position.y = (*asif).backTraj_[i].second[1];
		pose.pose.position.z = (*asif).backTraj_[i].second[2];
		pose.pose.orientation.w = (*asif).backTraj_[i].second[3];
		pose.pose.orientation.x = (*asif).backTraj_[i].second[4];
		pose.pose.orientation.y = (*asif).backTraj_[i].second[5];
		pose.pose.orientation.z = (*asif).backTraj_[i].second[6];
    i++;
	}

	pub_backupTraj_.publish(backTrajMsg_);
}

void inputCallback(const uav_sim_ros::input::ConstPtr msg)
{
	inputDes_ = *msg;

  filterInput();
}

void cmdCallback(const uav_sim_ros::cmd::ConstPtr msg)
{
	cmdDes_ = *msg;
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
	sub_cmdDes_ = nh_->subscribe<uav_sim_ros::cmd>("uav_cmd_des", 1, cmdCallback);
  sub_input_ = nh_->subscribe<uav_sim_ros::input>("uav_input", 1, inputCallback);

	pub_cmd_ = nh_->advertise<uav_sim_ros::cmd>("uav_cmd", 1);
	pub_info_ = nh_->advertise<uav_sim_ros::filterInfo>("safety_filter_info", 1);
  pub_backupTraj_ = nh_->advertise<nav_msgs::Path>("backup_traj", 1);

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

  ASIF::ASIFimplicit::Options opts;
	opts.backTrajHorizon = backup_Tmax_;
	opts.backTrajDt = integration_dt_;
	opts.relaxReachLb = 5.;
	opts.relaxSafeLb = 1.;


	asif = new ASIF::ASIFimplicit(nx,nu,npSS,npBS,npBTSS,
	                              safetySet,backupSet,dynamics,dynamicsGradients,backupController);

	asif->initialize(lb,ub,opts);
  geometry_msgs::PoseStamped poseTmp;
  backTrajMsg_.header.frame_id = "/world";
  backTrajMsg_.poses.resize((*asif).backTraj_.size(),poseTmp);
  // Take it for a spin
	ros::spin();

	return 0;
}
