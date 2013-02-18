#include "myheader.h"
#define PI 3.141592653
double ratio=1.0;

double PieArea( double radius )
{
		double area;
		area = PI*radius*radius*ratio;
		return( area );
}
