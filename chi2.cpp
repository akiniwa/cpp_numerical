#include <stdio.h>
#include <math.h>

double chi2(double u, double beta_0, double beta);

int main()
{
	double n = 3;
	double beta_0 = 1.2;
	double beta = 0;

	char *data_file;
	FILE *fp = popen("gnuplot -persist", "w");
	if (fp == NULL) {
		return -1;
	}

	fprintf(fp, "set xrange [0:6]\n");
	fprintf(fp, "set yrange [0:1]\n");
	fprintf(fp, "plot '-' with lines linetype 1\n");
	for (int i=1;i<600;i++)
	{
		beta = beta + 0.01;
		fprintf(fp, "%f %f\n", beta, chi2(n, beta_0, beta));	
	}

	pclose(fp);

	return 0;
}


double chi2(double n, double beta_0, double beta)
{
	double f;

	f = (1/tgamma(n/2))*pow((n/2*beta_0), n/2)*pow(beta, n/2-1)*exp((-n*beta)/(2*beta_0));

	return f;
}
