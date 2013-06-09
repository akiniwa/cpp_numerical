#include "satellite.hpp"
#include <stdio.h>

const double Satellite::root_AC = 0.007674;
const double Satellite::theta_dot = 1.0;

Satellite::Satellite() {
    qx = 1.2;
    qy = 3.5;
    qz = 0;

    px = 1.5;
    py = 2.0;
    pz = 0;

    dT = 0.00001;

    p_dot = 0.0;
    q_dot = 0.0;
}

double Satellite::RungeKuttaDQ(double p) {
    return root_AC*dT;
}

double Satellite::RungeKuttaDP(double q) {
    return theta_dot*dT;
}

double Satellite::getCoordinate(int axis) {
    // printf("%f, %f\n", qx, px);
    p_dot = sqrt(2*qx)*cos(px);
    q_dot = sqrt(2*qx)*sin(px);
    switch (axis) {
        case X:
            return (p_dot*cos(px) + q_dot*sin(px));
            break;
        case Y:
            return (-p_dot*sin(px) + q_dot*cos(px));
            break;
        case Z:
            return qz;
            break;
        default:
            return 0;
            break;
    }
}
