#ifndef DYNAMICS_H
#define DYNAMICS_H

#include "UAVDynamicsGradient.h"
#include "UAVDynamics.h"
#include "common.hpp"
#include <math.h>
#include <vector>
#include <stdint.h>
#include <cstring>


// static const uint32_t STATE_LENGTH = 17;


// static const uint32_t INPUT_LENGTH = 4;
static const uint32_t MEASUREMENT_LENGTH = 17;

// When you call 
// static const double model[15] = {44.798,            //mb
//                                  2.485,             //mw
//                                  0.055936595310797, //Jw
//                                  -0.02322718759275, //a2
//                                  0.166845864363019, //c2
//                                  3.604960049044268, //A2
//                                  2*3.836289730154863, //B2
//                                  1.069672194414735, //C2
//                                  1.5, //K
//                                  0.195,             //r
//                                  0.5,               //L
//                                  9.81,              //gGravity
//                                  1.1,                //FricCoeff 3.185188257847262
//                                  1.0e-3,            //velEps
//                                  1.225479467549329  //FricCoeff 1.225479467549329
//                                  };



inline void dynamics(const double t,
              const double X[STATE_LENGTH],
              const double U[INPUT_LENGTH],
                    double xDot[STATE_LENGTH])
{
  double f[STATE_LENGTH];
  double g[INPUT_LENGTH*STATE_LENGTH];
  UAVDynamics(X,f,g);
  for(int i=0; i<STATE_LENGTH; i++)
  {
     xDot[i]=f[i];
  for(int j=0; j<INPUT_LENGTH; j++)
  {
     xDot[i]+=g[i+j*STATE_LENGTH]*U[j];
  }
  }
}

// Dynamics gradent is the same as dyanamics gradient raw


// Need to do - should be inside asif filter - asiffilter.hpp
// Mesurement H and Measurement Gradient do


inline void dynamicsGradient(const double t,
                      const double X[STATE_LENGTH],
                      const double U[INPUT_LENGTH],
                      double Df[STATE_LENGTH*STATE_LENGTH])
{
  double Dg[STATE_LENGTH*INPUT_LENGTH*STATE_LENGTH];
  UAVDynamicsGradient(X,Df,Dg);
  for(uint32_t i=0; i<STATE_LENGTH; i++)
   {
      for(uint32_t j=0; j<STATE_LENGTH; j++)
      {
         double tmp = Df[i+j*STATE_LENGTH];
         for(uint32_t k=0; k<INPUT_LENGTH; k++)
         {
            tmp+=Dg[i+k*(STATE_LENGTH)+j*STATE_LENGTH*INPUT_LENGTH]*U[k];
         }
         Df[i+j*STATE_LENGTH] = tmp;
      }
   }
}

inline void measurementH(const double X[STATE_LENGTH],
                  double h[MEASUREMENT_LENGTH])
{
   h[0] = X[0]; //
   h[1] = X[1]; // 
   h[2] = X[2]; // 
   h[3] = X[3]; // 
   h[4] = X[4]; 
   h[5] = X[5]; // 
   h[6] = X[6]; // 
   h[7] = X[7]; //
   h[8] = X[8]; // 
   h[9] = X[9]; 
   h[10] = X[10];
   h[11] = X[11]; 
   h[12] = X[12]; 
   h[13] = X[3]; 
   h[14] = X[4]; 
   h[15] = X[5]; 
   h[16] = X[6]; 

}

// measuring states directly
// vector of 1s and 0s
// it will change for 9,10,11,12
// 

inline void measurementHGradient(const double X[STATE_LENGTH],
                          double Dh[MEASUREMENT_LENGTH*STATE_LENGTH])
{
   //with respect to x[0]
   Dh[0] = 1. ;
   Dh[1] = 0 ;
   Dh[2] = 0.;
   Dh[3] = 0.;
   Dh[4] = 0.;
   Dh[5] = 0.;
   Dh[6] = 0.;
   Dh[7] = 0.;
   Dh[8] = 0.;
   Dh[9] = 0.;
   Dh[10] = 0.;
   Dh[11] = 0.;
   Dh[12] = 0.;
   Dh[13] = 0.;
   Dh[14] = 0.;
   Dh[15] = 0.;
   Dh[16] = 0.;

   //with respect to x[1]
   Dh[0+17] = 0.;
   Dh[1+17] = 1.;
   Dh[2+17] = 0.;
   Dh[3+17] = 0.;
   Dh[4+17] = 0.;
   Dh[5+17] = 0.;
   Dh[6+17] = 0.;
   Dh[7+17] = 0.;
   Dh[8+17] = 0.;
   Dh[9+17] = 0.;
   Dh[10+17] = 0.;
   Dh[11+17] = 0.;
   Dh[12+17] = 0.;
   Dh[13+17] = 0.;
   Dh[14+17] = 0.;
   Dh[15+17] = 0.;
   Dh[16+17] = 0.;


   //with respect to x[2]
   Dh[0+17*2] = 0.;
   Dh[1+17*2] = 0.;
   Dh[2+17*2] = 1.;
   Dh[3+17*2] = 0.;
   Dh[4+17*2] = 0.;
   Dh[5+17*2] = 0.;
   Dh[6+17*2] = 0.;
   Dh[7+17*2] = 0.;
   Dh[8+17*2] = 0.;
   Dh[9+17*2] = 0.;
   Dh[10+17*2] = 0.;
   Dh[11+17*2] = 0.;
   Dh[12+17*2] = 0.;
   Dh[13+17*2] = 0.;
   Dh[14+17*2] = 0.;
   Dh[15+17*2] = 0.;
   Dh[16+17*2] = 0.;

   //with respect to x[3]
   Dh[0+17*3] = 0.;
   Dh[1+17*3] = 0.;
   Dh[2+17*3] = 0.;
   Dh[3+17*3] = 1.;
   Dh[4+17*3] = 0.;
   Dh[5+17*3] = 0.;
   Dh[6+17*3] = 0.;
   Dh[7+17*3] = 0.;
   Dh[8+17*3] = 0.;
   Dh[9+17*3] = 0.;
   Dh[10+17*3] = 0.;
   Dh[11+17*3] = 0.;
   Dh[12+17*3] = 0.;
   Dh[13+17*3] = 0.;
   Dh[14+17*3] = 0.;
   Dh[15+17*3] = 0.;
   Dh[16+17*3] = 0.;
   //with respect to x[4]
   Dh[0+17*4] = 0.;
   Dh[1+17*4] = 0.;
   Dh[2+17*4] = 0.;
   Dh[3+17*4] = 0.;
   Dh[4+17*4] = 1.;
   Dh[5+17*4] = 0.;
   Dh[6+17*4] = 0.;
   Dh[7+17*4] = 0.;
   Dh[8+17*4] = 0.;
   Dh[9+17*4] = 0.;
   Dh[10+17*4] = 0.;
   Dh[11+17*4] = 0.;
   Dh[12+17*4] = 0.;
   Dh[13+17*4] = 0.;
   Dh[14+17*4] = 0.;
   Dh[15+17*4] = 0.;
   Dh[16+17*4] = 0.;

   //with respect to x[5]
   Dh[0+17*5] = 0.;
   Dh[1+17*5] = 0.;
   Dh[2+17*5] = 0.;
   Dh[3+17*5] = 0.;
   Dh[4+17*5] = 0.;
   Dh[5+17*5] = 1.;
   Dh[6+17*5] = 0.;
   Dh[7+17*5] = 0.;
   Dh[8+17*5] = 0.;
   Dh[9+17*5] = 0.;
   Dh[10+17*5] = 0.;
   Dh[11+17*5] = 0.;
   Dh[12+17*5] = 0.;
   Dh[13+17*5] = 0.;
   Dh[14+17*5] = 0.;
   Dh[15+17*5] = 0.;
   Dh[16+17*5] = 0.;

   //with respect to x[6]
   Dh[0+17*6] = 0.;
   Dh[1+17*6] = 0.;
   Dh[2+17*6] = 0.;
   Dh[3+17*6] = 0.;
   Dh[4+17*6] = 0.;
   Dh[5+17*6] = 0.;
   Dh[6+17*6] = 1.;
   Dh[7+17*6] = 0.;
   Dh[8+17*6] = 0.;
   Dh[9+17*6] = 0.;
   Dh[10+17*6] = 0.;
   Dh[11+17*6] = 0.;
   Dh[12+17*6] = 0.;
   Dh[13+17*6] = 0.;
   Dh[14+17*6] = 0.;
   Dh[15+17*6] = 0.;
   Dh[16+17*6] = 0.;

   //with respect to x[7]
   Dh[0+17*7] = 0.;
   Dh[1+17*7] = 0.;
   Dh[2+17*7] = 0.;
   Dh[3+17*7] = 0.;
   Dh[4+17*7] = 0.;
   Dh[5+17*7] = 0.;
   Dh[6+17*7] = 0.;
   Dh[7+17*7] = 1.;
   Dh[8+17*7] = 0.;
   Dh[9+17*7] = 0.;
   Dh[10+17*7] = 0.;
   Dh[11+17*7] = 0.;
   Dh[12+17*7] = 0.;
   Dh[13+17*7] = 0.;
   Dh[14+17*7] = 0.;
   Dh[15+17*7] = 0.;
   Dh[16+17*7] = 0.;

   //with respect to x[8]
   Dh[0+17*8] = 0.;
   Dh[1+17*8] = 0.;
   Dh[2+17*8] = 0.;
   Dh[3+17*8] = 0.;
   Dh[4+17*8] = 0.;
   Dh[5+17*8] = 0.;
   Dh[6+17*8] = 0.;
   Dh[7+17*8] = 0.;
   Dh[8+17*8] = 1.;
   Dh[9+17*8] = 0.;
   Dh[10+17*8] = 0.;
   Dh[11+17*8] = 0.;
   Dh[12+17*8] = 0.;
   Dh[13+17*8] = 0.;
   Dh[14+17*8] = 0.;
   Dh[15+17*8] = 0.;
   Dh[16+17*8] = 0.;

   //with respect to x[9]
   Dh[0+17*9] = 0.;
   Dh[1+17*9] = 0.;
   Dh[2+17*9] = 0.;
   Dh[3+17*9] = 0.;
   Dh[4+17*9] = 0.;
   Dh[5+17*9] = 0.;
   Dh[6+17*9] = 0.;
   Dh[7+17*9] = 0.;
   Dh[8+17*9] = 0.;
   Dh[9+17*9] = 1.;
   Dh[10+17*9] = 0.;
   Dh[11+17*9] = 0.;
   Dh[12+17*9] = 0.;
   Dh[13+17*9] = 0.;
   Dh[14+17*9] = 0.;
   Dh[15+17*9] = 0.;
   Dh[16+17*9] = 0.;

   //with respect to x[10]
   Dh[0+17*10] = 0.;
   Dh[1+17*10] = 0.;
   Dh[2+17*10] = 0.;
   Dh[3+17*10] = 0.;
   Dh[4+17*10] = 0.;
   Dh[5+17*10] = 0.;
   Dh[6+17*10] = 0.;
   Dh[7+17*10] = 0.;
   Dh[8+17*10] = 0.;
   Dh[9+17*10] = 0.;
   Dh[10+17*10] = 1.;
   Dh[11+17*10] = 0.;
   Dh[12+17*10] = 0.;
   Dh[13+17*10] = 0.;
   Dh[14+17*10] = 0.;
   Dh[15+17*10] = 0.;
   Dh[16+17*10] = 0.;

   //with respect to x[11]
   Dh[0+17*11] = 0.;
   Dh[1+17*11] = 0.;
   Dh[2+17*11] = 0.;
   Dh[3+17*11] = 0.;
   Dh[4+17*11] = 0.;
   Dh[5+17*11] = 0.;
   Dh[6+17*11] = 0.;
   Dh[7+17*11] = 0.;
   Dh[8+17*11] = 0.;
   Dh[9+17*11] = 0.;
   Dh[10+17*11] = 0.;
   Dh[11+17*11] = 1.;
   Dh[12+17*11] = 0.;
   Dh[13+17*11] = 0.;
   Dh[14+17*11] = 0.;
   Dh[15+17*11] = 0.;
   Dh[16+17*11] = 0.;

   //with respect to x[12]
   Dh[0+17*12] = 0.;
   Dh[1+17*12] = 0.;
   Dh[2+17*12] = 0.;
   Dh[3+17*12] = 0.;
   Dh[4+17*12] = 0.;
   Dh[5+17*12] = 0.;
   Dh[6+17*12] = 0.;
   Dh[7+17*12] = 0.;
   Dh[8+17*12] = 0.;
   Dh[9+17*12] = 0.;
   Dh[10+17*12] = 0.;
   Dh[11+17*12] = 0.;
   Dh[12+17*12] = 1.;
   Dh[13+17*12] = 0.;
   Dh[14+17*12] = 0.;
   Dh[15+17*12] = 0.;
   Dh[16+17*12] = 0.;

   Dh[0+17*13] = 0.;
   Dh[1+17*13] = 0.;
   Dh[2+17*13] = 0.;
   Dh[3+17*13] = 0.;
   Dh[4+17*13] = 0.;
   Dh[5+17*13] = 0.;
   Dh[6+17*13] = 0.;
   Dh[7+17*13] = 0.;
   Dh[8+17*13] = 0.;
   Dh[9+17*13] = 0.;
   Dh[10+17*13] = 0.;
   Dh[11+17*13] = 0.;
   Dh[12+17*13] = 0.;
   Dh[13+17*13] = 1.;
   Dh[14+17*13] = 0.;
   Dh[15+17*13] = 0.;
   Dh[16+17*13] = 0.;


   Dh[0+17*14] = 0.;
   Dh[1+17*14] = 0.;
   Dh[2+17*14] = 0.;
   Dh[3+17*14] = 0.;
   Dh[4+17*14] = 0.;
   Dh[5+17*14] = 0.;
   Dh[6+17*14] = 0.;
   Dh[7+17*14] = 0.;
   Dh[8+17*14] = 0.;
   Dh[9+17*14] = 0.;
   Dh[10+17*14] = 0.;
   Dh[11+17*14] = 0.;
   Dh[12+17*14] = 0.;
   Dh[13+17*14] = 0.;
   Dh[14+17*14] = 1.;
   Dh[15+17*14] = 0.;
   Dh[16+17*14] = 0.;



   Dh[0+17*15] = 0.;
   Dh[1+17*15] = 0.;
   Dh[2+17*15] = 0.;
   Dh[3+17*15] = 0.;
   Dh[4+17*15] = 0.;
   Dh[5+17*15] = 0.;
   Dh[6+17*15] = 0.;
   Dh[7+17*15] = 0.;
   Dh[8+17*15] = 0.;
   Dh[9+17*15] = 0.;
   Dh[10+17*15] = 0.;
   Dh[11+17*15] = 0.;
   Dh[12+17*15] = 0.;
   Dh[13+17*15] = 0.;
   Dh[14+17*15] = 0.;
   Dh[15+17*15] = 1.;
   Dh[16+17*15] = 0.;



   Dh[0+17*16] = 0.;
   Dh[1+17*16] = 0.;
   Dh[2+17*16] = 0.;
   Dh[3+17*16] = 0.;
   Dh[4+17*16] = 0.;
   Dh[5+17*16] = 0.;
   Dh[6+17*16] = 0.;
   Dh[7+17*16] = 0.;
   Dh[8+17*16] = 0.;
   Dh[9+17*16] = 0.;
   Dh[10+17*16] = 0.;
   Dh[11+17*16] = 0.;
   Dh[12+17*16] = 0.;
   Dh[13+17*16] = 0.;
   Dh[14+17*16] = 0.;
   Dh[15+17*16] = 0.;
   Dh[16+17*16] = 1.;





  

}

#endif
