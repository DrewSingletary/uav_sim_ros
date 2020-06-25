/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_BackupController_api.h
 *
 * MATLAB Coder version            : 4.2
 * C/C++ source code generated on  : 27-Feb-2020 15:55:30
 */

#ifndef _CODER_BACKUPCONTROLLER_API_H
#define _CODER_BACKUPCONTROLLER_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_BackupController_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void BackupController(real_T X[17], real_T M[6], real_T u[4], creal_T du
  [68]);
extern void BackupController_api(const mxArray * const prhs[2], int32_T nlhs,
  const mxArray *plhs[2]);
extern void BackupController_atexit(void);
extern void BackupController_initialize(void);
extern void BackupController_terminate(void);
extern void BackupController_xil_shutdown(void);
extern void BackupController_xil_terminate(void);

#endif

/*
 * File trailer for _coder_BackupController_api.h
 *
 * [EOF]
 */
