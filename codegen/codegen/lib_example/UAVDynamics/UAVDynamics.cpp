//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: UAVDynamics.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 27-Feb-2020 15:18:13
//

// Include Files
#include <string.h>
#include "UAVDynamics.h"

// Function Definitions

//
// Arguments    : const double X[17]
//                double f[17]
//                double g[68]
// Return Type  : void
//
void UAVDynamics(const double X[17], double f[17], double g[68])
{
  static const double dv0[68] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 28754.28571428571, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 28754.28571428571, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 28754.28571428571, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 28754.28571428571 };

  double t2;
  double t3;
  double t4;
  double t5;
  double t6;
  double t7;
  double t8;
  double t9;
  double t12;
  double t17;
  memcpy(&g[0], &dv0[0], 68U * sizeof(double));

  // MATF
  //     F = MATF(IN1)
  //     This function was generated by the Symbolic Math Toolbox version 8.3.
  //     27-Feb-2020 15:18:07
  t2 = X[13] * X[13];
  t3 = X[14] * X[14];
  t4 = X[15] * X[15];
  t5 = X[16] * X[16];
  t6 = X[3] * X[3];
  t7 = X[4] * X[4];
  t8 = X[5] * X[5];
  t9 = X[6] * X[6];
  t12 = 1.0 / (((t6 + t7) + t8) + t9);
  t17 = ((t2 * 9.9009900990099013E-6 + t3 * 9.9009900990099013E-6) + t4 *
         9.9009900990099013E-6) + t5 * 9.9009900990099013E-6;
  f[0] = X[7];
  f[1] = X[8];
  f[2] = X[9];
  f[3] = (X[4] * X[10] * -0.5 - X[5] * X[11] / 2.0) - X[6] * X[12] / 2.0;
  f[4] = (X[3] * X[10] / 2.0 + X[5] * X[12] / 2.0) - X[6] * X[11] / 2.0;
  f[5] = (X[3] * X[11] / 2.0 - X[4] * X[12] / 2.0) + X[6] * X[10] / 2.0;
  f[6] = (X[3] * X[12] / 2.0 + X[4] * X[11] / 2.0) - X[5] * X[10] / 2.0;
  f[7] = t17 * (X[3] * X[5] * t12 * 2.0 + X[4] * X[6] * t12 * 2.0);
  f[8] = -t17 * (X[3] * X[4] * t12 * 2.0 - X[5] * X[6] * t12 * 2.0);
  f[9] = t17 * (((t6 * t12 - t7 * t12) - t8 * t12) + t9 * t12) - 9.80665;
  f[10] = (((t2 * 1.4142135623730951 * -0.0001351148476204774 - t3 *
             1.4142135623730951 * 0.0001351148476204774) + t4 *
            1.4142135623730951 * 0.0001351148476204774) + t5 *
           1.4142135623730951 * 0.0001351148476204774) - X[11] * X[12] *
    0.64877645999099243;
  f[11] = (((t2 * 1.4142135623730951 * -0.0001231611358193637 + t3 *
             1.4142135623730951 * 0.0001231611358193637) + t4 *
            1.4142135623730951 * 0.0001231611358193637) - t5 *
           1.4142135623730951 * 0.0001231611358193637) + X[10] * X[12] *
    0.67984946972288751;
  f[12] = (((t2 * -9.6740906354802637E-6 + t3 * 9.6740906354802637E-6) - t4 *
            9.6740906354802637E-6) + t5 * 9.6740906354802637E-6) - X[10] * X[11]
    * 0.055593774185226569;
  f[13] = X[13] * -26.42285714285714 - t2 * 0.0064285714285714276;
  f[14] = X[14] * -26.42285714285714 - t3 * 0.0064285714285714276;
  f[15] = X[15] * -26.42285714285714 - t4 * 0.0064285714285714276;
  f[16] = X[16] * -26.42285714285714 - t5 * 0.0064285714285714276;
}

//
// File trailer for UAVDynamics.cpp
//
// [EOF]
//
