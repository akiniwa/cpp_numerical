#include "particle.hpp"

class Satellite : public Particle {
   private:
        static const double GM;
        static const double M;
        double RungeKuttaDQ(double);
        double RungeKuttaDP(double);
    public:
        Satellite();
};
