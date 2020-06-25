//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: matf.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 27-Feb-2020 15:18:17
//

// Include Files
#include "rt_nonfinite.h"
#include "UAVDynamicsWithGrad.h"
#include "matf.h"

// Function Definitions

//
// MATF
//     F = MATF(IN1)
// Arguments    : const double in1[17]
//                double f[17]
// Return Type  : void
//
void matf(const double in1[17], double f[17])
{
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

  //     This function was generated by the Symbolic Math Toolbox version 8.3.
  //     27-Feb-2020 15:18:07
  t2 = in1[13] * in1[13];
  t3 = in1[14] * in1[14];
  t4 = in1[15] * in1[15];
  t5 = in1[16] * in1[16];
  t6 = in1[3] * in1[3];
  t7 = in1[4] * in1[4];
  t8 = in1[5] * in1[5];
  t9 = in1[6] * in1[6];
  t12 = 1.0 / (((t6 + t7) + t8) + t9);
  t17 = ((t2 * 9.9009900990099013E-6 + t3 * 9.9009900990099013E-6) + t4 *
         9.9009900990099013E-6) + t5 * 9.9009900990099013E-6;
  f[0] = in1[7];
  f[1] = in1[8];
  f[2] = in1[9];
  f[3] = (in1[4] * in1[10] * -0.5 - in1[5] * in1[11] / 2.0) - in1[6] * in1[12] /
    2.0;
  f[4] = (in1[3] * in1[10] / 2.0 + in1[5] * in1[12] / 2.0) - in1[6] * in1[11] /
    2.0;
  f[5] = (in1[3] * in1[11] / 2.0 - in1[4] * in1[12] / 2.0) + in1[6] * in1[10] /
    2.0;
  f[6] = (in1[3] * in1[12] / 2.0 + in1[4] * in1[11] / 2.0) - in1[5] * in1[10] /
    2.0;
  f[7] = t17 * (in1[3] * in1[5] * t12 * 2.0 + in1[4] * in1[6] * t12 * 2.0);
  f[8] = -t17 * (in1[3] * in1[4] * t12 * 2.0 - in1[5] * in1[6] * t12 * 2.0);
  f[9] = t17 * (((t6 * t12 - t7 * t12) - t8 * t12) + t9 * t12) - 9.80665;
  f[10] = (((t2 * 1.4142135623730951 * -0.0001351148476204774 - t3 *
             1.4142135623730951 * 0.0001351148476204774) + t4 *
            1.4142135623730951 * 0.0001351148476204774) + t5 *
           1.4142135623730951 * 0.0001351148476204774) - in1[11] * in1[12] *
    0.64877645999099243;
  f[11] = (((t2 * 1.4142135623730951 * -0.0001231611358193637 + t3 *
             1.4142135623730951 * 0.0001231611358193637) + t4 *
            1.4142135623730951 * 0.0001231611358193637) - t5 *
           1.4142135623730951 * 0.0001231611358193637) + in1[10] * in1[12] *
    0.67984946972288751;
  f[12] = (((t2 * -9.6740906354802637E-6 + t3 * 9.6740906354802637E-6) - t4 *
            9.6740906354802637E-6) + t5 * 9.6740906354802637E-6) - in1[10] *
    in1[11] * 0.055593774185226569;
  f[13] = in1[13] * -26.42285714285714 - t2 * 0.0064285714285714276;
  f[14] = in1[14] * -26.42285714285714 - t3 * 0.0064285714285714276;
  f[15] = in1[15] * -26.42285714285714 - t4 * 0.0064285714285714276;
  f[16] = in1[16] * -26.42285714285714 - t5 * 0.0064285714285714276;
}

//
// File trailer for matf.cpp
//
// [EOF]
//
