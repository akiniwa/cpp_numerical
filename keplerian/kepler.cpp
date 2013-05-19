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
#define GM         (4*M_PI*M_PI*10)
#define M          (1.0)
#define dT         (1.0/256)

//---- graphic setting
#define WIN_WIDTH  (256)
#define WIN_HEIGHT (256)

class Satellite {
    public:
        Satellite();

        double qx;
        double qy;
        double qz;

        double px;
        double py;
        double pz;
        double qxk[4], qyk[4], qzk[4], pxk[4], pyk[4], pzk[4];

        void motion();
        void RungeKutta();
        double RungeKuttaDQ();
        double RungeKuttaDP();
        int rungeIndex;
};

Satellite::Satellite() {
    qx = 0;
    qy = 0;
    qz = 0;

    px = 0;
    py = 0;
    pz = 0;

    rungeIndex = 0;
}

void Satellite::motion() {
    rungeIndex = 0;
    RungeKutta( qx, qy, qz, px, py, pz );
    rungeIndex = 1;
    RungeKutta( qx+0.5*qxk[0], qy+0.5*qyk[0], qz+0.5*qzk[0], px+0.5*pxk[0], py+0.5*pyk[0], pz+0.5*pzk[0] );
    rungeIndex = 2;
    RungeKutta( qx+0.5*qxk[1], qy+0.5*qyk[1], qz+0.5*qzk[1], px+0.5*pxk[1], py+0.5*pyk[1], pz+0.5*pzk[1] );
    rungeIndex = 3;
    RungeKutta( qx+qxk[2], qy+qyk[2], qz+qzk[2], px+pxk[2], py+pyk[2], pz+pzk[2] );

    qx += (qxk[0] +  2*qxk[1] + 2*qxk[2] + qxk[3])*(1.0/6);
    qy += (qyk[0] +  2*qyk[1] + 2*qyk[2] + qyk[3])*(1.0/6);
    qz += (qzk[0] +  2*qzk[1] + 2*qzk[2] + qzk[3])*(1.0/6);

    px += (pxk[0] +  2*pxk[1] + 2*pxk[2] + pxk[3])*(1.0/6);
    py += (pyk[0] +  2*pyk[1] + 2*pyk[2] + pyk[3])*(1.0/6);
    pz += (pzk[0] +  2*pzk[1] + 2*pzk[2] + pzk[3])*(1.0/6);
}

double Satellite::RungeKuttaDQ(double lp) {
    double coodinate = lp/M*dT;
    return coodinate;
}

double Satellite::RungeKuttaDP(double lq) {
    double q, qi3;

    q   = hypot(hypot( qx, qy ), qz);
    qi3 = 1.0/(q*q*q);

    double momemtum = -GM*M*lq*qi3*dT
    return momemtum;
}

void Satellite::RungeKutta( double qx, double qy, double qz, double px, double py, double pz ) {
    qxk[rungeIndex] = RungeKuttaDQ(px);
    qyk[rungeIndex] = RungeKuttaDQ(py); 
    qzk[rungeIndex] = RungeKuttaDQ(pz);

    pxk[rungeIndex] = RungeKuttaDP(qx);
    pyk[rungeIndex] = RungeKuttaDP(qy);
    pzk[rungeIndex] = RungeKuttaDP(qz);
}
/*
void RungeKutta( double qx, double qy, double qz, double px, double py, double pz,
        double& qxk, double& qyk, double& qzk, double& pxk, double& pyk, double& pzk ) {
    double q, qi3;
    q   = hypot(hypot( qx, qy ), qz);
    qi3 = 1.0/(q*q*q);

    qxk = px/M*dT;
    qyk = py/M*dT;
    qzk = pz/M*dT;

    pxk = -GM*M*qx*qi3*dT;
    pyk = -GM*M*qy*qi3*dT;
    pzk = -GM*M*qz*qi3*dT;
}

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
*/
//---- main function
int main(int argc, char *argv[])
{
    Satellite* debris;
    debris = new Satellite();

    /*
    double qx=1.7, qy=0.0, qz=0.0;
    double px=0.0, py=sqrt(GM)*M, pz=0.0;

    double T;
    double qxk[4], qyk[4], qzk[4], pxk[4], pyk[4], pzk[4];
    */
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
        debris->motion();
        fprintf(fp, "%f %f %f", debris->qx, debris->qy, debris->qz);
        /*
        RungeKutta( qx, qy, qz, px, py, pz,
                qxk[0], qyk[0], qzk[0], pxk[0], pyk[0], pzk[0] );
        RungeKutta( qx+0.5*qxk[0], qy+0.5*qyk[0], qz+0.5*qzk[0], px+0.5*pxk[0], py+0.5*pyk[0], pz+0.5*pzk[0],
                qxk[1], qyk[1], qzk[1], pxk[1], pyk[1], pzk[1] );
        RungeKutta( qx+0.5*qxk[1], qy+0.5*qyk[1], qz+0.5*qzk[1], px+0.5*pxk[1], py+0.5*pyk[1], pz+0.5*pzk[1],
                qxk[2], qyk[2], qzk[2], pxk[2], pyk[2], pzk[2] );
        RungeKutta( qx+qxk[2], qy+qyk[2], qz+qzk[2], px+pxk[2], py+pyk[2], pz+pzk[2],
                qxk[3], qyk[3], qzk[3], pxk[3], pyk[3], pzk[3] );

        qx += (qxk[0] +  2*qxk[1] + 2*qxk[2] + qxk[3])*(1.0/6);
        qy += (qyk[0] +  2*qyk[1] + 2*qyk[2] + qyk[3])*(1.0/6);
        qz += (qzk[0] +  2*qzk[1] + 2*qzk[2] + qzk[3])*(1.0/6);

        px += (pxk[0] +  2*pxk[1] + 2*pxk[2] + pxk[3])*(1.0/6);
        py += (pyk[0] +  2*pyk[1] + 2*pyk[2] + pyk[3])*(1.0/6);
        pz += (pzk[0] +  2*pzk[1] + 2*pzk[2] + pzk[3])*(1.0/6);

        //fprintf(fp, "%f %f %f %f %f %f\n", qx, cos(M_PI*0.25)*qy-sin(M_PI*0.25)*qz, sin(M_PI*0.25)*qy+cos(M_PI*0.25)*qz, px, py, pz);
        //	fprintf(fp, "%f %f %f %f %f %f\n", cos(M_PI*0.25)*qx - sin(M_PI*0.25)*qy, sin(M_PI*0.25)*qx + cos(M_PI*0.25)*qy, qz, px, py, pz);
        fprintf(fp, "%f %f %f %f %f %f\n", cos(M_PI*0.25)*qx - sin(M_PI*0.25)*qz, qy, sin(M_PI*0.25)*qx + cos(M_PI*0.25)*qz, px, py, pz);
        //fprintf(fp, "%f %f %f %f %f %f\n",qx,qy,qz,px,py,pz);
        //	fprintf(fp, "%f %f %f %f\n",qx,qy,px,py);
        */
    }

    fclose(fp);
    glutInit(&argc, argv);
    openGL();
}
