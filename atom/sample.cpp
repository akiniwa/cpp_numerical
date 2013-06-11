#include <stdio.h>
#include <math.h>

//double R;
/* Rの初期値を固定 */  
const double dT = 0.001;
const double totalE = -4911;

const double D_xb = 18.0;
const double beta_xb = 0.60326;
const double myu_xb = 7.55891;

const double D_bc = 4911.0;
const double beta_bc = 0.9380;
const double myu_bc = 5.6994;

const double myu = 3.8779;
const double m = 63.5;

void Runge_Kutta(double R, double ) { 
}

double XBlength(double R, double r) {
	return pow((pow(R, 2) + pow(r, 2)/4.0), 0.5);
}

double XBPotentialE(double R) {
	return (D_xb * ((exp(-2*beta_xb*(R - myu_xb))) - 2 * exp(-beta_xb*(R - myu_xb)))); 
}

double BCPotentialE(double r) {
	return (D_bc * ((exp(-2*beta_bc*(r - myu_bc))) - 2 * exp(-beta_bc*(r - myu_bc)))); 
}

double totalPotentialE(double R, double r) {
	return (2 * XBPotentialE(XBlength(R, r)) + BCPotentialE(r));
}

int main() {
    double R;
    double Rk[4];
    double rk[4];
    const double r = 5.6994;
    FILE *gp;

    gp = popen("gnuplot -persist", "w");	// パイプを開き、gnuplotの立ち上げ
    
    fprintf(gp, "plot '-' with dots \n");
    // fprintf(gp, "plot \n");
    for (double i = -0.5; i < 10; i+=0.001) {
        double Q = i - 7.001;
        double tP = totalPotentialE(i+myu_bc, myu_bc);

        if ((totalE-tP)>0) {
            fprintf(gp, "%f %f\n", Q, sqrt(2*myu*(totalE - tP)));
            fprintf(gp, "%f %f\n", Q, -sqrt(2*myu*(totalE - tP)));
        } 
    }

    fprintf(gp, " exit\n");	 // gnuplotの終了
    fflush(gp);
    pclose(gp);	// パイプを閉じる
}
