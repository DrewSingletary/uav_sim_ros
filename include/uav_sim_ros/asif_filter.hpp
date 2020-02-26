#ifndef ASIF_FILTER_H
#define ASIF_FILTER_H

#include "asif++.h"
#include <Eigen/Dense>
#include <algorithm>
#include "UAVDynamicsGradient.h"

static const uint32_t nx = 17;
static const uint32_t nu = 4;
static const uint32_t npSS = 2;
static const uint32_t npBS = 1;
static const uint32_t npBTSS = 2;

static const double lb[nu] = {0};
static const double ub[nu] = {1};

ASIF::ASIFimplicit *asif;

void safetySet(const double x[nx], double h[npSS], double Dh[npSS*nx])
{

}

void backupSet(const double x[nx], double h[1], double Dh[nx])
{
  const Vector3d v(x[7],x[8],x[9]);
  const Vector3d omega(x[10],x[11],x[12]);
  const Vector4d q(x[3],x[4],x[5],x[6]);
  const Vector4d qUnit(1.0,0.0,0.0,0.0);
  filter_info_.vBackup = v.norm();
  const double tmp = (terminalVelMax_*terminalVelMax_);
  h = 1 - v.squaredNorm()/tmp; - omega.squaredNorm()/tmp - (q-qUnit).squaredNorm()/tmp;
}

void backupController(const double x[nx], double u[nu], double Du[nu*nx])
{
  
}


void dynamics(const double X[nx], double f[nx], double g[nu*nx])
{
  UAVDynamics(X,f,g);
}

void dynamicsGradients(const double x[nx], double Df[nx*nx], double Dg[nx*nu*nx])
{
  UAVDynamicsGradient(x,Df,Dg);
}

#endif
