//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: BackupController_rtwutil.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 27-Feb-2020 15:55:30
//

// Include Files
#include <cmath>
#include "rt_nonfinite.h"
#include "BackupController_rtwutil.h"

// Function Definitions

//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
double rt_hypotd_snf(double u0, double u1)
{
  double y;
  double a;
  double b;
  a = std::abs(u0);
  b = std::abs(u1);
  if (a < b) {
    a /= b;
    y = b * std::sqrt(a * a + 1.0);
  } else if (a > b) {
    b /= a;
    y = a * std::sqrt(b * b + 1.0);
  } else if (rtIsNaN(b)) {
    y = b;
  } else {
    y = a * 1.4142135623730951;
  }

  return y;
}

//
// File trailer for BackupController_rtwutil.cpp
//
// [EOF]
//
