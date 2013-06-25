#include <iostream>
#include <math.h>
#include <fstream>

const double GM = 4*M_PI*M_PI;
const double M = 2;
const double dT = 0.0001;
const int sympI = 3;
const double J2 = 1082.264E-6;

const double symp[3] = {
    +1.35120719195965777182,
    -1.70241438391931554364,
    +1.35120719195965777182
};

struct Particle {
    double px, py, pz;
    double qx, qy, qz;
    Particle() {
        px = 0.0;
        py = 0.6*sqrt(2*GM);
        pz = 0.0;
        
        qx = 1.0;
        qy = 0.2;
        qz = 0.3;
    }
};

Particle particle;

double calc(double x, double y, double z) {
    double r = hypot(hypot(x, y), z);
//  double accelaration = (-GM/(r*r*r) + 3*GM*r*r/(r*r*r)*J2*(1.5*pow(particle.qz/r, 2)-0.5))*x;
    double accelaration = -GM*x/(r*r*r);
    return accelaration;
}

void symp2(Particle* pa, double t) {
    pa->px+=calc(pa->qx, pa->qy, pa->qz) *0.5*t;
    pa->py+=calc(pa->qy, pa->qz, pa->qx) *0.5*t;
    pa->pz+=calc(pa->qz, pa->qx, pa->qy) *0.5*t;

    pa->qx+=pa->px/M*t;
    pa->qy+=pa->py/M*t;
    pa->qz+=pa->pz/M*t;

    pa->px+=calc(pa->qx, pa->qy, pa->qz) *0.5*t;
    pa->py+=calc(pa->qy, pa->qz, pa->qx) *0.5*t;
    pa->pz+=calc(pa->qz, pa->qx, pa->qy) *0.5*t;
}

double calcE(Particle pa) {
    double q = hypot(hypot(pa.qx, pa.qy), pa.qz);
    double p = hypot(hypot(pa.px, pa.py), pa.pz);
    double K = 0.5/M*pow(p, 2);
    double U = - GM*M/q;
    return K + U;
}

double a_length(Particle pa) {
    double q = hypot(hypot(pa.qx, pa.qy), pa.qz);
    double p = hypot(hypot(pa.px, pa.py), pa.pz);
    return q*GM/(2*GM-pow(p, 2)*q);
}

/*
double h_moment() {
    double lxy = 1/M
}
*/

int main() {
    std::ofstream ofs;
    ofs.open( "kep" );
    for (int i=0;i<1000000;i++) {
            for (int s=0;s<sympI;s++) {
                symp2(&particle, dT*symp[s]);
                //std::cout << symp[s] << std::endl;
    //            symp2(&particle, dT*1.7);
                /*
                particle.px+=calc(x, y) *symp_u[i]* dT;
                particle.py+=calc(y, x) *symp_u[i]* dT;

                particle.qx+=particle.px/M*symp_k[i]* dT;
                particle.qy+=particle.py/M*symp_k[i]* dT;
                */
           }
            //   std::cout << "px = " << particle.px << ", py = " << particle.py << std::endl;
            //        std::cout << "qx = " << particle.qx << ", px = " << particle.px << std::endl;

        if (i%500==0) {
            ofs << particle.qx << " " << particle.qy << " " << particle.qz << " " << calcE(particle) << " " << a_length(particle) << std::endl;
        }
     }
    ofs.close();
}
