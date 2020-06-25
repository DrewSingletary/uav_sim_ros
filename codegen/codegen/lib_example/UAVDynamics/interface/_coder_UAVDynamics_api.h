/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_UAVDynamics_api.h
 *
 * MATLAB Coder version            : 4.2
 * C/C++ source code generated on  : 27-Feb-2020 15:18:13
 */

#ifndef _CODER_UAVDYNAMICS_API_H
#define _CODER_UAVDYNAMICS_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_UAVDynamics_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void UAVDynamics(real_T X[17], real_T f[17], real_T g[68]);
extern void UAVDynamics_api(const mxArray * const prhs[1], int32_T nlhs, const
  mxArray *plhs[2]);
extern void UAVDynamics_atexit(void);
extern void UAVDynamics_initialize(void);
extern void UAVDynamics_terminate(void);
extern void UAVDynamics_xil_shutdown(void);
extern void UAVDynamics_xil_terminate(void);

#endif

/*
 * File trailer for _coder_UAVDynamics_api.h
 *
 * [EOF]
 */
