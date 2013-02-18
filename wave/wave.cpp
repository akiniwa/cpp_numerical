#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>


#include <math.h>
#define  ANIMATION

#include "openGLsample.hpp"

//---- physical setting
#define N          (256)
#define K          (1.0)
#define M          (1.0)
#define dT         (1.0/16)

//---- graphic setting
#define WIN_WIDTH  (512)
#define WIN_HEIGHT (256)

//---- initialize wave
void Init( double x[N], double p[N] )
{
  for( int i=0 ; i<N ; i++ ){
    x[i] = exp(-0.01*(i-N/2)*(i-N/2));
    p[i] = 0.0;
  }
}

//---- set border conditions on both side edges
void Edge( double x[N], double p[N] )
{
  x[0] = x[N-1] = 0.0;
  p[0] = p[N-1] = 0.0;
}

//---- evolve the system by time 16*dT
void Evolve( double x[N], double p[N] )
{
  int i;

  for( int interval=0 ; interval<16 ; interval++ ){
    for( i=0 ; i<=N-1 ; i++ ){
      x[i] += p[i]/M*dT;
    }
    p[0] += K*( x[1] - x[0] )*dT;
    for( i=1 ; i<=N-2 ; i++ ){
      p[i] += K*( x[i+1] + x[i-1] - 2*x[i] )*dT;
    }
    p[N-1] += K*( x[N-2] - x[N-1] )*dT;
  }
}

//---- draw phonon wave
void Draw( double x[N] )
{
  for( int i=1 ; i<N ; i++ ){
  }
}


//---- main function
int main(int argc, char *argv[])
{

  double x[N], p[N];
  FILE *fp;

  if (argc <= 1) {
		  printf("not enough argument\n");
		  exit(0);
  }
  if ((fp = fopen(argv[1], "w")) == NULL) {
		  printf("file cannot open\n");
		  exit(0);
  }

  int i = 0;
  Init( x, p );
  do{
    Edge( x, p );
    Evolve( x, p );
    Draw( x );
	i++;
	fprintf(fp, "%f %f \n",x[i], p[i]);
  }while(i<257);
  	fclose(fp);

}
