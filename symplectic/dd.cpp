#include <iostream>
#include <math.h>
#include <fstream>

const double GM = 4*M_PI*M_PI;
const double M = 1;
const double dT = 0.0001;
const int sympI = 4;

const double symp_k[4] = {
    +0.67560359597982888591,
    -0.17560359597982888591,
    -0.17560359597982888591,
    +0.67560359597982888591
};

const double symp_u[4] = {
    +0.00000000000000000000,
    +1.35120719195965777182,
    -1.70241438391931554364,
    +1.35120719195965777182
};

struct Particle {
    double px, py;
    double qx, qy;
    Particle() {
        px = 0.0;
        py = 0.75*sqrt(2*GM);
        qx = 1.2;
        qy = 0.0;
    }
};

double calc(double x, double y) {
    double r = hypot(x, y);
    //accelaration = (-GM/(r*r*r) + 3*GM*r*r/(r*r*r)*J2*(1.5*pow(qz/q, 2)-0.5))*x;
    return -GM*x/(r*r*r);
    //return -x;
}

void symp2(Particle* pa, double t) {
    pa->px+=calc(pa->qx, pa->qy) *0.5*t;
    pa->py+=calc(pa->qy, pa->qx) *0.5*t;

    pa->qx+=pa->px/M*t;
    pa->qy+=pa->py/M*t;

    pa->px+=calc(pa->qx, pa->qy) *0.5*t;
    pa->py+=calc(pa->qy, pa->qx) *0.5*t;

    //pa->qx+=1.0;
    //pa->qy+=1.0;
}

double calcE(Particle pa) {
    double q = hypot(pa.qx, pa.qy);
    double p = hypot(pa.px, pa.py);
    double K = 0.5/M*pow(q, 2);
    double U = - GM*M/q;
    return K + U;
}

double a_length(Particle pa) {
    double q = hypot(pa.qx, pa.qy);
    double p = hypot(pa.px, pa.py);
    return q*GM/(2*GM-pow(p, 2)*q);
}

int main() {
    Particle particle;
    std::ofstream ofs;
    ofs.open( "kep" );
    for (int i=0;i<1000000;i++) {
            double x = particle.qx;
            double y = particle.qy;
//            for (int s=0;i<sympI;i++) {
                symp2(&particle, dT);
                /*
                particle.px+=calc(x, y) *symp_u[i]* dT;
                particle.py+=calc(y, x) *symp_u[i]* dT;

                particle.qx+=particle.px/M*symp_k[i]* dT;
                particle.qy+=particle.py/M*symp_k[i]* dT;
                */
 //           }
            //   std::cout << "px = " << particle.px << ", py = " << particle.py << std::endl;
            //        std::cout << "qx = " << particle.qx << ", px = " << particle.px << std::endl;

        if (i%500==0) {
            ofs << particle.qx << " " << particle.qy << " " << 0.0 << " " << calcE(particle) << " " << a_length(particle) << std::endl;
        }
     }
    ofs.close();
}
