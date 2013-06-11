#include <stdio.h>
#include <math.h>

//double R;
double r;
double Rk[4];
double rk[4];
double dT = 0.001;
double totalE = -1/2661.6;

double D_xb = 18.0;
double beta_xb = 0.60326;
double myu_xb = 7.55891;

double D_bc = 4911.0;
double beta_bc = 0.9380;
double myu_bc = 5.6994;

double myu = 3.8779;
double m = 63.5;

/* Rの初期値固定 */
const double R = 20.0;

void Runge_Kutta(double R, double ) {

}

int main() {
    printf("%f, %f", pow(2, 3), exp(3));

}
