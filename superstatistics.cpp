#include <stdio.h>
#include <math.h>

double probability(double n, double beta_0, double u);

int main()
{
	double n = 3;
	double beta_0 = 1.2;
	double u = -6;

	char *data_file;
	FILE *fp = popen("gnuplot -persist", "w");
	if (fp == NULL) {
		return -1;
	}

	fprintf(fp, "set xrange [-6:6]\n");
	fprintf(fp, "set yrange [0:1]\n");
	fprintf(fp, "plot '-' with lines linetype 1\n");
	for (int i=1;i<1200;i++)
	{
		u = u + 0.01;
		fprintf(fp, "%f %f\n", u, probability(n, beta_0, u));	
	}

	pclose(fp);

	return 0;
}


double probability(double n, double beta_0, double u)
{
	double f;

	f = 0.398942*(tgamma(n/2+1/2)/tgamma(n/2))*pow(beta_0/(M_PI*n), 1/2)*(1/pow(1+(beta_0*pow(u, 2))/n, n/2+1/2));

	return f;
}
