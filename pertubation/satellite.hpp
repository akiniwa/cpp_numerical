#include "particle.hpp"

class Satellite : public Particle {
   private:
        static const double J2;
        static const double eq_radius;
        double RungeKuttaDQ(double);
        double RungeKuttaDP(double);
        double rotate(int);
        double omega;
        double inclination;
        double omegaSmall;
    public:
        static const double totalE;
        static const double GM;
        static const double M;
        Satellite();
        void COEs(double, double, double);
        double getCoordinate(int);
        double accelaration;
        enum XYZ {
            X, 
            Y,
            Z
        };
        enum ANGLE {
            OMEGA,
            INCLINATION,
            OMEGASMALL
        };
};
