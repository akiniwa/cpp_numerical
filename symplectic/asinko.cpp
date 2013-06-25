#include <stdio.h>
#include <math.h>

#define N 6
#define M 1
#define G 1
#define J2 1082.264E-6
#define aE 1

void runge(double t,double y[],double step);
double f(double t,double y[],int i);

int main () 
{
	double t,y[6],dt,tmax;
	
	/*   y[0]=x, y[1]=y, y[2]=z, y[3]=vx, y[4]=vy, y[5]=vz	*/
	
	y[0]=1.0;
	y[1]=0.0;
	y[2]=0.0;
	y[3]=0.0;
	y[4]=1.1;
	y[5]=0.0;
	dt=0.001;
	tmax=100.0;
	
	FILE *output0,*output1,*output2;
	
	output0=fopen("test0.txt","w");
	output1=fopen("test1.txt","w");
	output2=fopen("test2.txt","w");
	
	for (t=0; t<tmax; t += dt) {
		
		runge(t, y, dt);
		
		double r = sqrt(pow(y[0], 2)+ pow(y[1], 2)+ pow(y[2], 2));	//rの距離
		double lxy = 1/M * (y[0]*y[4]-y[1]*y[3]);	//角運動量÷M （x−y）
		double lyz = 1/M * (y[1]*y[5]-y[2]*y[4]);	//角運動量÷M （y−z）
		double lzx = 1/M * (y[2]*y[3]-y[0]*y[5]);	//角運動量÷M （z-x)
		
		/*------------ 軌道要素 a,e,I,OMEGA,omega,t0 ここから ------------*/
		double a = pow(2/r-(pow(y[3],2)+pow(y[4],2)+pow(y[5],2))/(G*M),-1);		//長半径 a
		double h = pow(lxy*lxy+lyz*lyz+lzx*lzx,0.5);	//全角運動量 h
		double e = pow((1-h*h/(G*M*a)),0.5);	//離心率e
		double I = acos(lxy/h);	//軌道傾斜角 I
		double OMEGA = asin(lyz/(sin(I)*h));	//昇交点経度 Ω
//		double omega = 
		/*------------ 軌道要素 a,e,I,OMEGA,omega,t0 ここまで ------------*/
		
		/*------------エネルギー計算 ここから------------*/
		double KE = (0.5*y[3]*y[3]+ 0.5*y[4]*y[4]+ 0.5*y[5]*y[5]);	//運動エネルギー
		double PE = -G*M/r;	//ポテンシャルエネルギー（摂動なし）
//		double PE = -G*M/r*(1+G*M*pow(aE,2)/pow(r,3)*J2*(1.5*pow(y[2]/r,2)-0.5));	//ポテンシャルエネルギー（摂動あり）
		printf("t=%f KE+PE=%f KE=%f PE=%f a=%f e=%f h=%f\n",t,KE+PE,KE,PE,a,e,h);	//全エネルギーをチェック
		/*------------エネルギー計算 ここまで------------*/
		
		fprintf(output0,"%f %f\n",t,KE+PE);
		fprintf(output1,"%f %f %f\n",y[0],y[1],y[2]);
		fprintf(output2,"%f %f\n",t,OMEGA);
//		fprintf(output2,"%f %f %f\n",y[0],y[1]);	
	}
	
	fclose(output0);
	fclose(output1);
	fclose(output2);
	
    return 0;
}

void runge(double t,double y[],double step)
{
	double	h=step/2.0;
	double	s1[N], s2[N], s3[N],
	k1[N], k2[N], k3[N], k4[N];
	int i;
	
	for(i=0; i<N; i++) s1[i] = y[i]+0.5*(k1[i]=step*f(t,y,i));
	for(i=0; i<N; i++) s2[i] = y[i]+0.5*(k2[i]=step*f(t+h,s1,i));
	for(i=0; i<N; i++) s3[i] = y[i]+	(k3[i]=step*f(t+h,s2,i));
	for(i=0; i<N; i++) k4[i] =                 step*f(t+step,s3,i);
	
	for(i=0; i<N; i++) y[i] += (k1[i]+2*k2[i]+2*k3[i]+k4[i])/6.0;
}


double f(double t,double y[], int i)
{
	double r = sqrt(pow(y[0], 2)+ pow(y[1], 2)); //rの距離
	
	if (i == 0) {
		return(y[3]);
	} else if (i == 1) {
		return (y[4]);
	} else if (i == 2) {
		return (y[5]);
	} else if (i == 3) {
		return (-G/pow(r,3)*M*y[0]);
//		return (-G/pow(r,3)*M*y[0]-(G*M)*(7.5*J2*(pow(aE,2)*pow(y[2],2)/pow(r,6))+1.5*J2*pow(aE,2)/pow(r,4))*y[0]/r);	//J2あり

	} else if (i == 4) {
		return (-G/pow(r,3)*M*y[1]);
//		return (-G/pow(r,3)*M*y[1]-(G*M/r)*(7.5*J2*(pow(aE,2)*pow(y[2],2)/pow(r,6))+1.5*J2*pow(aE,2)/pow(r,4))*y[1]/r);	//J2あり

	} else if (i == 5) {
		return (-G/pow(r,3)*M*y[2]);
//		return (-G/pow(r,3)*M*y[2]-(G*M/r)*(7.5*J2*(pow(aE,2)*pow(y[2],2)/pow(r,6))+1.5*J2*pow(aE,2)/pow(r,4))*y[2]/r);	//J2あり

	} else {
		printf("unexpected error \n");
		return 0;
	}
}

/*
 //	単振動
 double f(double t,double y[], int i)
 {
 if(i == 0){return(y[1]);}
 else if (i == 1){return (-y[0]);}
 else {printf("unexpected error \n");return 0;}
 }
 */
