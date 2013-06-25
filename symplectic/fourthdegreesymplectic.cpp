#include <iostream>
#include <math.h>
#include <fstream>

//const double GM = 4*M_PI*M_PI;
const double G = 4*M_PI*M_PI;
const double M = 2;
const double m = 1;
const double dT = 0.0001;
const int sympI = 3;
const double J2 = 1082.264E-6;
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
        px = 0.0;
        py = 0.6*sqrt(2*G*M);
        pz = 0.0;
        
        qx = aE;
        qy = 0.0;
        qz = 0.0;
    }
};

// 衛星の宣言
Particle particle;

// 
double calc(double x, double y, double z) {
    double r = hypot(hypot(x, y), z);
    /*
<<<<<<< HEAD
//  double accelaration = (-GM/(r*r*r) + 3*GM*r*r/(r*r*r)*J2*(1.5*pow(particle.qz/r, 2)-0.5))*x;
    double accelaration = -GM*x/(r*r*r);
=======
*/
//    double accelaration = G*M*(-1/(pow(r, 3)) + 7.5*aE*aE*z*z*J2/(pow(r, 7)) - 1.5*J2*aE*aE/pow(r, 5))*x;
  double accelaration = -G*M*x/(r*r*r);
//>>>>>>> origin/ellipse
    return accelaration;
}

// 二次のシンプレクティック
void symp2(Particle* pa, double t) {
    pa->px+=calc(pa->qx, pa->qy, pa->qz) *0.5*t;
    pa->py+=calc(pa->qy, pa->qz, pa->qx) *0.5*t;
    pa->pz+=calc(pa->qz, pa->qx, pa->qy) *0.5*t;

    pa->qx+=pa->px/m*t;
    pa->qy+=pa->py/m*t;
    pa->qz+=pa->pz/m*t;

    pa->px+=calc(pa->qx, pa->qy, pa->qz) *0.5*t;
    pa->py+=calc(pa->qy, pa->qz, pa->qx) *0.5*t;
    pa->pz+=calc(pa->qz, pa->qx, pa->qy) *0.5*t;
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
  double U = - G*M*m/q;
//    double U = - G*M*m*(1.0/q + J2*pow(aE, 2)/pow(q, 3)*(1.5*pow((pa.qz/q), 2) - 0.5));
    return K + U;
}

/*
double h_moment() {
    double lxy = 1/M
}
*/

int main() {
    std::ofstream ofs;
    ofs.open( "kep" );
    for (int i=0;i<1000000;i++) {
        for (int s=0;s<sympI;s++) {
            // 2次のシンプレクティック積分を3回、係数を変化させて行う
            symp2(&particle, dT*symp[s]);
        }
        if (i%500==0) {
            // ファイルに出力
            ofs << particle.qx << " " << particle.qy << " " << particle.qz << " " << calcE(particle) << " " << a_length(particle) << std::endl;
        }
     }
    ofs.close();
}
