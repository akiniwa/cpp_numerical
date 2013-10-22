double calc(double x, double y, double z) {
    double sunR = pow(((nonDsunL-x)*(nonDsunL-x) + y*y + z*z), 0.5);
    double r = hypot(hypot(x, y), z);
    double accelaration = (-1/pow(r, 3) + (-nonDsunM/pow(sunR, 3)))*x;
    return accelaration;
}

double calcXY_J2(double x, double y, double z) {
    double sunR = pow(((nonDsunL-x)*(nonDsunL-x) + y*y + z*z), 0.5);
    double r = hypot(hypot(x, y), z);
    double accelaration = ( (-1/pow(r, 3)) + J2*( (-7.5*pow(z, 2)/pow(r, 7)) + (1.5/pow(r, 5)) ) + (-nonDsunM/pow(sunR, 3)) )*x;
    return accelaration;
} 

double calcZ_J2(double x, double y, double z) {
    double sunR = pow(((nonDsunL-x)*(nonDsunL-x) + y*y + z*z), 0.5);
    double r = hypot(hypot(x, y), z);
    double accelaration = ( (-1/pow(r, 3)) + J2*( (3/pow(r, 5)) - (7.5*pow(x, 2)/pow(r, 7)) + (1.5/pow(r, 5)) ) + (-nonDsunM/pow(sunR, 3)) )*x;
    return accelaration;
}

// 二次のシンプレクティック
void symp2(Particle* pa, double t) {
    pa->px+=calc(pa->qx, pa->qy, pa->qz) *0.5*t;
    pa->py+=calc(pa->qy, pa->qz, pa->qx) *0.5*t;
    pa->pz+=calc(pa->qz, pa->qx, pa->qy) *0.5*t;

    pa->qx += pa->px/m*t;
    pa->qy += pa->py/m*t;
    pa->qz += pa->pz/m*t;

    pa->px += calc(pa->qx, pa->qy, pa->qz) *0.5*t;
    pa->py += calc(pa->qy, pa->qz, pa->qx) *0.5*t;
    pa->pz += calc(pa->qz, pa->qx, pa->qy) *0.5*t;
}

// J2
void symp2_for_J2(Particle* pa, double t) {
    pa->px+=calcXY_J2(pa->qx, pa->qy, pa->qz) *0.5*t;
    pa->py+=calcXY_J2(pa->qy, pa->qx, pa->qz) *0.5*t;
    pa->pz+=calcZ_J2(pa->qz, pa->qx, pa->qy) *0.5*t;

    pa->qx += pa->px/m*t;
    pa->qy += pa->py/m*t;
    pa->qz += pa->pz/m*t;

    pa->px+=calcXY_J2(pa->qx, pa->qy, pa->qz) *0.5*t;
    pa->py+=calcXY_J2(pa->qy, pa->qx, pa->qz) *0.5*t;
    pa->pz+=calcZ_J2(pa->qz, pa->qx, pa->qy) *0.5*t;
}
