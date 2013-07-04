#include "satellite.hpp"

//const double Satellite::GM = 4*M_PI*M_PI;
const double Satellite::GM = 1.0;
const double Satellite::M = 1.0;
const double Satellite::totalE = 1.0;
const double Satellite::J2 = 1082.264E-6;
const double aE = 1.0;

Satellite::Satellite() {
    qx = 0.1;
    qy = 0.1;
    qz = 1.0;

    px = 1.0;
    py = 0.5;
    pz = 0.1;

    accelaration = 0.0;
    dT = 0.001;
}

double Satellite::RungeKuttaDQ(double lp) {
    double coordinate = lp*dT;
    return coordinate;
}

double Satellite::RungeKuttaDP(double lqx, double lqy, double lqz) {
    double r   = hypot(hypot( lqx, lqy ), lqz);
    accelaration = GM*(-1/(pow(r, 3)) - 7.5*pow(aE, 2)*pow(lqz, 2)*J2/(pow(r, 7)) + 1.5*J2*pow(aE, 2)/pow(r, 5))*lqx;
    //accelaration = -GM*lqx/pow(r, 3);
    double momemtum = accelaration*dT;
    return momemtum;
}

double Satellite::RungeKuttaDPZ(double lqx, double lqy, double lqz) {
    double r   = hypot(hypot( lqx, lqy ), lqz);
    accelaration = GM*(-1*lqx/(pow(r, 3)) + J2*pow(aE, 2)*(3*lqx/(pow(r, 5)) - 7.5*pow(lqx, 3)/pow(r, 7) + 1.5*lqx/pow(r, 5)));
    //accelaration = -GM*lqx/pow(r, 3);
    double momemtum = accelaration*dT;
    return momemtum;
}

double Satellite::getKineticE() {
    double p = hypot(hypot(px, py), pz);
    return 0.5*pow(p, 2);
}

double Satellite::getPotentialE() {
    double q = hypot(hypot(qx, qy), qz);
    double R = -GM*pow(aE,2)*J2/pow(q,3)*(1.5*pow(qz/q,2)-0.5);
    // double R = 0;

    return -GM*M/q + R;
}
