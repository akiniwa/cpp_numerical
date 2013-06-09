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
    FILE *fp2;
    if (argc <= 1) {
        printf("not enough argument\n");
        exit(0);
    }
    if ((fp = fopen(argv[1], "w")) == NULL) {
        printf("file cannot open\n");
        exit(0);
    }
    if ((fp2 = fopen(argv[2], "w")) == NULL) {
        printf("file cannot open\n");
        exit(0);
    }

    Satellite* debris;
    debris = new Satellite();
    debris->COEs(M_PI*0.4,M_PI*0.3, M_PI*.025);
//  debris->COEs(0.0, 0.0, 0.0);

    bool rotate = true;
    double T;
    for( T=0.0 ; T<10.00 ; T += debris->dT ){
        debris->motion();

        double r = hypot(hypot(debris->qx, debris->qy), debris->qz);
        double p = hypot(hypot(debris->px, debris->py), debris->pz);
        //printf("%f\n", 0.5*pow(p, 2) - debris->GM/r);
        double q = hypot(debris->qz , debris->qx);
        double theta = atan2(debris->qy, q) * 180.0 / M_PI;
        fprintf(fp, "%f %f %f\n", debris->getCoordinate(Satellite::X),debris->getCoordinate(Satellite::Y), debris->getCoordinate(Satellite::Z));
        //fprintf(fp, "%f %f %f\n", theta+rotate*180.0, p, 0.0);
        //if (abs(theta) > 89.0) {
         //   rotate = !rotate;
        //}
        if (rotate==true) {
            fprintf(fp2, "%f %f %f\n", theta, p, 0.0);
        } else {
            fprintf(fp2, "%f %f %f\n", 180-theta, p, 0.0);
        }
     }
    fclose(fp);
    glutInit(&argc, argv);
    openGL();
}
