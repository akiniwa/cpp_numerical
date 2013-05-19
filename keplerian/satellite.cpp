#include "satellite.hpp"

const double Satellite::GM = 4*M_PI*M_PI*10;
const double Satellite::M = 1.0;

Satellite::Satellite() {
    qx = 1.2;
    qy = 0;
    qz = 0;

    px = 0;
    py = sqrt(GM)*M;
    pz = 0;

    dT = 0.005;
}

double Satellite::RungeKuttaDQ(double lp) {
    double coordinate = lp/M*dT;
    return coordinate;
}

double Satellite::RungeKuttaDP(double lq) {
    double q, qi3;

    q   = hypot(hypot( qx, qy ), qz);
    qi3 = 1.0/(q*q*q);

    double momemtum = -GM*M*lq*qi3*dT;
    return momemtum;
}

double Satellite::rotate(int axis) {
    double l_qx, l_qy, l_qz;
    double i_qx, i_qy, i_qz;

    /* rotate Omega */
    l_qx = (cos(omega)*qx - sin(omega)*qy);
    l_qy = (sin(omega)*qx + cos(omega)*qy);
    l_qz = pz;

    /* rotate inclination */
    i_qx = l_qx;
    i_qy = (cos(inclination)*l_qy - sin(inclination)*l_qz);
    i_qz = (sin(inclination)*l_qy + cos(inclination)*l_qz);

    /* rotate omegaSmall */


    switch (axis) {
        case X:
            return i_qx;
            break;
        case Y:
            return i_qy;
            break;
        case Z:
            return i_qz;
            break;
        default:
            return 0;
            break;
    }
}

void Satellite::COEs(double O, double i, double o) {
    omega = O;
    inclination = i;
    omegaSmall = o;
}

double Satellite::getCoordinate(int axis) {
    return rotate(axis);
}
