#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "disp_array.c"
#include "kihonenzan.c"

#define n 6   //number of molecule
#define D 3  //dimention
#define N n*3    //dimention * number of molecule
#define dist 0.001	//刻み幅
#define mass 1.0
#define wantH -11.0 //希望するエネルギー

void jacobi(double y[],double p[]);
void runge(double t, double y[],double p[],double step);
void KaisekiHyperspherical(double y[],double p[]);

double getrand(int rmin,int rmax);
double hamilton(double y[],double p[]);
double angular(double y[],double p[]);
double fy(double t, double y[],double p[], int i);
double fp(double t,double y[],double p[], int i);

double px,py,pz;
double t,H;
double LJ,totallkinetic,Lx,Ly,Lz;
double fcent,fcenti[3],fpot;
double L[3];

double jacoy[D*(n-1)],jacop[D*(n-1)];
double Gy[D],Gp[D];
long int ldW=D,nW=n-1,ldR=D, ldUt=n-1; /* ldR=ldW, ldUt=nW */
double  gyradii[D];
double difgyradii[D]; /*da/dt = Dgyradii*/
double omega[3]; /*du/dt->omega[3]=transpose(w23,w31,w12)*/
double ganma[(n-4)*3]; /*du/dt->ganma[(n-4)*3]=(ganma1 ganma2 ganma3)*/
double kineticE,kineticG;


long int info, lwork,iwork;
int i,j,m,a,b,k;
  /*set matrix of dimention*/

/*double W[ldW*nW], R[ldR*ldR], Ut[nW*nW]*/
double W[D*(n-1)], R[D*D], Ut[(n-1)*(n-1)];

double randx;


int main(void)
{

  double y[N],p[N];
  double checkkinetic,powganma;

  int count=0;
  int tcount = 0;


  /*------set data FILE-----*/
  FILE *outputH;
  FILE *outputL;
  FILE *outgyradii;
  FILE *outputPDB;
  FILE *outcheck;
  FILE *outomeganma;
  FILE *outforce;

  outputH = fopen("outH.dat", "w");
  outputL = fopen("outL.dat", "w");
  outgyradii = fopen("gyradii_6_e11_low.dat","w");
  outcheck = fopen("check_kinetic6.dat","w");
  outputPDB = fopen("ar6move11_low.pdb","w");
  outomeganma = fopen("ar6omeganma.dat","w");
  outforce = fopen("ar6force.dat","w");

  /*set first memomenta as 0*/
  for (m=0; m<N; m++) {
    p[m] = 0.0;
  }

  /*set first point*/
  FILE *input;

  input	= fopen("minimum6.dat","r");

  char lBuf[1024];
  for( m=0; m < n; m++ ) {
    fgets( lBuf, sizeof( lBuf ), input );
    sscanf(lBuf,"%lf%lf%lf", &y[3*m], &y[3*m+1] , &y[3*m+2] );
  }


  fclose(input);
  srand((unsigned)time(NULL));

  hamilton(y,p);
  KaisekiHyperspherical(y,p);
  printf("Input Energy = %f\n",H);
  printf("Gyration radii= (%f %f %f)\n",gyradii[0],gyradii[1],gyradii[2]);

  /*-----Energy = - 11.0 に平衡化--------*/
  scalmomentum(y,p);
  hamilton(y,p);
  KaisekiHyperspherical(y,p);
  checkkinetic = totallkinetic - kineticE - kineticG;

  printf("scaled energy = %f\n",H);
  printf("-11に設定した後のcheck=%f\ntotallkinetic=%f : kineticE=%f\n",checkkinetic,totallkinetic,kineticE);
  printf("kineticG =%f\n\n",kineticG);
  printf("Gyration radii= (%f %f %f)\n",gyradii[0],gyradii[1],gyradii[2]);

  /*しばらくの間動かしてみる*/
  for (t=0.0; t < 200; t += dist) {
    runge(t,y,p,dist);
    KaisekiHyperspherical(y,p);
    hamilton(y,p);
    angular(y,p);
    checkkinetic = totallkinetic - kineticE - kineticG;

    /*output data*/
    fprintf(outputH,"%f %f %f %f\n",t,H,LJ,totallkinetic);
    fprintf(outputL,"%f %f %f %f\n",t,L[0],L[1]+1.0,L[2]+2.0);
    fprintf(outcheck,"%f %f\n",t,checkkinetic);

    if (count%500 == 0) {
      for (m=0; m<n; m++){
	fprintf(outputPDB,"ATOM      %d Ar       X   %d%12.3f%8.3f%8.3f  1.00  0.00\n",m+1,m+1,y[3*m],y[3*m+1],y[3*m+2]);
      }
      fprintf(outputPDB,"END\n");
    }

    fprintf(outgyradii,"%f %f %f %f\n",t,gyradii[0],gyradii[1],gyradii[2]);
    fprintf(outomeganma,"%f %f %f %f ",t,pow(omega[0],2),pow(omega[1],2),pow(omega[2],2));
    for(i=0;i<3;i++)
      { powganma = 0.0;
	for(k=0;k<n-4;k++)
	  {
	    powganma += pow(ganma[i*(n-4)+k],2);
	  }
	fprintf(outomeganma,"%f ",powganma);
      }
    fprintf(outomeganma,"\n");
    fprintf(outforce,"%f %f %f %f %f\n",t,fcent,fcenti[0],fcenti[1],fcenti[2]);

  }
  /*途中経過を吐き出す。*/
  hamilton(y,p);

  printf("ハミルトニアンは%fでした",H);
  printf("全部でt=%f秒,\n tcount=%d秒でした\nあぼーん ",t,tcount);
  printf("data stored in out.dat\n");
  fclose(outputH);
  fclose(outputL);
  fclose(outgyradii);
  fclose(outcheck);
  fclose(outputPDB);
  fclose(outomeganma);
  fclose(outforce);
}

/*--------end of main program-------------*/

  /*---Hamiltonian energy-----*/
double hamilton(double y[],double p[]){
  H=0;
  totallkinetic = 0;
  LJ = 0;

  for (i=0; i<n; i++) {
    for (j=0; j<3; j++) {
      totallkinetic += pow(p[3*i+j], 2)/2;
    }
  }

  for (i=0; i<n; i++) {
    for (j=0; j<n; j++) {
      if (i >= j) continue;
      LJ += 4*(pow(pow(y[3*i]-y[3*j],2)+pow(y[3*i+1]-y[3*j+1], 2)+pow(y[3*i+2]-y[3*j+2], 2), -6)
	       -pow(pow(y[3*i]-y[3*j],2)+pow(y[3*i+1]-y[3*j+1], 2)+pow(y[3*i+2]-y[3*j+2], 2), -3));
    }}
  H = totallkinetic + LJ;

  return (totallkinetic,H);

}

double angular(double y[],double p[]){
  Lx=0;
  Ly=0;
  Lz=0;
  for (i=0; i<n; i++) {
    Lx += y[3*i+1]*p[3*i+2]-y[3*i+2]*p[3*i+1];
    Ly += y[3*i+2]*p[3*i]-y[3*i]*p[3*i+2];
    Lz += y[3*i]*p[3*i+1]-y[3*i+1]*p[3*i];
  }
  double L[3]={Lx,Ly,Lz};
  return (L[3]);
}


/*スケーリングする*/
/*エネルギーが-11.0になるように調整する*/
/*速度(ここでは運動量)に対して定数倍する*/
scalmomentum(double y[],double p[]){
  double dx,dy,dz;

  int rmin,rmax;
  rmin =-1000;
  rmax =5000;

  /*move first point*/
  for(; ;){
    for(m=0;m<n;m++){
      dx = getrand(rmin,rmax)/1000000;
      dy = getrand(rmin,rmax)/1000000;
      dz = getrand(rmin,rmax)/1000000;
      y[3*m] += dx;
      y[3*m+1] += dy;
      y[3*m+2] += dz;
    }
    hamilton(y,p);

    if (H < wantH) break;
    else{
      for(m=0;m<n;m++){
	y[3*m] -= dx;
	y[3*m+1] -= dy;
	y[3*m+2] -= dz;
      }
    }
  }

  printf("エネルギーちょっとあげました\n");
  hamilton(y,p);
  printf("hamiltonian = %f",H);

  /*少しだけ動かしてみる*/
  for (t=0.0; t < 0.05; t += dist) {
    runge(t,y,p,dist);
  }

  hamilton(y,p);
  double c1;
  c1 = sqrt((wantH - LJ)/totallkinetic);
  for(i=0;i<n;i++){
    p[3*i] = c1 * p[3*i];
    p[3*i+1] = c1 * p[3*i+1];
    p[3*i+2] = c1 * p[3*i+2];
  }
}

/* Runge-Kutta structure*/
void runge(double t, double y[],double p[],double step)
{

    double s=step/2.0,
      a1Y[N], a2Y[N], a3Y[N],k1Y[N], k2Y[N], k3Y[N], k4Y[N],
      a1PY[N], a2PY[N], a3PY[N],k1PY[N], k2PY[N], k3PY[N], k4PY[N];

    for (i=0; i<N; i++){
      a1Y[i] =y[i]+0.5*(k1Y[i]=step*fy(t,y,p,i));
      a1PY[i] =p[i]+0.5*(k1PY[i]=step*fp(t,y,p,i));
    }
    for (i=0; i<N; i++){
      a2Y[i] =y[i]+0.5*(k2Y[i]=step*fy(t+s,a1Y,a1PY, i));
      a2PY[i] =p[i]+0.5*(k2PY[i]=step*fp(t+s,a1Y,a1PY, i));
    }
    for (i=0; i<N; i++){
      a3Y[i] =y[i]+    (k3Y[i]=step*fy(t+s,a2Y,a2PY, i));
      a3PY[i] =p[i]+  (k3PY[i]=step*fp(t+s,a2Y,a2PY, i));
    }
    for (i=0; i<N; i++){
      k4Y[i] =                  step*fy(t+step, a3Y,a3PY, i);
      k4PY[i] =                 step*fp(t+step, a3Y,a3PY, i);
    }

    for (i=0; i<N; i++){
      y[i] += (k1Y[i]+2.0*k2Y[i]+2.0*k3Y[i]+k4Y[i])/6.0;
      p[i] += (k1PY[i]+2.0*k2PY[i]+2.0*k3PY[i]+k4PY[i])/6.0;
    }
}


/*------hamiltonian eg--------*/
/*------calculate dy/dt-------*/
double fy(double t, double y[],double p[], int i)
{
  return (p[i]);
}
double fp(double t,double y[],double p[], int i)
{
  px=0;
  py=0; 
  pz=0;
  a=i/3;
  b=a*3;
  if (i%3 == 0) {
    px = 0;
    for (j=0;j<n;j++){
      if(b == 3*j) continue;  //自身以外のすべての分子との結果を足す。
      px += 48.0*(y[b]-y[3*j])/(pow(pow(y[b]-y[3*j],2.0)+pow(y[b+1]-y[3*j+1], 2.0)+pow(y[b+2]-y[3*j+2], 2.0), 7.0))
	-24.0*(y[b]-y[3*j])/(pow(pow(y[b]-y[3*j],2.0)+pow(y[b+1]-y[3*j+1], 2.0)+pow(y[b+2]-y[3*j+2], 2.0), 4.0));
    }
    return (px);
  }

    if (i%3 == 1) {
      py=0;
      for (j=0; j<n; j++) {
	if (b == 3*j) continue;                 //自身以外のすべての分子との結果を足す。
	py += 48.0*(y[b+1]-y[3*j+1])/(pow(pow(y[b]-y[3*j],2.0)+pow(y[b+1]-y[3*j+1], 2.0)+pow(y[b+2]-y[3*j+2], 2.0), 7.0))
	  -24.0*(y[b+1]-y[3*j+1])/(pow(pow(y[b]-y[3*j],2.0)+pow(y[b+1]-y[3*j+1], 2.0)+pow(y[b+2]-y[3*j+2], 2.0), 4.0));

      }
      return (py);
    }

    if (i%3 == 2) {
      pz=0;
      for (j=0; j<n; j++) {
	if (b == 3*j) continue;                 //自身以外のすべての分子との結果を足す。
	pz += 48.0*(y[b+2]-y[3*j+2])/(pow(pow(y[b]-y[3*j],2.0)+pow(y[b+1]-y[3*j+1], 2.0)+pow(y[b+2]-y[3*j+2], 2.0), 7.0))
	  -24.0*(y[b+2]-y[3*j+2])/(pow(pow(y[b]-y[3*j],2.0)+pow(y[b+1]-y[3*j+1], 2.0)+pow(y[b+2]-y[3*j+2], 2.0), 4.0));
      }

      return (pz);
    }
    else return 0;
}


  /*calculate tokuichi*/
int tokuichi(double y[]){
  int i,j;
  char jobz[]="A";
  long   lwork =  3*min(ldW,nW)*min(ldW,nW) + max(max(ldW,nW), 4*min(ldW,nW)*min(ldW,nW) + 4*min(ldW,nW));
  double work[lwork];
  long iwork[8*min(ldW,nW)];

    for(i=0;i<ldW*nW;i++){
    W[i] =0.0;
  }

  for(i=0;i<ldW*nW;i++){
    W[i] = y[i];
  }

  for(i=0;i<nW*nW;i++)
    {
      Ut[i]=0.0;
    }
  for(i=0;i<ldR*ldR;i++)
    {
      R[i] = 0.0;
    }

  for(i=0;i<3;i++)
    {
      gyradii[i]= 0.0;
	}
  dgesdd_(jobz, &ldW, &nW, W, &ldW,
	  gyradii, R, &ldR, Ut, &ldUt, work, &lwork, iwork, &info);

  if(info) {
    fprintf(stderr, "dgesdd_ error\n");
    fprintf(stderr, "dgesdd_ info = %ld\n", info);
    exit(1);
  }

  /*check
    char trans = 'N';
    double alpha,beta;
    double C[ldW*nW],RR[ldR*nW];
    for(i=0;i<ldR*nW;i++)RR[i]=0.0;
    for(i=0;i<3;i++){
    RR[3*i] = R[3*i] * gyradii[i];
    RR[3*i+1] = R[3*i+1] * gyradii[i];
    RR[3*i+2] = R[3*i+2] * gyradii[i];
    }
    alpha = 1.0; beta = 0.0; 
    dgemm_(&trans,&trans,&ldW,&nW,&nW,&alpha,RR,&ldR,Ut,&ldUt,&beta,C,&ldW);
    display_array(C,ldW,nW);
    --------print--------------*/

  return 0;

}



/*Input(y[])->jacobi vector(jaco[])を返す関数*/
/*ついでに重心の座標Gyと、重心の運動量Gpももとめる*/
void jacobi(double y[],double p[]){
  int i,j,k;
  double a;
  double nyu[n-1]; //sum[0] = sum 1-i,sum[1] = sum 1-i+1
  double mrsumy[3],mpsump[3];;
  /*初期値=0*/
  for(i=0;i<3*(n-1);i++)
    {
      jacoy[i] = 0.0;
      jacop[i] = 0.0;
    }

  for(i=0;i<n-1;i++){
    a = (double)i;
    nyu[i] = mass*(a+1)/(a+2);
  }

  for(i=0;i<n-1;i++){
    for(j=0;j<3;j++){
      mrsumy[j] =0.0;
      mpsump[j] =0.0;
    }
    for(k=0;k<i+1;k++){
      mrsumy[0] += y[3*k];
      mrsumy[1] += y[3*k+1];
      mrsumy[2] += y[3*k+2];

      mpsump[0] += p[3*k];
      mpsump[1] += p[3*k+1];
      mpsump[2] += p[3*k+2];


    }

    jacoy[3*i] = sqrt(nyu[i])*((mrsumy[0]/(i+1)) - y[3*(i+1)]);
    jacoy[3*i+1] = sqrt(nyu[i])*((mrsumy[1]/(i+1)) - y[3*(i+1)+1]);
    jacoy[3*i+2] = sqrt(nyu[i])*((mrsumy[2]/(i+1)) - y[3*(i+1)+2]);

    jacop[3*i] = sqrt(nyu[i])*((mpsump[0]/(i+1)) - p[3*(i+1)]);
    jacop[3*i+1] = sqrt(nyu[i])*((mpsump[1]/(i+1)) - p[3*(i+1)+1]);
    jacop[3*i+2] = sqrt(nyu[i])*((mpsump[2]/(i+1)) - p[3*(i+1)+2]);
  }

  for(i=0;i<3;i++)
    {
      mrsumy[i] = 0.0;
            mpsump[i] = 0.0;
        }
  for(k=0;k<n;k++){
      mrsumy[0] += y[3*k];
      mrsumy[1] += y[3*k+1];
      mrsumy[2] += y[3*k+2];

      mpsump[0] += p[3*k];
      mpsump[1] += p[3*k+1];
      mpsump[2] += p[3*k+2];
    }
  kineticG = 0.0;
  for(i=0;i<3;i++)
    {
      Gy[i] = mrsumy[i]/n;
      Gp[i] = mpsump[i]/n;
      kineticG += 0.50*n*pow(Gp[i],2);
    }
}

  //generate random numbers
double getrand(int rmin,int rmax){
  randx = 0;

  randx = rmin + (int)(rand()*(rmax-rmin+1.0)/(1.0+RAND_MAX));

  return randx;
}


/*微分したgyratioin radii,omega,ganmaを求める関数*/

void KaisekiHyperspherical(double y[],double p[])
{
  jacobi(y,p);
  tokuichi(jacoy);

  /*A*B=Cが基本*/
  char TRANSA[]="N";   /*転置＝T,転置しない=N,*/
  char TRANSB[]="N";
  int lda,ldb,ldc; /*A,B,Cそれぞれの転置前の行数*/
  int mm,nn,kk;
  double alpha,beta;

  alpha = 1.0;
  beta = 0.0;

  /*dgemm_=はC:=(alpha)A*B+(beta)Cを計算*/
  /*因数はdgemm_(A転置,B転置,左(A)の行数mm,右(B)の列数nn,左(A)の行列の列数kk,alpha*/
  /*             A,Aの行数lda,B,Bの行数ldb,beta,C,Cの行数ldc)*/
  /*!!!!!!!!下の段に書いたそれぞれA,B,Cが転置する前の行数を書くことに注意!!!!!!!*/

  /*--------------------------------------*/
  /*まずSdot=Wdot^T*W+W^T*Wdot*を計算する*/
  double dotS[(n-1)*(n-1)];
  double dotWTW[(n-1)*(n-1)],WTdotW[(n-1)*(n-1)];
  double dotW[3*(n-1)];

  dainyuu(W,jacoy,3*(n-1));
  dainyuu(dotW,jacop,3*(n-1));
  lda = 3;
  ldb = 3;
  ldc = n-1;
  /* dotWTW = dotW^T * W */
  TRANSA[0]='T';
  mm=n-1; nn=n-1; kk=3;
  dgemm_(TRANSA,TRANSB, &mm, &nn, &kk, &alpha, dotW, &lda, W, &ldb, &beta, dotWTW, &ldc);

  /* WTdotW = W^T * dotW */
  dainyuu(W,jacoy,3*(n-1));
  dainyuu(dotW,jacop,3*(n-1));
  TRANSA[0]='T';
  mm=n-1; nn=n-1; kk=3;
  dgemm_(TRANSA,TRANSB, &mm, &nn, &kk, &alpha, W, &lda, dotW, &ldb, &beta, WTdotW, &ldc);
  tasu(dotS,dotWTW,WTdotW,(n-1)*(n-1));

  /*-------------------------------------*/
  /*つぎにU^T*dotS*Uを計算する*/
  /*Ut＝U^Tであることに注意して計算をすすめる*/
  double UT[(n-1)*(n-1)];
  double UTdotS[(n-1)*(n-1)];
  double UTdotSU[(n-1)*(n-1)];
  dainyuu(UT,Ut,(n-1)*(n-1));
  mm=n-1;   nn=n-1;   kk=n-1;
  lda=n-1;  ldb
	      =n-1;  ldc=n-1;
  /*UTdotS=U^T*dotS*/
  TRANSA[0] = 'N';
  TRANSB[0] = 'N';
  dgemm_(TRANSA,TRANSB, &mm, &nn, &kk, &alpha, UT, &lda, dotS, &ldb, &beta, UTdotS, &ldc);
  /*UTdotSU= UTdotS * U  */
  TRANSA[0] ='N';
  TRANSB[0] = 'T'; /*transpose[UT]=U*/
  dainyuu(UT,Ut,(n-1)*(n-1));
  dgemm_(TRANSA,TRANSB, &mm, &nn, &kk, &alpha, UTdotS, &lda, UT, &ldb, &beta, UTdotSU, &ldc);

  /*---------------------------------------*/
  difgyradii[0]=UTdotSU[0]/(2*gyradii[0]);
  difgyradii[1]=UTdotSU[n]/(2*gyradii[1]);
  difgyradii[2]=UTdotSU[2*n]/(2*gyradii[2]);

  /*omega[3]=transpose(w23,w31,w12)*/
  omega[0]=UTdotSU[n+1]/(pow(gyradii[1],2)-pow(gyradii[2],2));
  omega[1]=UTdotSU[2*(n-1)]/(pow(gyradii[2],2)-pow(gyradii[0],2));
  omega[2]=UTdotSU[1]/(pow(gyradii[0],2)-pow(gyradii[1],2));

  /*ganma[(n-4)*3]=(ganma1 ganma2 ganma3)*/
  /*ganma1=ganma[0--(n-3)]*/
  /*ganma2=ganma[1*(n-4)--((n-4)+(n-3))]*/
  /*ganma3=ganma[2*(n-4)--(2*(n-4)+(n-3))]*/
  for(i=0;i<3;i++)
    {
      for(k=0;k<n-4;k++)
	{
	  ganma[i*(n-4)+k] = UTdotSU[3+i*(n-1)+k]/pow(gyradii[i],2);
	}
    }
  /*calculate kinetic energy */
  double kinetica,kineticw,kineticv,totallganma;
  kinetica = 0.0;   kineticw = 0.0;   kineticv = 0.0;
  kinetica = 0.50*(pow(difgyradii[0],2)+pow(difgyradii[1],2)+pow(difgyradii[2],2));
  kineticw =( pow( pow(gyradii[0],2) - pow(gyradii[1],2) ,2 )/( 2 * ( pow( gyradii[0],2 ) + pow(gyradii[1],2)) ) ) * pow(omega[2],2)
    +( pow( pow(gyradii[1],2) - pow(gyradii[2],2) ,2)/( 2 * ( pow( gyradii[1],2 ) + pow(gyradii[2],2)) ) ) * pow(omega[0],2)
    +( pow( pow(gyradii[2],2) - pow(gyradii[0],2) ,2)/( 2 * ( pow( gyradii[2],2 ) + pow(gyradii[0],2)) ) ) * pow(omega[1],2);
  for(i=0;i<3;i++)    {
    totallganma = 0.0;
    for(k=0;k<n-4;k++){
      totallganma += pow(ganma[i*(n-4)+k],2);
    }
    kineticv += 0.50*pow(gyradii[i],2)*totallganma;
  }
  kineticE = kinetica + kineticw + kineticv;

  /*calculate force*/
  /*force from potential*/

  /*force from cent*/
  for(i=0;i<3;i++)  fcenti[i] = 0.0;

  fcenti[0] = pow(omega[2],2)*gyradii[0]*(pow(gyradii[0],2)+3*pow(gyradii[1],2))*(pow(gyradii[0],2)-pow(gyradii[1],2))/pow( pow(gyradii[0],2) + pow(gyradii[1],2),2)
    +pow(omega[1],2)*gyradii[0]*(pow(gyradii[0],2)+3*pow(gyradii[2],2))*(pow(gyradii[0],2)-pow(gyradii[2],2))/pow( pow(gyradii[0],2) + pow(gyradii[2],2),2);
  fcenti[1] = pow(omega[2],2)*gyradii[1]*(pow(gyradii[1],2)+3*pow(gyradii[0],2))*(pow(gyradii[1],2)-pow(gyradii[0],2))/pow( pow(gyradii[1],2) + pow(gyradii[0],2),2)
    +pow(omega[0],2)*gyradii[1]*(pow(gyradii[1],2)+3*pow(gyradii[2],2))*(pow(gyradii[1],2)-pow(gyradii[2],2))/pow( pow(gyradii[1],2) + pow(gyradii[2],2),2);
  fcenti[2] = pow(omega[1],2)*gyradii[2]*(pow(gyradii[2],2)+3*pow(gyradii[0],2))*(pow(gyradii[2],2)-pow(gyradii[0],2))/pow( pow(gyradii[2],2) + pow(gyradii[0],2),2)
    +pow(omega[0],2)*gyradii[2]*(pow(gyradii[2],2)+3*pow(gyradii[1],2))*(pow(gyradii[2],2)-pow(gyradii[1],2))/pow( pow(gyradii[2],2) + pow(gyradii[1],2),2);

  for(i=0;i<3;i++){
    totallganma = 0.0;
    for(k=0;k<n-4;k++){
      totallganma += pow(ganma[i*(n-4)+k],2);
    }
    fcenti[i] += gyradii[i]*totallganma;
  }

  fcent = sqrt( pow(fcenti[0],2) + pow(fcenti[1],2) + pow(fcenti[2],2) );

}
