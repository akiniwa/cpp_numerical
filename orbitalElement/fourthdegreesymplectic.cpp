#include <iostream>
#include <math.h>
#include <fstream>
#include <time.h>
#include <stdio.h>

const double G = 1;
//const double G = 4*M_PI*M_PI;
const double M = 1;
const double m = 1;
const double dT = 0.001;
const int sympI = 3;
const double J2 = 1082.264E-4;
const double aE = 1.0;
//const double aE = 6378E3;

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
        px =0.0;
        py =1.0;
        pz =0.0;
        
        qx = 1.0;
        qy = 0.0;
        qz = 0.0;
        /*
        px =0.1;
        py =1.0;
        pz =0.1;
        
        qx = 1.0;
        qy = 0.1;
        qz = 0.1;
        */
    }
};

// 衛星の宣言
Particle particle;

double calc(double x, double y, double z) {
    double r = hypot(hypot(x, y), z);
    double accelaration = G*M*(-1/(pow(r, 3)) - 7.5*pow(aE, 2)*pow(z, 2)*J2/(pow(r, 7)) + 1.5*J2*pow(aE, 2)/pow(r, 5))*x;
//  double accelaration = -G*M*x/(r*r*r);
    return accelaration;
}
double calcZ(double x, double y, double z) {
    double r = hypot(hypot(x, y), z);
    double accelaration = G*M*(-1/(pow(r, 3)) + J2*pow(aE, 2)*(3/(pow(r, 5)) - 7.5*pow(x, 2)/pow(r, 7) + 1.5/pow(r, 5)))*x;
//  double accelaration = -G*M*x/(r*r*r);
    return accelaration;
}
// 二次のシンプレクティック
void symp2(Particle* pa, double t) {
    pa->px+=calc(pa->qx, pa->qy, pa->qz) *0.5*t;
    pa->py+=calc(pa->qy, pa->qx, pa->qz) *0.5*t;
    pa->pz+=calcZ(pa->qz, pa->qx, pa->qy) *0.5*t;

    pa->qx+=pa->px/m*t;
    pa->qy+=pa->py/m*t;
    pa->qz+=pa->pz/m*t;

    pa->px+=calc(pa->qx, pa->qy, pa->qz) *0.5*t;
    pa->py+=calc(pa->qy, pa->qx, pa->qz) *0.5*t;
    pa->pz+=calcZ(pa->qz, pa->qx, pa->qy) *0.5*t;
}

// 軌道長半径を計算
double a_length(Particle pa) {
    double q = hypot(hypot(pa.qx, pa.qy), pa.qz);
    double p = hypot(hypot(pa.px, pa.py), pa.pz);
    return q*G*M/(2*G*M-pow(p, 2)*q);
}

// 全エネルギーを計算
double calcE(Particle pa) {
    double q = hypot(hypot(pa.qx, pa.qy), pa.qz);
    double p = hypot(hypot(pa.px, pa.py), pa.pz);
    double K = 0.5/m*pow(p, 2);
//    double U = - G*M*m/q;
    double U = - G*M*m*(1.0/q + J2*pow(aE, 2)/pow(q, 3)*(1.5*pow((pa.qz/q), 2) - 0.5));
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

    //FILE* f = fopen("kep", "w");
    std::ofstream ofs;
    ofs.open( "kep" );
    //fprintf(f, "x y z Energy\n");
    ofs << "x y z Energy a h e I OMEGA" << std::endl;
    for (int i=0;i<1000000;i++) {
        if (i%500==0) {
            // ファイルに出力
            double a = a_length(particle);
            double h = h_moment(particle);
            double e = e_anomaly(a, h);
            double I = I_anomaly(h);
            double OMEGA = OMEGA_anomaly(I, h);
            
    //        fprintf(f, "%f, %f, %f, %f\n", particle.qx, particle.qy, particle.qz, calcE(particle));
            ofs << particle.qx << " " << particle.qy << " " << particle.qz << " " << calcE(particle) << " " << a << " " << h << " " << e << " " << I << " " << OMEGA << std::endl;
        }
         for (int s=0;s<sympI;s++) {
            // 2次のシンプレクティック積分を3回、係数を変化させて行う
            symp2(&particle, dT*symp[s]);
        }
    }
    ofs.close();
    end = clock();
    printf("%.10lf[s]\n", (double)(end - start)/CLOCKS_PER_SEC);
}
