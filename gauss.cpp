#include <stdio.h>
#include <math.h>

double gaussian(double u);

int main()
{
	char *data_file;
	FILE *fp = popen("gnuplot -persist", "w");
	if (fp == NULL) {
		return -1;
	}

	double u = -6;

	fprintf(fp, "set xrange [-6:6]\n");
	fprintf(fp, "set yrange [0:1]\n");
	fprintf(fp, "plot '-' with lines linetype 1\n");
	for (int i=1;i<1200;i++)
	{
		u = u + 0.01;
		fprintf(fp, "%f %f\n", u, gaussian(u));	
			if (i==600)
			printf("%f\n",gaussian(u));
	}

	pclose(fp);

	return 0;
}


double gaussian(double u)
{
	double f;

	f = (1/sqrt(2*M_PI))*exp(-pow(u, 2)/2);

	return f;
}
