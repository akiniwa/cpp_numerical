#include "satellite.hpp"

const double Satellite::GM = 4*M_PI*M_PI;
const double Satellite::M = 4*M_PI;
const double Satellite::totalE = 1.0;
const double Satellite::J2 = 1082.264E-6;
const double Satellite::eq_radius = 63;

Satellite::Satellite() {
    qx = 1.2;
    qy = 0.0;
    qz = 0.0;

    px = 0.0;
    py = 2.29*sqrt(2*GM);
    pz = 0.0;

    accelaration = 0.0;
    dT = 0.001;
}

double Satellite::RungeKuttaDQ(double lp) {
    double coordinate = lp/M*dT;
    return coordinate;
}

double Satellite::RungeKuttaDP(double lq) {
    double q, qi3, qi4;

    q   = hypot(hypot( qx, qy ), qz);
    qi3 = 1.0/(q*q*q);
    qi4 = qi3 / q;

    accelaration = (-GM*qi3 + 3*GM*q*q*qi4*J2*30*(1.5*pow(qz/q, 2)-0.5))*lq;
    //accelaration = -GM*lq*qi3;
    double momemtum = accelaration*dT;
    return momemtum;
//    return -lq*dT;
}

double Satellite::rotate(int axis) {
    double l_qx, l_qy, l_qz;
    double i_qx, i_qy, i_qz;
    /* rotate Omega */
    /*
    l_qx = (cos(omega)*qx - sin(omega)*qy);
    l_qy = (sin(omega)*qx + cos(omega)*qy);
    l_qz = pz;
    */
    i_qx = qx;
    i_qy = qy;
    i_qz = qz;
    /* rotate inclination */
    /*
    i_qx = l_qx;
    i_qy = (cos(inclination)*l_qy - sin(inclination)*l_qz);
    i_qz = (sin(inclination)*l_qy + cos(inclination)*l_qz);
    */
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
