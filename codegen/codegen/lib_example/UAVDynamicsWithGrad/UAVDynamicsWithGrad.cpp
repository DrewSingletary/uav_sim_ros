//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: UAVDynamicsWithGrad.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 27-Feb-2020 15:18:17
//

// Include Files
#include <string.h>
#include "rt_nonfinite.h"
#include "UAVDynamicsWithGrad.h"
#include "matDFcl.h"
#include "matf.h"

// Function Definitions

//
// Arguments    : const double X[17]
//                const double V[4]
//                double f[17]
//                double g[68]
//                double DFcl[289]
// Return Type  : void
//
void UAVDynamicsWithGrad(const double X[17], const double [4], double f[17],
  double g[68], double DFcl[289])
{
  static const double dv0[68] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 28754.28571428571, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 28754.28571428571, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 28754.28571428571, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 28754.28571428571 };

  memcpy(&g[0], &dv0[0], 68U * sizeof(double));
  matf(X, f);
  matDFcl(X, DFcl);
}

//
// File trailer for UAVDynamicsWithGrad.cpp
//
// [EOF]
//
