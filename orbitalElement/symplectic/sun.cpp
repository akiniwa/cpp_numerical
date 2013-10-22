#include <iostream>
#include <math.h>
#include <fstream>
#include <time.h>
#include <stdio.h>

const double L = 6.3781E6;
const double sunL = 149600E5;
const double sunM = 1.989E30;
const double G = 6.6743E-11;
const double M = 5.9722E24;
const int earthT = 23*3600+56*60+4;
const int sateliteT = 22*3600;

const double m = 1;
const double dT = 0.001;
const int sympI = 3;
const double J2 = 1082.264E-6;

const double geo_radius = pow(G*M*pow((earthT/(2*M_PI)), 2), 1/3.0);
const double geo_velocity = geo_radius*2*M_PI/earthT;

const double nonDsunM = sunM/M;
const double nonDsunL = sunL/L;

const int MAXTIME = 10000000;

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
        py = geo_velocity/pow(M*G/L, 0.5);
        pz = 0.0/pow(M*G/L, 0.5);
        
        qx = geo_radius/L;
        qy = 0.0;
        qz = 0.0;
    }
};

// $B1R@1$N@k8@(B
Particle particle;

#include "calcKeplerElement.cpp"
#include "odeSunSymplectic.cpp"

// $B50F;D9H>7B$r7W;;(B
int main(int arg, char* args[]) {
    bool J2 = false;

    if (args[1]) {
        printf("You have a J2 perturbation.\n");
        J2 = true;
    } else {
        printf("No perturbation.\n");
        J2 = false;
    }

    double incrementT = 0.0;
    clock_t start, end;
    start = clock();

    double earthT_nonD = earthT/sqrt(pow(L, 3)/(M*G));	        //$BL5<!852=(B $BCO5e<+E><~4|(B
    double sateliteT_nonD = sateliteT/sqrt(pow(L, 3)/(M*G));    //$BL5<!852=(B $B1R@1<~4|(B

    FILE* f = fopen("dirsun/sunkep", "w");
    FILE* reso = fopen("dirsun/sunreso", "w");

    fprintf(f, "x      y      z      Energy      a      h      e      I      OMEGA\n");
    fprintf(reso, "cos() sin()\n");

    for (int i=0;i<MAXTIME;i++) {
        double theta = 2*M_PI/earthT_nonD*incrementT;
        incrementT += dT;

        if (i%5000==0) {
            reportElement_toFile(f, reso, J2, theta);
        }

        for (int s=0;s<sympI;s++) {
            // 2$B<!$N%7%s%W%l%/%F%#%C%/@QJ,$r(B3$B2s!"78?t$rJQ2=$5$;$F9T$&(B
            if (args[1]) {
                symp2_for_J2(&particle, dT*symp[s]);
            } else {
                symp2(&particle, dT*symp[s]);
            }
        }
    }

    fclose(f);
    fclose(reso);
    end = clock();
    printf("%.10lf[s]\n", (double)(end - start)/CLOCKS_PER_SEC);
}
