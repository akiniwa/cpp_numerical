#include <iostream>
#include <math.h>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <string.h>

const double L = 6.3781E6;
const double G = 6.6743E-11;
const double M = 5.9722E24;
const int earthT = 23*3600+56*60+4;
const int sateliteT = 12*3600;

const double m = 1;
const double dT = 0.001;
const int sympI = 3;
const double J2 = 1082.264E-6;

const double geo_radius = pow(G*M*pow((earthT/(2*M_PI)), 2), 1/3.0);
const double geo_velocity = geo_radius*2*M_PI/earthT;

const int MAXTIME = 1000000;

const int particleNumber = 2;

// シンプレクティック積分に使う係数
const double symp[3] = {
    +1.35120719195965777182,
    -1.70241438391931554364,
    +1.35120719195965777182
};

// 衛星の定義
struct Particle {
    double px, py, pz;
    double qx, qy, qz;
    Particle() {
        printf("I called.\n");
        px = 0.0/pow(M*G/L, 0.5);
        py = geo_velocity/pow(M*G/L, 0.5);
        pz = 0.0/pow(M*G/L, 0.5);
        
        qx = geo_radius/L;
        qy = 0.0;
        qz = 0.0;
    }

    Particle(float x) {
        printf("with you.\n");
        px = 0.0/pow(M*G/L, 0.5);
        py = geo_velocity/pow(M*G/L, 0.5);
        pz = 0.0/pow(M*G/L, 0.5);
        
        qx = geo_radius/L;
        qy = 0.0;
        qz = 0.0;
    }
};

// 衛星の宣言
Particle par[particleNumber];

#include "calcKeplerElement.cpp"
#include "odeSymplectic.cpp"

// 軌道長半径を計算
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

    double earthT_nonD = earthT/sqrt(pow(L, 3)/(M*G));	        //無次元化 地球自転周期
    double sateliteT_nonD = sateliteT/sqrt(pow(L, 3)/(M*G));    //無次元化 衛星周期

    FILE* one[particleNumber];
    FILE* reso[particleNumber];
    for (int i=0;i<particleNumber;i++) {

        if (i==1) {
            //double energy = calcE_J2(par[0].qx, par[0].py, par[0]);
            //double energy = calcE(par[0].qx, par[0].py);
            //par[i].py = 0.9*geo_velocity/pow(M*G/L, 0.5);
            //par[i].qx = -1.0/(energy-0.5*pow(par[i].py, 2))*0.962;
        }

        char str[3];
        char filename[20] = "test/";
        sprintf(str, "%d", i);
        char strreso[10];
        sprintf(strreso, "%s%d","reso", i);
        one[i] = fopen(strcat(filename, str), "w");
        reso[i] = fopen(strcat(filename, strreso), "w");
    }
    FILE* f = fopen("keps/py", "w");

    fprintf(f, "x      y      z      Energy      a      h      e      I      OMEGA\n");
    //fprintf(reso, "cos() sin()\n");

    for (int i=0;i<MAXTIME;i++) {
        double theta = 2*M_PI/earthT_nonD*incrementT;
        //double theta = 2*M_PI/sateliteT_nonD*incrementT;
        incrementT += dT;

        if (i%500==0) {
            for (int i=0;i<particleNumber;i++) {
                reportElement_toFile(one[i], reso[i], J2, theta, par[i]);
            }
        }

        // ワンステップでdT=0.001動く
        for (int s=0;s<sympI;s++) {
            // 2次のシンプレクティック積分を3回、係数を変化させて行う
            if (args[1]) {
                for (int i=0;i<particleNumber;i++) {
                    symp2_for_J2(&par[i], dT*symp[s]);
                }
            } else {
                for (int i=0;i<particleNumber;i++) {
                    symp2(&par[i], dT*symp[s]);
                }
           }
        }
    }
    fclose(f);
   // fclose(reso);
    end = clock();
    printf("%.10lf[s]\n", (double)(end - start)/CLOCKS_PER_SEC);
}
