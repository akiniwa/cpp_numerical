#include "particle.hpp"

class Satellite : public Particle {
   private:
        static const double M;
        double RungeKuttaDQ(double);
        double RungeKuttaDP(double);
        double rotate(int);
        double omega;
        double inclination;
        double omegaSmall;
        double theta;
    public:
        static const double GM;
        static const double totalE;
        Satellite();
        void COEs(double, double, double);
        double getCoordinate(int);
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
