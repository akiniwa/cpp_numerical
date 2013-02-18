/*
 * =====================================================================================
 *
 *       Filename:  paraboloidOrbid.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013/02/17 20時14分37秒
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
#include "openGLsample.hpp"

//---- physical setting
#define G          (9.80665)
#define M          (1.0)
#define dT         (1.0/256)

//---- graphic setting
#define WIN_WIDTH  (256)
#define WIN_HEIGHT (256)

//---- main function
int main(int argc, char *argv[])
{

  double qx=0.0, qy=0.0;
  double px=1.0, py=2.0;
  FILE *fp;

  if (argc <= 1) {
		  printf("not enough argument\n");
		  exit(0);
  }
  if ((fp = fopen(argv[1], "w")) == NULL) {
		  printf("file cannot open\n");
		  exit(0);
  }

  do{
    qx += px/M*dT;
    qy += py/M*dT;
    px += M*0*dT;
    py += -M*G*dT;
	fprintf(fp, "%f %f %f %f\n",qx,qy,px,py);
}while( qy > 0.0 );
	fclose(fp);

	// call openGL with filename
	/*  
    FILE *fp2 = popen("/Applications/gnuplot.app/gnuplot -persist", "w");
    fprintf(fp2, "plot \"%s\"\n", argv[1]);
	fclose(fp2);
	*/
	glutInit(&argc, argv);
	openGL();
}
