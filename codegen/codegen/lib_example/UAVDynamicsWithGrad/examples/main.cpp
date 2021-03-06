//
// Academic License - for use in teaching, academic research, and meeting
// course requirements at degree granting institutions only.  Not for
// government, commercial, or other organizational use.
// File: main.cpp
//
// MATLAB Coder version            : 4.2
// C/C++ source code generated on  : 27-Feb-2020 15:18:17
//

//***********************************************************************
// This automatically generated example C++ main file shows how to call
// entry-point functions that MATLAB Coder generated. You must customize
// this file for your application. Do not modify this file directly.
// Instead, make a copy of this file, modify it, and integrate it into
// your development environment.
//
// This file initializes entry-point function arguments to a default
// size and value before calling the entry-point functions. It does
// not store or use any values returned from the entry-point functions.
// If necessary, it does pre-allocate memory for returned values.
// You can use this file as a starting point for a main function that
// you can deploy in your application.
//
// After you copy the file, and before you deploy it, you must make the
// following changes:
// * For variable-size function arguments, change the example sizes to
// the sizes that your application requires.
// * Change the example values of function arguments to the values that
// your application requires.
// * If the entry-point functions return values, store these values or
// otherwise use them as required by your application.
//
//***********************************************************************
// Include Files
#include "rt_nonfinite.h"
#include "UAVDynamicsWithGrad.h"
#include "main.h"
#include "UAVDynamicsWithGrad_terminate.h"
#include "UAVDynamicsWithGrad_initialize.h"

// Function Declarations
static void argInit_17x1_real_T(double result[17]);
static void argInit_4x1_real_T(double result[4]);
static double argInit_real_T();
static void main_UAVDynamicsWithGrad();

// Function Definitions

//
// Arguments    : double result[17]
// Return Type  : void
//
static void argInit_17x1_real_T(double result[17])
{
  int idx0;

  // Loop over the array to initialize each element.
  for (idx0 = 0; idx0 < 17; idx0++) {
    // Set the value of the array element.
    // Change this value to the value that the application requires.
    result[idx0] = argInit_real_T();
  }
}

//
// Arguments    : double result[4]
// Return Type  : void
//
static void argInit_4x1_real_T(double result[4])
{
  double result_tmp;

  // Loop over the array to initialize each element.
  // Set the value of the array element.
  // Change this value to the value that the application requires.
  result_tmp = argInit_real_T();
  result[0] = result_tmp;

  // Set the value of the array element.
  // Change this value to the value that the application requires.
  result[1] = result_tmp;

  // Set the value of the array element.
  // Change this value to the value that the application requires.
  result[2] = argInit_real_T();

  // Set the value of the array element.
  // Change this value to the value that the application requires.
  result[3] = argInit_real_T();
}

//
// Arguments    : void
// Return Type  : double
//
static double argInit_real_T()
{
  return 0.0;
}

//
// Arguments    : void
// Return Type  : void
//
static void main_UAVDynamicsWithGrad()
{
  double dv1[17];
  double dv2[4];
  double f[17];
  double g[68];
  double DFcl[289];

  // Initialize function 'UAVDynamicsWithGrad' input arguments.
  // Initialize function input argument 'X'.
  // Initialize function input argument 'V'.
  // Call the entry-point 'UAVDynamicsWithGrad'.
  argInit_17x1_real_T(dv1);
  argInit_4x1_real_T(dv2);
  UAVDynamicsWithGrad(dv1, dv2, f, g, DFcl);
}

//
// Arguments    : int argc
//                const char * const argv[]
// Return Type  : int
//
int main(int, const char * const [])
{
  // Initialize the application.
  // You do not need to do this more than one time.
  UAVDynamicsWithGrad_initialize();

  // Invoke the entry-point functions.
  // You can call entry-point functions multiple times.
  main_UAVDynamicsWithGrad();

  // Terminate the application.
  // You do not need to do this more than one time.
  UAVDynamicsWithGrad_terminate();
  return 0;
}

//
// File trailer for main.cpp
//
// [EOF]
//
