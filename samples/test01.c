#include <stdio.h>
#include <math.h>

int main()
{
	int i;
	float b;

	b = 0;

	for (i=1; i <= 100; i++) {
		b = b + 0.1;
	}
	
			printf("%f", b);

	return 0;



/* シフト演算とは2のべき乗をかけることである */
}
