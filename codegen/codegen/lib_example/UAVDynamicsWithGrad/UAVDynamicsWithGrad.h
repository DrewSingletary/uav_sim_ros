//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: UAVDynamicsWithGrad.h
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 27-Feb-2020 15:18:17
//
#ifndef UAVDYNAMICSWITHGRAD_H
#define UAVDYNAMICSWITHGRAD_H

// Include Files
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"
#include "UAVDynamicsWithGrad_types.h"

// Function Declarations
extern void UAVDynamicsWithGrad(const double X[17], const double V[4], double f
  [17], double g[68], double DFcl[289]);

#endif

//
// File trailer for UAVDynamicsWithGrad.h
//
// [EOF]
//
