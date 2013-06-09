#include "particle.hpp"

class Satellite : public Particle {
   private:
        static const double root_AC;
        static const double theta_dot;
        double RungeKuttaDQ(double);
        double RungeKuttaDP(double);
        double p_dot;
        double q_dot;
    public:
        Satellite();
        double getCoordinate(int);
        enum XYZ {
            X, 
            Y,
            Z
        };
};
