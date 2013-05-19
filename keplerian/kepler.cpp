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

#include <math.h>
#include "openGLsample.hpp"
#include "satellite.hpp"
//---- main function
int main(int argc, char *argv[])
{
    FILE *fp;
    if (argc <= 1) {
        printf("not enough argument\n");
        exit(0);
    }
    if ((fp = fopen(argv[1], "w")) == NULL) {
        printf("file cannot open\n");
        exit(0);
    }

    Satellite* debris;
    debris = new Satellite();

    double T;
    for( T=0.0 ; T<10.00 ; T += debris->dT ){
        debris->motion();
//        debris->COEs(M_PI*0.2, M_PI*0.1, M_PI*.025);
        debris->COEs(M_PI*0.5, M_PI*0.5, 0);
        fprintf(fp, "%f %f %f\n", debris->getCoordinate(Satellite::X), debris->getCoordinate(Satellite::Y), debris->getCoordinate(Satellite::Z));
   }
   fclose(fp);
   glutInit(&argc, argv);
   openGL();
}
