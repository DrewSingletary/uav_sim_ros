/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_UAVDynamicsWithGrad_api.h
 *
 * MATLAB Coder version            : 4.2
 * C/C++ source code generated on  : 27-Feb-2020 15:18:17
 */

#ifndef _CODER_UAVDYNAMICSWITHGRAD_API_H
#define _CODER_UAVDYNAMICSWITHGRAD_API_H

/* Include Files */
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"
#include <stddef.h>
#include <stdlib.h>
#include "_coder_UAVDynamicsWithGrad_api.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void UAVDynamicsWithGrad(real_T X[17], real_T V[4], real_T f[17], real_T
  g[68], real_T DFcl[289]);
extern void UAVDynamicsWithGrad_api(const mxArray * const prhs[2], int32_T nlhs,
  const mxArray *plhs[3]);
extern void UAVDynamicsWithGrad_atexit(void);
extern void UAVDynamicsWithGrad_initialize(void);
extern void UAVDynamicsWithGrad_terminate(void);
extern void UAVDynamicsWithGrad_xil_shutdown(void);
extern void UAVDynamicsWithGrad_xil_terminate(void);

#endif

/*
 * File trailer for _coder_UAVDynamicsWithGrad_api.h
 *
 * [EOF]
 */
