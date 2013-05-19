#include "satellite.hpp"

const double Satellite::GM = 4*M_PI*M_PI;
const double Satellite::M = 1.0;

Satellite::Satellite() {
    qx = 1.7;
    qy = 0;
    qz = 0;

    px = 0;
    py = sqrt(GM)*M;
    pz = 0;

    dT = 0.01;
}

double Satellite::RungeKuttaDQ(double lp) {
    double coodinate = lp/M*dT;
    return coodinate;
}

double Satellite::RungeKuttaDP(double lq) {
    double q, qi3;

    q   = hypot(hypot( qx, qy ), qz);
    qi3 = 1.0/(q*q*q);

    double momemtum = -GM*M*lq*qi3*dT;
    return momemtum;
}


