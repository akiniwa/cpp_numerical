#include "particle.hpp"

class Satellite : public Particle {
   private:
        static const double J2;
        double RungeKuttaDQ(double);
        double RungeKuttaDP(double, double, double);
        double RungeKuttaDPZ(double, double, double);
   public:
        static const double totalE;
        static const double GM;
        static const double M;
        Satellite();
        double accelaration;
        double getKineticE();
        double getPotentialE();
};
