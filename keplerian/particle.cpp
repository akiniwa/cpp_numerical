#include "particle.hpp"

Particle::Particle() {
    qx = 0;
    qy = 0;
    qz = 0;

    px = 0;
    py = 0;
    pz = 0;

    dT = 0.01;
    rungeIndex = 0;
}

void Particle::RungeKutta( double qx, double qy, double qz, double px, double py, double pz ) {
    qxk[rungeIndex] = RungeKuttaDQ(px);
    qyk[rungeIndex] = RungeKuttaDQ(py); 
    qzk[rungeIndex] = RungeKuttaDQ(pz);

    pxk[rungeIndex] = RungeKuttaDP(qx);
    pyk[rungeIndex] = RungeKuttaDP(qy);
    pzk[rungeIndex] = RungeKuttaDP(qz);
}

void Particle::motion() {
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
