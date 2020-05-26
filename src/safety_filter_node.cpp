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
ros::Subscriber sub_state_agent_2;
ros::Subscriber sub_backTraj_2;
ros::Subscriber sub_input_;


ros::Publisher pub_cmd_;
ros::Publisher pub_input_;
ros::Publisher pub_info_;
ros::Publisher pub_backupTraj_;
ros::Publisher pub_ball_;
ros::Publisher cmdDesVizu_pub_;

uav_sim_ros::input inputDes_;
uav_sim_ros::input input_;
uav_sim_ros::cmd cmdDes_;
uav_sim_ros::cmd cmdAct_;
uav_sim_ros::state stateCurrent_;
uav_sim_ros::state state2Current_;
uav_sim_ros::filterInfo filter_info_;
nav_msgs::Path backTrajMsg_;
nav_msgs::Path backTraj2Msg_;
visualization_msgs::Marker ball_;
visualization_msgs::Marker cmdDesVizu_;

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

bool multiAgent = false;

static const uint32_t nx = 17;
static const uint32_t nu = 4;
static const uint32_t npSS = 1;
static const uint32_t npBS = 1;
static const uint32_t npBTSS = 2;

static const double lb[nu] = {0,0,0,0};
static const double ub[nu] = {1,1,1,1};

ASIF::ASIFimplicit *asif;

CyberTimer filterTimer(10000);

double ball_radius_ = .5;

bool posOnly_ = false;

void safetySet(const double x[nx], double h[npSS], double Dh[npSS*nx])
{
  double ball_avoid_r = ball_radius_;
  double ball_avoid_x, ball_avoid_y, ball_avoid_z;
  // if no data from the other states, simply avoid a sphere at (5,5,0)
  ball_avoid_x = 5;
  ball_avoid_y = 5;
  ball_avoid_z = 0;
  if (multiAgent){
    if (posOnly_)
    {
      ball_avoid_x = state2Current_.x;
      ball_avoid_y = state2Current_.y;
      ball_avoid_z = state2Current_.z;

      h[0] = ((x[0]-ball_avoid_x)*(x[0]-ball_avoid_x)+(x[1]-ball_avoid_y)*(x[1]-ball_avoid_y)+(x[2]-ball_avoid_z)*(x[2]-ball_avoid_z))-ball_avoid_r*ball_avoid_r;
    } else
    {
      uint32_t size = backTraj2Msg_.poses.size();
      double x2[size] = {0};
      double y2[size] = {0};
      double z2[size] = {0};
      uint32_t i = 0;
      for(auto & pose : backTraj2Msg_.poses)
    	{
        x2[i] = pose.pose.position.x;
        y2[i] = pose.pose.position.y;
        z2[i] = pose.pose.position.z;
        i++;
    	}
      h[0] = 100;
      for (int j = 0; j < backTraj2Msg_.poses.size(); j++)
      {
        double h_current = ((x[0]-x2[j])*(x[0]-x2[j])+(x[1]-y2[j])*(x[1]-y2[j])+(x[2]-z2[j])*(x[2]-z2[j]))-ball_avoid_r*ball_avoid_r;
        if (h[0] > h_current) {
          h[0] = h_current;
          ball_avoid_x = x2[j];
          ball_avoid_y = y2[j];
          ball_avoid_z = z2[j];
        }
      }
    }
  }
  else {
    h[0] = ((x[0]-ball_avoid_x)*(x[0]-ball_avoid_x)+(x[1]-ball_avoid_y)*(x[1]-ball_avoid_y)+(x[2]-ball_avoid_z)*(x[2]-ball_avoid_z))-ball_avoid_r*ball_avoid_r;
  }
  filter_info_.hCurrent = h[0];
  for (int i = 0; i < npSS*nx; i++) {
    Dh[i] = 0;
  }

  Dh[0] = 2*x[0]-2*ball_avoid_x;
  Dh[1] = 2*x[1]-2*ball_avoid_y;
  Dh[2] = 2*x[2]-2*ball_avoid_z;
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
	saturateInPlace(u,0.0,1.0,INPUT_LENGTH);
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
}

void dynamicsGradients(const double x[nx], double Df[nx*nx], double Dg[nx*nu*nx])
{
  UAVDynamicsGradient(x,Df,Dg);
}

void filterInput(void)
{
  if (passTrough_) {
    for (int i = 0; i < nu; i ++)
      input_.input[i] = inputDes_.input[i];
    cmdAct_ = cmdDes_;
    pub_cmd_.publish(cmdAct_);
    return;
  }

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
  // ROS_INFO("Average filter time: %f", filterTimer.getAverage()*1.0e6);

  filter_info_.hBackup = asif->hBackupEnd_;
  filter_info_.hSafety = asif->hSafetyNow_;

	if (rc < 0) {
		ROS_INFO("QP FAILED");
	}

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
  for (int i = 0; i < nu; i ++)
    input_.input[i] = uActNow[i];


}

void inputCallback(const uav_sim_ros::input::ConstPtr msg)
{
	inputDes_ = *msg;

  filterInput();

  pub_input_.publish(input_);

  std::string ns = ros::this_node::getNamespace();
  ball_.header.frame_id = ns+"/"+"uav/base_link";
  ball_.type = visualization_msgs::Marker::SPHERE;
  ball_.action = visualization_msgs::Marker::ADD;
  ball_.color.a = 0.5; // Don't forget to set the alpha!
  ball_.color.r = 1.0;
  ball_.color.g = 0.0;
  ball_.color.b = 0.0;
  ball_.pose.position.x = 0;
  ball_.pose.position.y = 0;
  ball_.pose.position.z = 0;
  ball_.pose.orientation.x = 0.0;
  ball_.pose.orientation.y = 0.0;
  ball_.pose.orientation.z = 0.0;
  ball_.pose.orientation.w = 1.0;
  ball_.scale.x = ball_radius_;
  ball_.scale.y = ball_radius_;
  ball_.scale.z = ball_radius_;
  pub_ball_.publish(ball_);

  const Vector3d vDesVec = {cmdDes_.vDes[0],cmdDes_.vDes[1],cmdDes_.vDes[2]};
  Quaterniond quatVec_(1.0,0.0,0.0,0.0);
  quatVec_ = Quaterniond(stateCurrent_.qw,
                         stateCurrent_.qx,
                         stateCurrent_.qy,
                         stateCurrent_.qz);
  Vector3d eulVec_(0.0,0.0,0.0);
  quat2eulZYX(quatVec_,eulVec_);
  const Vector3d vDesWorldVec = rotz(eulVec_(2))*vDesVec;
  cmdDesVizu_.points[0].x = stateCurrent_.x;
	cmdDesVizu_.points[0].y = stateCurrent_.y;
	cmdDesVizu_.points[0].z = stateCurrent_.z;
	cmdDesVizu_.points[1].x = stateCurrent_.x + vDesWorldVec(0)*KpVxy_;
	cmdDesVizu_.points[1].y = stateCurrent_.y + vDesWorldVec(1)*KpVxy_;
	cmdDesVizu_.points[1].z = stateCurrent_.z + vDesWorldVec(2)*KpVxy_;
  cmdDesVizu_pub_.publish(cmdDesVizu_);
}

void cmdCallback(const uav_sim_ros::cmd::ConstPtr msg)
{
	cmdDes_ = *msg;
}

void stateCallback(const uav_sim_ros::state::ConstPtr msg)
{
	stateCurrent_ = *msg;
}

void state2Callback(const uav_sim_ros::state::ConstPtr msg)
{
	state2Current_ = *msg;
}

void backupTrajCallback(const nav_msgs::Path::ConstPtr msg)
{
	backTraj2Msg_ = *msg;
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
  std::string ns = ros::this_node::getNamespace();
  if (ns.compare("/uav1") == 0)
  {
    sub_state_agent_2 = nh_->subscribe<uav_sim_ros::state>("/uav2/uav_state", 1, state2Callback);
    sub_backTraj_2 = nh_->subscribe<nav_msgs::Path>("/uav2/backup_traj", 1, backupTrajCallback);
    multiAgent = true;
  }
  else if (ns.compare("/uav2") == 0)
  {
    sub_state_agent_2 = nh_->subscribe<uav_sim_ros::state>("/uav1/uav_state", 1, state2Callback);
    sub_backTraj_2 = nh_->subscribe<nav_msgs::Path>("/uav1/backup_traj", 1, backupTrajCallback);
    multiAgent = true;
  }
  else
    ROS_WARN("SHOULDN'T DO THAT: %s",ns.c_str());
	sub_cmdDes_ = nh_->subscribe<uav_sim_ros::cmd>("uav_cmd_des", 1, cmdCallback);
  sub_input_ = nh_->subscribe<uav_sim_ros::input>("uav_input_des", 1, inputCallback);

	pub_cmd_ = nh_->advertise<uav_sim_ros::cmd>("uav_cmd", 1);
  pub_input_ = nh_->advertise<uav_sim_ros::input>("uav_input", 1);
	pub_info_ = nh_->advertise<uav_sim_ros::filterInfo>("safety_filter_info", 1);
  pub_backupTraj_ = nh_->advertise<nav_msgs::Path>("backup_traj", 1);
  pub_ball_ = nh_->advertise<visualization_msgs::Marker>("ball", 1);
  cmdDesVizu_pub_ = nh_->advertise<visualization_msgs::Marker>("uav_cmd_des_vizu", 1);

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

  nhParams_->param<double>("ball_radius",ball_radius_,1.0);
  nhParams_->param<bool>("posOnly",posOnly_,true);

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
	opts.relaxSafeLb = 10.;
  opts.x0 = new double [nx]();
  opts.x0[3] = 1;


	asif = new ASIF::ASIFimplicit(nx,nu,npSS,npBS,npBTSS,
	                              safetySet,backupSet,dynamics,dynamicsGradients,backupController);

	asif->initialize(lb,ub,opts);
  geometry_msgs::PoseStamped poseTmp;
  backTrajMsg_.header.frame_id = "/world";
  backTrajMsg_.poses.resize((*asif).backTraj_.size(),poseTmp);

  cmdDesVizu_.header.frame_id = "/world";
	cmdDesVizu_.ns = ros::this_node::getNamespace();
	cmdDesVizu_.points.resize(2);
	cmdDesVizu_.color.a = 1.0;
	cmdDesVizu_.color.r = 0.0;
	cmdDesVizu_.color.g = 1.0;
	cmdDesVizu_.color.b = 0.0;
	cmdDesVizu_.scale.x = 0.05;
	cmdDesVizu_.scale.y = 0.1;
	cmdDesVizu_.scale.z = 0.1;
	cmdDesVizu_.id = 0;
	cmdDesVizu_.type = visualization_msgs::Marker::ARROW;
	cmdDesVizu_.action = visualization_msgs::Marker::ADD;
  // Take it for a spin
	ros::spin();

	return 0;
}
