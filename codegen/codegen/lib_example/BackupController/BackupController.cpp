//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: BackupController.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 27-Feb-2020 15:55:30
//

// Include Files
#include <cmath>
#include <string.h>
#include "rt_nonfinite.h"
#include "BackupController.h"
#include "backupU.h"
#include "BackupController_rtwutil.h"

// Function Definitions

//
// Arguments    : const double X[17]
//                const double M[6]
//                double u[4]
//                creal_T du[68]
// Return Type  : void
//
void BackupController(const double X[17], const double M[6], double u[4],
                      creal_T du[68])
{
  double t2;
  double t3;
  double t22_re;
  double t22_im;
  double t23_re;
  double t23_im;
  double t19;
  double t20;
  double t28;
  double t29;
  double t30_re;
  double t31_re;
  double t146_im;
  double t35;
  double t36;
  double t37;
  double t39;
  double t51_re;
  double t51_im;
  double t40;
  double t44;
  double t45;
  double t59;
  double t46;
  double t60;
  double t111;
  double t112;
  double t113;
  double t114;
  double t29_re_tmp;
  double t29_im_tmp;
  double t30_re_tmp;
  double t30_im_tmp;
  double X_re_tmp;
  double b_t29_re_tmp;
  double b_t29_im_tmp;
  double t31_re_tmp;
  double t31_im_tmp;
  double b_X_re_tmp;
  double X_re;
  double X_im;
  double X_im_tmp;
  double b_X_re;
  double b_X_im;
  double c_X_re_tmp;
  double t142_re;
  double t142_im;
  double d_X_re_tmp;
  double e_X_re_tmp;
  double b_X_im_tmp;
  double f_X_re_tmp;
  double c_X_im_tmp;
  double X_re_tmp_tmp;
  double t153_re;
  double t153_im;
  double t154_re;
  double t154_im;
  creal_T dcv0[68];
  backupU(X, M, u);

  // BACKUPDU
  //     DU = BACKUPDU(IN1,IN2)
  //     This function was generated by the Symbolic Math Toolbox version 8.3.
  //     27-Feb-2020 15:55:26
  t2 = X[5] * 4.0;
  t3 = X[6] * 4.0;
  t22_re = X[3] * 0.0;
  t22_im = X[3] * 2.0;
  t23_re = X[4] * 0.0;
  t23_im = X[4] * 2.0;
  t19 = X[5] * X[5] * 2.0;
  t20 = X[6] * X[6] * 2.0;
  t28 = X[3] * X[4] * 2.0 + X[5] * X[6] * 2.0;
  t29 = X[3] * X[6] * 2.0 + X[4] * X[5] * 2.0;
  t30_re = -t3 + t22_re;
  t31_re = -t2 + t23_re;
  t146_im = X[3] * X[5] * 2.0 + -(X[4] * X[6] * 2.0);
  t35 = (X[4] * X[4] * 2.0 + t19) - 1.0;
  t36 = (t19 + t20) - 1.0;
  t37 = t35 * t35;
  t39 = 1.0 / t35;
  t51_re = (((-t19 + -t20) + X[6] * t22_re) + X[5] * t23_re) + 1.0;
  t51_im = X[6] * t22_im + X[5] * t23_im;
  t40 = 1.0 / t37;
  t19 = t51_re;
  t20 = t51_im;
  if (t51_im == 0.0) {
    if (t51_re < 0.0) {
      t51_re = -1.0;
    } else if (t51_re > 0.0) {
      t51_re = 1.0;
    } else {
      if (t51_re == 0.0) {
        t51_re = 0.0;
      }
    }

    t51_im = 0.0;
  } else {
    if ((std::abs(t51_re) > 8.9884656743115785E+307) || (std::abs(t51_im) >
         8.9884656743115785E+307)) {
      t51_re /= 2.0;
      t51_im /= 2.0;
    }

    t23_re = rt_hypotd_snf(t51_re, t51_im);
    if (t23_re == 0.0) {
      t51_re = 0.0;
      t51_im = 0.0;
    } else {
      t51_re /= t23_re;
      t51_im /= t23_re;
    }
  }

  t44 = M[5] * X[4] * t40 * 4.0;
  t45 = M[5] * t2 * t40;
  t59 = 1.0 / rt_hypotd_snf(t19, t20);
  t20 = 1.0 / (t28 * t28 + t37);
  t46 = 1.0 / std::sqrt(-(t146_im * t146_im) + 1.0);
  t60 = t59 * t59;
  t23_re = M[2] * M[0];
  t19 = t23_re * t29 * t59;
  t23_re = t23_re * t36 * t59;
  t111 = t19 + t23_re;
  t37 *= t20;
  t112 = -t19 + t23_re;
  t113 = t19 + -t23_re;
  t114 = -t19 + -t23_re;
  t29_re_tmp = t29 * t30_re;
  t29_im_tmp = t29 * t22_im;
  t30_re_tmp = t36 * t30_re;
  t30_im_tmp = t36 * t22_im;
  X_re_tmp = X[3] * t59;
  t22_im = M[0] * (-(t60 * (t29_re_tmp * t51_im + t29_im_tmp * t51_re) * X[7]) +
                   -(t60 * (t30_re_tmp * t51_im + t30_im_tmp * t51_re) * X[8]));
  b_t29_re_tmp = t29 * t31_re;
  b_t29_im_tmp = t29 * t23_im;
  t31_re_tmp = t36 * t31_re;
  t31_im_tmp = t36 * t23_im;
  b_X_re_tmp = X[4] * t59;
  t23_im = M[0] * (-(t60 * (b_t29_re_tmp * t51_im + b_t29_im_tmp * t51_re) * X[7])
                   + -(t60 * (t31_re_tmp * t51_im + t31_im_tmp * t51_re) * X[8]));
  t19 = X[6] * t29;
  t30_re = t19 * t51_re;
  t146_im = t19 * t51_im;
  X_re = X[7] * (t60 * t30_re);
  X_im = X[7] * (t60 * t146_im);
  t19 = X[6] * t36;
  t22_re = t19 * t51_re;
  X_im_tmp = t19 * t51_im;
  b_X_re = X[8] * (t60 * t22_re);
  b_X_im = X[8] * (t60 * X_im_tmp);
  t19 = X[6] * t59;
  c_X_re_tmp = t19 * X[7];
  t142_re = M[2] * (X[4] * t35 * t20 * 2.0 + M[0] * ((-(c_X_re_tmp * 2.0) +
    (X_re * 0.0 - X_im * 2.0)) + (b_X_re * 0.0 - b_X_im * 2.0)));
  t142_im = M[2] * (M[0] * ((X_re * 2.0 + X_im * 0.0) + (b_X_re * 2.0 + b_X_im *
    0.0)));
  d_X_re_tmp = X[5] * t29;
  e_X_re_tmp = d_X_re_tmp * t51_re;
  b_X_im_tmp = d_X_re_tmp * t51_im;
  X_re = X[7] * (t60 * e_X_re_tmp);
  X_im = X[7] * (t60 * b_X_im_tmp);
  d_X_re_tmp = X[5] * t36;
  f_X_re_tmp = d_X_re_tmp * t51_re;
  c_X_im_tmp = d_X_re_tmp * t51_im;
  b_X_re = X[8] * (t60 * f_X_re_tmp);
  b_X_im = X[8] * (t60 * c_X_im_tmp);
  X_re_tmp_tmp = X[5] * t59;
  d_X_re_tmp = X_re_tmp_tmp * X[7];
  t153_re = M[2] * (t37 * (X[3] * t39 * 2.0 + -(X[4] * t28 * t40 * 4.0)) + M[0] *
                    ((-(d_X_re_tmp * 2.0) + (X_re * 0.0 - X_im * 2.0)) + (b_X_re
    * 0.0 - b_X_im * 2.0)));
  t153_im = M[2] * (M[0] * ((X_re * 2.0 + X_im * 0.0) + (b_X_re * 2.0 + b_X_im *
    0.0)));
  t23_re = X[5] * t35 * t20 * 2.0 - M[0] * (((X_re_tmp * X[7] * 2.0 + t3 * t59 *
    X[8]) + -(t60 * (t29_re_tmp * t51_re - t29_im_tmp * t51_im) * X[7])) + -(t60
    * (t30_re_tmp * t51_re - t30_im_tmp * t51_im) * X[8]));
  t154_re = -M[2] * t23_re;
  t154_im = -M[2] * (0.0 - t22_im);
  t3 = M[2] * t23_re;
  t35 = M[2] * (0.0 - t22_im);
  t23_re = t37 * (X[6] * t39 * 2.0 + X[5] * t28 * t40 * -4.0) - M[0] *
    (((b_X_re_tmp * X[7] * 2.0 + t2 * t59 * X[8]) + -(t60 * (b_t29_re_tmp *
        t51_re - b_t29_im_tmp * t51_im) * X[7])) + -(t60 * (t31_re_tmp * t51_re
       - t31_im_tmp * t51_im) * X[8]));
  t36 = -M[2] * t23_re;
  t59 = -M[2] * (0.0 - t23_im);
  t29 = M[2] * t23_re;
  t31_re = M[2] * (0.0 - t23_im);
  X_re = X[8] * (t60 * t30_re);
  X_im = X[8] * (t60 * t146_im);
  b_X_re = X[7] * (t60 * t22_re);
  b_X_im = X[7] * (t60 * X_im_tmp);
  t19 = M[2] * (X[5] * t46 * 2.0 + M[0] * ((t19 * X[8] * 2.0 + -(X_re * 0.0 -
    X_im * 2.0)) + (b_X_re * 0.0 - b_X_im * 2.0)));
  t37 = M[2] * (M[0] * (-(X_re * 2.0 + X_im * 0.0) + (b_X_re * 2.0 + b_X_im *
    0.0)));
  t30_re = M[2] * (X[3] * t46 * 2.0 + M[0] * (((b_X_re_tmp * X[8] * 2.0 +
    d_X_re_tmp * -4.0) + -(t60 * (b_t29_re_tmp * t51_re - b_t29_im_tmp * t51_im)
    * X[8])) + t60 * (t31_re_tmp * t51_re - t31_im_tmp * t51_im) * X[7]));
  t146_im = M[2] * (M[0] * (-(t60 * (b_t29_re_tmp * t51_im + b_t29_im_tmp *
    t51_re) * X[8]) + t60 * (t31_re_tmp * t51_im + t31_im_tmp * t51_re) * X[7]));
  X_re = X[8] * (t60 * e_X_re_tmp);
  X_im = X[8] * (t60 * b_X_im_tmp);
  b_X_re = X[7] * (t60 * f_X_re_tmp);
  b_X_im = X[7] * (t60 * c_X_im_tmp);
  t23_re = M[2] * (X[6] * t46 * 2.0 + -(M[0] * ((X_re_tmp_tmp * X[8] * 2.0 +
    -(X_re * 0.0 - X_im * 2.0)) + (b_X_re * 0.0 - b_X_im * 2.0))));
  t20 = M[2] * -(M[0] * (-(X_re * 2.0 + X_im * 0.0) + (b_X_re * 2.0 + b_X_im *
    0.0)));
  t22_re = M[2] * (X[4] * t46 * 2.0 + -(M[0] * (((X_re_tmp * X[8] * 2.0 +
    c_X_re_tmp * -4.0) + -(t60 * (t29_re_tmp * t51_re - t29_im_tmp * t51_im) *
    X[8])) + t60 * (t30_re_tmp * t51_re - t30_im_tmp * t51_im) * X[7])));
  t22_im = M[2] * -(M[0] * (-(t60 * (t29_re_tmp * t51_im + t29_im_tmp * t51_re) *
    X[8]) + t60 * (t30_re_tmp * t51_im + t30_im_tmp * t51_re) * X[7]));
  dcv0[0].re = 0.0;
  dcv0[0].im = 0.0;
  dcv0[1].re = 0.0;
  dcv0[1].im = 0.0;
  dcv0[2].re = 0.0;
  dcv0[2].im = 0.0;
  dcv0[3].re = 0.0;
  dcv0[3].im = 0.0;
  dcv0[4].re = 0.0;
  dcv0[4].im = 0.0;
  dcv0[5].re = 0.0;
  dcv0[5].im = 0.0;
  dcv0[6].re = 0.0;
  dcv0[6].im = 0.0;
  dcv0[7].re = 0.0;
  dcv0[7].im = 0.0;
  dcv0[8].re = 0.0;
  dcv0[8].im = 0.0;
  dcv0[9].re = 0.0;
  dcv0[9].im = 0.0;
  dcv0[10].re = 0.0;
  dcv0[10].im = 0.0;
  dcv0[11].re = 0.0;
  dcv0[11].im = 0.0;
  dcv0[12].re = t19 + -t142_re;
  dcv0[12].im = t37 + -t142_im;
  dcv0[13].re = -t19 + -t142_re;
  dcv0[13].im = -t37 + -t142_im;
  dcv0[14].re = -t19 + t142_re;
  dcv0[14].im = -t37 + t142_im;
  dcv0[15].re = t19 + t142_re;
  dcv0[15].im = t37 + t142_im;
  t19 = t44 + -t23_re;
  dcv0[16].re = t19 + -t153_re;
  dcv0[16].im = -t20 + -t153_im;
  t23_re += t44;
  dcv0[17].re = t23_re + -t153_re;
  dcv0[17].im = t20 + -t153_im;
  dcv0[18].re = t23_re + t153_re;
  dcv0[18].im = t20 + t153_im;
  dcv0[19].re = t19 + t153_re;
  dcv0[19].im = -t20 + t153_im;
  t19 = t45 + t30_re;
  dcv0[20].re = t19 + t36;
  dcv0[20].im = t146_im + t59;
  t23_re = t45 + -t30_re;
  dcv0[21].re = t23_re + t36;
  dcv0[21].im = -t146_im + t59;
  dcv0[22].re = t23_re + t29;
  dcv0[22].im = -t146_im + t31_re;
  dcv0[23].re = t19 + t29;
  dcv0[23].im = t146_im + t31_re;
  dcv0[24].re = -t22_re + t154_re;
  dcv0[24].im = -t22_im + t154_im;
  dcv0[25].re = t22_re + t154_re;
  dcv0[25].im = t22_im + t154_im;
  dcv0[26].re = t22_re + t3;
  dcv0[26].im = t22_im + t35;
  dcv0[27].re = -t22_re + t3;
  dcv0[27].im = -t22_im + t35;
  dcv0[28].re = t113;
  dcv0[28].im = 0.0;
  dcv0[29].re = t111;
  dcv0[29].im = 0.0;
  dcv0[30].re = t112;
  dcv0[30].im = 0.0;
  dcv0[31].re = t114;
  dcv0[31].im = 0.0;
  dcv0[32].re = t111;
  dcv0[32].im = 0.0;
  dcv0[33].re = t112;
  dcv0[33].im = 0.0;
  dcv0[34].re = t114;
  dcv0[34].im = 0.0;
  dcv0[35].re = t113;
  dcv0[35].im = 0.0;
  dcv0[36].re = -M[1];
  dcv0[36].im = 0.0;
  dcv0[37].re = -M[1];
  dcv0[37].im = 0.0;
  dcv0[38].re = -M[1];
  dcv0[38].im = 0.0;
  dcv0[39].re = -M[1];
  dcv0[39].im = 0.0;
  dcv0[40].re = M[3];
  dcv0[40].im = 0.0;
  dcv0[41].re = M[3];
  dcv0[41].im = 0.0;
  dcv0[42].re = -M[3];
  dcv0[42].im = 0.0;
  dcv0[43].re = -M[3];
  dcv0[43].im = 0.0;
  dcv0[44].re = M[3];
  dcv0[44].im = 0.0;
  dcv0[45].re = -M[3];
  dcv0[45].im = 0.0;
  dcv0[46].re = -M[3];
  dcv0[46].im = 0.0;
  dcv0[47].re = M[3];
  dcv0[47].im = 0.0;
  dcv0[48].re = M[4];
  dcv0[48].im = 0.0;
  dcv0[49].re = -M[4];
  dcv0[49].im = 0.0;
  dcv0[50].re = M[4];
  dcv0[50].im = 0.0;
  dcv0[51].re = -M[4];
  dcv0[51].im = 0.0;
  dcv0[52].re = 0.0;
  dcv0[52].im = 0.0;
  dcv0[53].re = 0.0;
  dcv0[53].im = 0.0;
  dcv0[54].re = 0.0;
  dcv0[54].im = 0.0;
  dcv0[55].re = 0.0;
  dcv0[55].im = 0.0;
  dcv0[56].re = 0.0;
  dcv0[56].im = 0.0;
  dcv0[57].re = 0.0;
  dcv0[57].im = 0.0;
  dcv0[58].re = 0.0;
  dcv0[58].im = 0.0;
  dcv0[59].re = 0.0;
  dcv0[59].im = 0.0;
  dcv0[60].re = 0.0;
  dcv0[60].im = 0.0;
  dcv0[61].re = 0.0;
  dcv0[61].im = 0.0;
  dcv0[62].re = 0.0;
  dcv0[62].im = 0.0;
  dcv0[63].re = 0.0;
  dcv0[63].im = 0.0;
  dcv0[64].re = 0.0;
  dcv0[64].im = 0.0;
  dcv0[65].re = 0.0;
  dcv0[65].im = 0.0;
  dcv0[66].re = 0.0;
  dcv0[66].im = 0.0;
  dcv0[67].re = 0.0;
  dcv0[67].im = 0.0;
  memcpy(&du[0], &dcv0[0], 68U * sizeof(creal_T));
}

//
// File trailer for BackupController.cpp
//
// [EOF]
//
