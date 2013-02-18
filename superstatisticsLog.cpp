#include <stdio.h>
#include <math.h>

double probability(double u, double m, double beta, double s);

int main()
{
	double m = 1.2;
	double beta = 0;
	double s = 3;

	char *data_file;
	file *fp = popen("gnuplot -persist", "w");
	if (fp == null) {
		return -1;
	}

	fprintf(fp, "set xrange [0:12]\n");
	fprintf(fp, "set yrange [0:1]\n");
	fprintf(fp, "plot '-' with lines linetype 1\n");
	for (int i=1;i<1200;i++)
	{
		beta = beta + 0.01;
		fprintf(fp, "%f %f\n", beta, probability(m, beta, s));	
	}

	pclose(fp);

	return 0;
}


double probability(double m, double beta, double s)
{
	double f;

	f = (1/(beta*s*sqrt(2*m_pi)))*exp((pow(-log(beta/m),2))/(2*pow(s, 2)));

	return f;
}
