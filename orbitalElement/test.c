#include <stdio.h>
#include <math.h>
#include <time.h>

#define N 6
#define M 1
#define G 1
#define J2 1082.264E-6
//#define J2 0.5
#define aE 1

void runge(double t,double y[],double step);
double f(double t,double y[],int i);

int main () 
{
    clock_t start, end;
    start = clock();

	double t,y[6],dt,tmax;
	
	/*   y[0]=x, y[1]=y, y[2]=z, y[3]=vx, y[4]=vy, y[5]=vz	*/
	
	y[0]=1.0;	//x座標
	y[1]=0.1;	//y座標
	y[2]=1.0;	//z座標
	y[3]=0.1;	//速度vのx方向
	y[4]=1.0;	//速度vのy方向
	y[5]=0.1;	//速度vのz方向
	dt=0.001;
	tmax=1000.0;
	
	FILE *output0,*output1,*output2,*output3;
	
	output0=fopen("test0.txt","w");
	output1=fopen("test1.txt","w");
	output2=fopen("test2.txt","w");
	output3=fopen("test3.txt","w");
    int count = 0;
	
	for (t=0; t<tmax; t += dt) {
		
//		runge(t, y, dt);											//初期値をデータに書き込”まない”場合のルンゲクッタ
		
		double r = sqrt(pow(y[0], 2)+ pow(y[1], 2)+ pow(y[2], 2));	//rの距離
		double lxy = 1/M * (y[0]*y[4]-y[1]*y[3]);					//角運動量÷M （x−y）
		double lyz = 1/M * (y[1]*y[5]-y[2]*y[4]);					//角運動量÷M （y−z）
		double lzx = 1/M * (y[2]*y[3]-y[0]*y[5]);					//角運動量÷M （z-x)
		
		/*------------ 軌道要素 a,e,I,OMEGA,omega,t0 ここから ------------*/
		double a = pow(2/r-(pow(y[3],2)+pow(y[4],2)+pow(y[5],2))/(G*M),-1);		//長半径 a
		double h = pow(lxy*lxy+lyz*lyz+lzx*lzx,0.5);							//全角運動量 h →"sqrt(G*M*a*(1-e*e))"とも書けるらしい
		double e = pow((1-h*h/(G*M*a)),0.5);									//離心率e
		double I = acos(lxy/h);													//軌道傾斜角 I
		double OMEGA = asin(lyz/(sin(I)*h));									//昇交点経度 OMEGA
		double omega = acos( 1/e * 
							(
							(h/(G*M)*(-y[3]*sin(OMEGA)+y[4]*cos(OMEGA))*cos(I)
							 + y[5]*sin(I))-(y[0]*cos(OMEGA)+y[1]*sin(OMEGA))/r 
							)
						   );													//近点引数 omega
		/*------------ 軌道要素 a,e,I,OMEGA,omega,t0 ここまで ------------*/
		
		/*------------エネルギー計算 ここから------------*/
		double KE = (0.5*y[3]*y[3]+ 0.5*y[4]*y[4]+ 0.5*y[5]*y[5]);					//運動エネルギー
//		double PE = -G*M/r;															//ポテンシャルエネルギー（摂動なし）
   //     double R = 0.0;
		double R = -G*M*pow(aE,2)*J2/pow(r,3)*(1.5*pow(y[2]/r,2)-0.5);				//J2項による摂動関数 R
		double PE = -G*M/r+R;														//ポテンシャルエネルギー（摂動あり）
		/*------------エネルギー計算 ここまで------------*/
//		printf("t=%f KE+PE=%f KE=%f PE=%f a=%f e=%f h=%f I= %f lxy=%f　lyz=%f lzx=%f\n",t,KE+PE,KE,PE,a,e,h,I,lxy,lyz,lzx);	//各値をprintfでチェック
		
        if (count%500==0) {
//            fprintf(output0,"%f %f\n",t,KE+PE);
            fprintf(output1,"%.50f, %.50f, %.50f, %.50f\n",y[0],y[1],y[2], KE+PE);
 //          fprintf(output2,"%f %f\n",t,I);
 //           fprintf(output3,"%f %f\n",t,a);
        }
        count++;

		runge(t, y, dt);											//初期値をデータに書き込”む”場合のルンゲクッタ
		
	}
	
	fclose(output0);
	fclose(output1);
	fclose(output2);
	fclose(output3);
    end = clock();
    printf("%.10lf[s]\n", (double)(end - start)/CLOCKS_PER_SEC);
	
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
	double r = sqrt(pow(y[0], 2)+ pow(y[1], 2)+ pow(y[2], 2));							//rの距離
	double RXBIBUN = -G*M*pow(aE,2)*J2*(-7.5*pow(y[2],2)/pow(r,6)+1.5/pow(r,4))*y[0]/r;	//Rのxによる微分項
	double RYBIBUN = -G*M*pow(aE,2)*J2*(-7.5*pow(y[2],2)/pow(r,6)+1.5/pow(r,4))*y[1]/r;	//Rのyによる微分項
	double RZBIBUN = -G*M*pow(aE,2)*J2*(
					3*y[2]/pow(r,5)-7.5*pow(y[2],3)/pow(r,7)+
										1.5*y[2]/pow(r,5));
    //Rのzによる微分項
	
	if (i == 0) {
		return(y[3]);
	} else if (i == 1) {
		return (y[4]);
	} else if (i == 2) {
		return (y[5]);
	} else if (i == 3) {
//     	return (-G*M*y[0]/pow(r,3));			//摂動なしケプラー運動
		return (-G*M*y[0]/pow(r,3)-RXBIBUN);	//J2あり
	} else if (i == 4) {
//		return (-G*M*y[1]/pow(r,3));			//摂動なしケプラー運動
		return (-G*M*y[1]/pow(r,3)-RYBIBUN);	//J2あり
	} else if (i == 5) {
//		return (-G*M*y[2]/pow(r,3));			//摂動なしケプラー運動
		return (-G*M*y[2]/pow(r,3)-RZBIBUN);	//J2あり
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
