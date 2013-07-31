#include <iostream>
#include <math.h>
#include <fstream>
#include <time.h>
#include <stdio.h>

const double L = 6.3781E6;
const double G = 6.6743E-11;
const double M = 5.9722E24;
const int earthT = 24*3600;
const int sateliteT = 22*3600;

const double m = 1;
const double dT = 0.001;
const int sympI = 3;
const double J2 = 1082.264E-5;

const double geo_radius = pow(G*M*pow((earthT/(2*M_PI)), 2), 1/3.0);
const double geo_velocity = geo_radius*2*M_PI/earthT;

const int MAXTIME = 1000000;

// $B%7%s%W%l%/%F%#%C%/@QJ,$K;H$&78?t(B
const double symp[3] = {
    +1.35120719195965777182,
    -1.70241438391931554364,
    +1.35120719195965777182
};

// $B1R@1$NDj5A(B
struct Particle {
    double px, py, pz;
    double qx, qy, qz;
    Particle() {
        px = 0.0/pow(M*G/L, 0.5);
        py = geo_velocity/pow(M*G/L, 0.5)/0.95;
        pz = 0.0/pow(M*G/L, 0.5);
        
        qx = geo_radius/L;
        qy = 0.0;
        qz = 0.4;
    }
};

// $B1R@1$N@k8@(B
Particle particle;

double calc(double x, double y, double z) {
    double r = hypot(hypot(x, y), z);
    double accelaration = ( (-1/pow(r, 3)) + J2*( (-7.5*pow(z, 2)/pow(r, 7)) + (1.5/pow(r, 5)) ) )*x;
    return accelaration;
}

double calcZ(double x, double y, double z) {
    double r = hypot(hypot(x, y), z);
    double accelaration = ( (-1/pow(r, 3)) + J2*( (3/pow(r, 5)) - (7.5*pow(x, 2)/pow(r, 7)) + (1.5/pow(r, 5)) ) )*x;
    return accelaration;
}

// $BFs<!$N%7%s%W%l%/%F%#%C%/(B
void symp2(Particle* pa, double t) {
    pa->px+=calc(pa->qx, pa->qy, pa->qz) *0.5*t;
    pa->py+=calc(pa->qy, pa->qx, pa->qz) *0.5*t;
    pa->pz+=calcZ(pa->qz, pa->qx, pa->qy) *0.5*t;

    pa->qx += pa->px/m*t;
    pa->qy += pa->py/m*t;
    pa->qz += pa->pz/m*t;

    pa->px += calc(pa->qx, pa->qy, pa->qz) *0.5*t;
    pa->py += calc(pa->qy, pa->qx, pa->qz) *0.5*t;
    pa->pz += calcZ(pa->qz, pa->qx, pa->qy) *0.5*t;
}

// $B50F;D9H>7B$r7W;;(B
double a_length(double q, double p) {
   return q/(2-pow(p, 2)*q);
}

// $BA4%(%M%k%.!<$r7W;;(B
double calcE(double q, double p) {
    double K = 0.5*pow(p, 2);
//  double U = - 1.0/q;
    double U = -1.0/q + J2/pow(q, 3)*(1.5*pow((particle.qz/q), 2) - 0.5);
    return K + U;
}

double h_moment(Particle pa) {
    double lxy = 1/M * ((pa.qx * pa.py) - (pa.qy * pa.px));
    double lyz = 1/M * ((pa.qy * pa.pz) - (pa.qz * pa.py)); 
    double lzx = 1/M * ((pa.qz * pa.px) - (pa.qx * pa.pz));
    return hypot(hypot(lxy, lyz), lzx);
}
double e_anomaly(double a, double h) {
    return pow((1 - h*h/(G*M*a)), 0.5);
}

double I_anomaly(double h) {
    double lxy = 1/M * ((particle.qx * particle.py) - (particle.qy * particle.px));
    return acos(lxy/h);
}

double OMEGA_anomaly(double I, double h) {
    double lxy = 1/M * ((particle.qx * particle.py) - (particle.qy * particle.px));
    return asin(lxy/(sin(I)*h));
}

int main() {
    clock_t start, end;
    start = clock();

    double earthT_nonD = earthT/sqrt(pow(L, 3)/(M*G));	        //$BL5<!852=(B $BCO5e<+E><~4|(B
    double sateliteT_nonD = sateliteT/sqrt(pow(L, 3)/(M*G));    //$BL5<!852=(B $B1R@1<~4|(B

    FILE* f = fopen("kep", "w");
    FILE* reso = fopen("resonant", "w");

    fprintf(f, "x      y      z      Energy      a      h      e      I      OMEGA\n");
    fprintf(reso, "cos() sin()\n");

    for (int i=0;i<MAXTIME;i++) {
        if (i%500==0) {
            // $B%U%!%$%k$K=PNO(B
            double q = hypot(hypot(particle.qx, particle.qy), particle.qz);
            double p = hypot(hypot(particle.px, particle.py), particle.pz);

            double a = a_length(q, p);
            double h = h_moment(particle);
            double e = e_anomaly(a, h);
            double I = I_anomaly(h);
            double OMEGA = OMEGA_anomaly(I, h);

            double lambda = atan(particle.qy/particle.qx);
            double theta = 2*M_PI/earthT_nonD*dT;
            fprintf(f, "%.4f %.4f %.4f %.8f %.8f %.4f %.4f %.4f %.4f\n", particle.qx, particle.qy, particle.qz, calcE(q, p), a, h, e, I, OMEGA);
            fprintf(reso, "%f %f\n", a*(particle.qx/q*cos(theta)+particle.qy/q*sin(theta)), a*(particle.qy/q*cos(theta)-particle.qx/q*sin(theta))); 
        }
         for (int s=0;s<sympI;s++) {
            // 2$B<!$N%7%s%W%l%/%F%#%C%/@QJ,$r(B3$B2s!"78?t$rJQ2=$5$;$F9T$&(B
            symp2(&particle, dT*symp[s]);
        }
    }
    fclose(f);
    fclose(reso);
    end = clock();
    printf("%.10lf[s]\n", (double)(end - start)/CLOCKS_PER_SEC);
}