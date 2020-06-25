/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_UAVDynamicsGradient_mex.cpp
 *
 * MATLAB Coder version            : 4.2
 * C/C++ source code generated on  : 27-Feb-2020 15:18:20
 */

/* Include Files */
#include "_coder_UAVDynamicsGradient_api.h"
#include "_coder_UAVDynamicsGradient_mex.h"

/* Function Declarations */
static void UAVDynamicsGradient_mexFunction(int32_T nlhs, mxArray *plhs[2],
  int32_T nrhs, const mxArray *prhs[1]);

/* Function Definitions */

/*
 * Arguments    : int32_T nlhs
 *                mxArray *plhs[2]
 *                int32_T nrhs
 *                const mxArray *prhs[1]
 * Return Type  : void
 */
static void UAVDynamicsGradient_mexFunction(int32_T nlhs, mxArray *plhs[2],
  int32_T nrhs, const mxArray *prhs[1])
{
  const mxArray *outputs[2];
  int32_T b_nlhs;
  emlrtStack st = { NULL,              /* site */
    NULL,                              /* tls */
    NULL                               /* prev */
  };

  st.tls = emlrtRootTLSGlobal;

  /* Check for proper number of arguments. */
  if (nrhs != 1) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 1, 4,
                        19, "UAVDynamicsGradient");
  }

  if (nlhs > 2) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 19,
                        "UAVDynamicsGradient");
  }

  /* Call the function. */
  UAVDynamicsGradient_api(prhs, nlhs, outputs);

  /* Copy over outputs to the caller. */
  if (nlhs < 1) {
    b_nlhs = 1;
  } else {
    b_nlhs = nlhs;
  }

  emlrtReturnArrays(b_nlhs, plhs, outputs);
}

/*
 * Arguments    : int32_T nlhs
 *                mxArray * const plhs[]
 *                int32_T nrhs
 *                const mxArray * const prhs[]
 * Return Type  : void
 */
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs, const mxArray
                 *prhs[])
{
  mexAtExit(UAVDynamicsGradient_atexit);

  /* Module initialization. */
  UAVDynamicsGradient_initialize();

  /* Dispatch the entry-point. */
  UAVDynamicsGradient_mexFunction(nlhs, plhs, nrhs, prhs);

  /* Module termination. */
  UAVDynamicsGradient_terminate();
}

/*
 * Arguments    : void
 * Return Type  : emlrtCTX
 */
emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLS(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1);
  return emlrtRootTLSGlobal;
}

/*
 * File trailer for _coder_UAVDynamicsGradient_mex.cpp
 *
 * [EOF]
 */
