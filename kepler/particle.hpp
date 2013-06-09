#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <math.h>

class Particle {
    private:
        int rungeIndex;
        double qxk[4], qyk[4], qzk[4], pxk[4], pyk[4], pzk[4];
        void RungeKutta(double, double, double, double,double, double );
        virtual double RungeKuttaDQ(double) = 0;
        virtual double RungeKuttaDP(double) = 0;
     public:
        Particle();
        double qx;
        double qy;
        double qz;

        double px;
        double py;
        double pz;

        double dT;
        void motion();
};

#endif
