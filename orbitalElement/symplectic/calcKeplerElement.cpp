
double a_length(double q, double p) {
   return q/(2-pow(p, 2)*q);
}

// 全エネルギーを計算
double calcE(double q, double p) {
    double K = 0.5*pow(p, 2);
    double U = - 1.0/q;
    return K + U;
}

double calcE_J2(double q, double p) {
    double K = 0.5*pow(p, 2);
    double U = -1.0/q - J2/pow(q, 3)*(1.5*pow((particle.qz/q), 2) - 0.5);
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

void reportElement_toFile(FILE* f,FILE* reso, bool J2, double theta) {
    double q = hypot(hypot(particle.qx, particle.qy), particle.qz);
    double p = hypot(hypot(particle.px, particle.py), particle.pz);

    double a = a_length(q, p);
    double Energy = 0;
    if (J2) {
        Energy = calcE_J2(q, p);
    } else {
        Energy = calcE(q, p);
    }
    double h = h_moment(particle);
    double e = e_anomaly(a, h);
    double I = I_anomaly(h);
    double OMEGA = OMEGA_anomaly(I, h);

    fprintf(f, "%.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f %.4f\n", particle.qx, particle.qy, particle.qz, Energy, a, h, e, I, theta);
    fprintf(reso, "%f %f\n", a*(particle.qx/q*cos(theta)+particle.qy/q*sin(theta)), a*(particle.qy/q*cos(theta)-particle.qx/q*sin(theta))); 
}
