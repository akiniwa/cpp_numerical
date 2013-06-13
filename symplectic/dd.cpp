#include <iostream>
#include <math.h>
#include <fstream>

const double GM = 4*M_PI*M_PI;
const double M = 4*M_PI;
const double dT = 0.001;

struct Particle {
    double px, py;
    double qx, qy;
    Particle() {
        px = 0.0;
        py = 2.29*sqrt(2*GM);
        qx = 1.2;
        qy = 0.0;
    }
};

double calc(double x, double y) {
    double r = hypot(x, y);
    return -GM*x/(r*r*r);
    //return -x;
}

double calcE(Particle pa) {

    double q = hypot(pa.qx, pa.qy);
    double p = hypot(pa.px, pa.py);
    double K = 0.5/M*pow(q, 2);
    double U = - GM*M/q;
    return K + U;
}

int main() {
    Particle particle;
    std::ofstream ofs;
    ofs.open( "kep" );
    for (int i=0;i<100000000;i++) {
            double x = particle.qx;
            double y = particle.qy;
            particle.px+=calc(x, y) * dT;
            particle.py+=calc(y, x) * dT;

            particle.qx+=particle.px/M*dT;
            particle.qy+=particle.py/M*dT;

            //   std::cout << "px = " << particle.px << ", py = " << particle.py << std::endl;
            //        std::cout << "qx = " << particle.qx << ", px = " << particle.px << std::endl;

        if (i%500==0) {
            ofs << particle.qx << " " << particle.qy << " " << 0.0 << " " << calcE(particle) << std::endl;
        }
     }
    ofs.close();
}
