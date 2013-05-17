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
const float dT = 1/16.0;
const float m = 3.0;

//---- graphic setting
#define WIN_WIDTH  (256)
#define WIN_HEIGHT (256)

void RungeKutta( double  qx, double  px, double& qxk, double& pxk)
{
  qxk = px * dT;
  pxk = (6/m * (2/pow(qx, 13) - 1/pow(qx, 7))) * dT;
}

//---- main function
int main(int argc, char *argv[])
{
  double qx=3.9;
  double px=-0.2;

  double T;
  double qxk[4], pxk[4];
  FILE *fp;

  if (argc <= 1) {
		  printf("not enough argument\n");
		  exit(0);
  }
  if ((fp = fopen(argv[1], "w")) == NULL) {
		  printf("file cannot open\n");
		  exit(0);
  }

  for( T=0.0 ; T<30.00 ; T += dT ){
    RungeKutta( qx, px, qxk[0], pxk[0]);
    RungeKutta( qx+0.5*qxk[0], px+0.5*pxk[0], qxk[1], pxk[1]);
    RungeKutta( qx+0.5*qxk[1], px+0.5*pxk[1], qxk[2], pxk[2]);
    RungeKutta( qx+qxk[2], px+pxk[2], qxk[3], pxk[3]);

    qx += (qxk[0] +  2*qxk[1] + 2*qxk[2] + qxk[3])*(1.0/6);
    px += (pxk[0] +  2*pxk[1] + 2*pxk[2] + pxk[3])*(1.0/6);

	fprintf(fp, "%f %f %f\n", qx, 0.0, px);
  }

  fclose(fp);

  glutInit(&argc, argv);
  openGL();
}
