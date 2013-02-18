/*
 * =====================================================================================
 *
 *       Filename:  kepler.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/02/18 00時09分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>

#include <stdio.h>
#include <iostream>
#include <string>

#include <math.h>
#include "openGLsample.hpp"


//---- physical setting
#define GM         (4*M_PI*M_PI)
#define M          (1.0)
#define dT         (1.0/256)

//---- graphic setting
#define WIN_WIDTH  (256)
#define WIN_HEIGHT (256)

void RungeKutta( double  qx,  double  qy,  double  px,  double  py,
                 double& qxk, double& qyk, double& pxk, double& pyk )
{
  double q, qi3;
  q   = hypot( qx, qy );
  qi3 = 1.0/(q*q*q);
  qxk = px/M*dT;
  qyk = py/M*dT;
  pxk = -GM*M*qx*qi3*dT;
  pyk = -GM*M*qy*qi3*dT;
}



//---- main function
int main(int argc, char *argv[])
{

  double qx=1.0, qy=0.0;
  double px=0.0, py=sqrt(GM)*M;

  double T;
  double qxk[4], qyk[4], pxk[4], pyk[4];
  FILE *fp;

  if (argc <= 1) {
		  printf("not enough argument\n");
		  exit(0);
  }
  if ((fp = fopen(argv[1], "w")) == NULL) {
		  printf("file cannot open\n");
		  exit(0);
  }

  for( T=0.0 ; T<10.00 ; T += dT ){
//    NXDrawPoint( WIN_WIDTH/2 + (int)(64*qx), WIN_HEIGHT/2 + (int)(64*qy) );
    RungeKutta( qx, qy, px, py,
                qxk[0], qyk[0], pxk[0], pyk[0] );
    RungeKutta( qx+0.5*qxk[0], qy+0.5*qyk[0], px+0.5*pxk[0], py+0.5*pyk[0],
                qxk[1], qyk[1], pxk[1], pyk[1] );
    RungeKutta( qx+0.5*qxk[1], qy+0.5*qyk[1], px+0.5*pxk[1], py+0.5*pyk[1],
                qxk[2], qyk[2], pxk[2], pyk[2] );
    RungeKutta( qx+qxk[2], qy+qyk[2], px+pxk[2], py+pyk[2],
                qxk[3], qyk[3], pxk[3], pyk[3] );

    qx += (qxk[0] +  2*qxk[1] + 2*qxk[2] + qxk[3])*(1.0/6);
    qy += (qyk[0] +  2*qyk[1] + 2*qyk[2] + qyk[3])*(1.0/6);
    px += (pxk[0] +  2*pxk[1] + 2*pxk[2] + pxk[3])*(1.0/6);
    py += (pyk[0] +  2*pyk[1] + 2*pyk[2] + pyk[3])*(1.0/6);

	fprintf(fp, "%f %f %f %f\n",qx,qy,px,py);
  }

  fclose(fp);
  glutInit(&argc, argv);
  openGL();
}
