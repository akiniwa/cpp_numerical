#include <stdio.h>
#include <math.h>
#include <time.h>

void runge(double t,double y[],double step);
double f(double t,double y[],int i);

double L = 6.3781E6;	//地球の半径
double G = 6.6743E-11;	//重力定数
double M = 5.9722E24;	//地球の質量
int T = 23*3600+56*60+4;//地球の自転周期
int N = 6;	 //ルンゲクッタで用いる
int JJ = 2;	 //「JJ=0」でJ2項なし、「JJ=2」でJ2項あり
const double J2 = 1082.264E-6;	//J2係数

int main () {
    clock_t start, end;
    start = clock();
    double t,y[6],dt,tmax,T1;
    /*   y[0]=X, y[1]=Y, y[2]=Z, y[3]=Vx, y[4]=Vy, y[5]=Vz */
    double geo_radius = pow(G*M*pow((T/(2*M_PI)), 2), 1/3.0);
    double geo_velocity = geo_radius*2*M_PI/T;
    double qx, qy, qz, vx, vy, vz;
    qx = geo_radius;
    qy = 0.0;
    qz = 0.0;
    vx = 0.0;
    vy = geo_velocity;
    vz = 0.0;
    y[0]=qx/L;  //無次元 座標X
    y[1]=qy/L;  //無次元 座標Y
    y[2]=qz/L;  //無次元 座標Z
    y[3]=vx/sqrt(M*G/L);    //無次元 速度Vx
    y[4]=vy/sqrt(M*G/L);    //無次元 速度Vy
    y[5]=vz/sqrt(M*G/L);    //無次元 速度Vz

    T1=T/sqrt(pow(L,3)/(M*G));	//無次元化 地球自転周期
    dt=0.001;	//無次元時間 刻み幅
    tmax=60;
    FILE *output0,*output1,*output2,*output3;
    output0=fopen("Spica.txt","w");	 //乙女座スピカ＝エネルギー
    output1=fopen("Regulus.txt","w");	//獅子座レグルス＝軌道
    output2=fopen("Pollux.txt","w");	//双子座ポルックス
    output3=fopen("Antares.txt","w");	//蠍座アンタレス
    int count = 0;
    for (t=0; t<tmax; t += dt) {
        double r = sqrt(pow(y[0], 2)+ pow(y[1], 2)+ pow(y[2], 2));	//rの距離
        double lxy = 1/M * (y[0]*y[4]-y[1]*y[3]);	 //角運動量÷M （x−y）
        double lyz = 1/M * (y[1]*y[5]-y[2]*y[4]);	 //角運動量÷M （y−z）
        double lzx = 1/M * (y[2]*y[3]-y[0]*y[5]);	 //角運動量÷M （z-x)
        /*------------エネルギー計算 ここから------------*/
        double KE = (0.5*y[3]*y[3]+ 0.5*y[4]*y[4]+ 0.5*y[5]*y[5]);	//運動エネルギー
        double PE = -pow(r*L,-1);	 //ポテンシャルエネルギー（摂動あり）
        double R;
        if (JJ == 0) {
            R = 0.0;	 //J2なし
        } else if (JJ == 2){
            R = -G*M*J2/(L*pow(r,3))*(1.5*pow(y[2],2)/(pow(r,2)*pow(L,4))-0.5);	//J2あり
        }
        /*------------エネルギー計算 ここまで------------*/

        /*------------ 軌道要素 a,e,I,OMEGA,omega,t0 ここから ------------*/
        double a = pow(2/r-(pow(y[3], 2)+pow(y[4], 2)+pow(y[5], 2)),-1);	 //長半径 a
        double h = pow(lxy*lxy+lyz*lyz+lzx*lzx,0.5);	 //全角運動量 h →"sqrt(G*M*a*(1-e*e))"とも書けるらしい
        double e = pow((1-h*h/(G*M*a)),0.5);	 //離心率e
        double I = acos(lxy/h);	 //軌道傾斜角 I
        double OMEGA = asin(lyz/(sin(I)*h));	 //昇交点経度 OMEGA
        double omega = acos( 1/e *
                (
                 (h/(G*M)*(-y[3]*sin(OMEGA)+y[4]*cos(OMEGA))*cos(I)
                  + y[5]*sin(I))-(y[0]*cos(OMEGA)+y[1]*sin(OMEGA))/r
                )
                );	 //近点引数 omega
        /*------------ 軌道要素 a,e,I,OMEGA,omega,t0 ここまで ------------*/
        //	 printf("t=%f KE+PE=%f KE=%f PE=%f a=%f e=%f h=%f I= %f lxy=%f　lyz=%f lzx=%f\n",t,KE+PE,KE,PE,a,e,h,I,lxy,lyz,lzx);	//各値をprintfでチェック
        double theta = 2*M_PI/T1*t;	 //無次元化 地球自転角度
        double lambda = atan(y[1]/y[0]);	 //無次元化 惑星角度
        double T2 = pow(2*a/(qx/L),1.5)*T1;	 //無次元化 惑星周期
        /* T2式内で、本来は a のところを 2*a として、都合あわせしていることを注意 */

        /*------------fprintf ここから------------*/
        if (count%250==0) {	 //データ打ち込みは250ループに１度
            fprintf(output0,"%f %f\n",t,KE+PE+R);
            fprintf(output1,"%.5f %.5f %.5f %.5f\n",y[0],y[1],y[2], a);
            fprintf(output2,"%f %f\n",a*cos(lambda-theta),a*sin(lambda-theta));	//cos(λ-θ)とsin(λ-θ)
            fprintf(output3,"%f %f %f %f\n",t,T1,T2,sin(theta));
        }
        count++;
        /*------------fprintf ここまで------------*/
        runge(t, y, dt);
    }
    fclose(output0);
    fclose(output1);
    fclose(output2);
    fclose(output3);
    end = clock();
    printf("\n%.10lf[s]\n", (double)(end - start)/CLOCKS_PER_SEC);
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
    double r = sqrt(pow(y[0], 2)+ pow(y[1], 2)+ pow(y[2], 2));	 //rの距離
    double RXBIBUN = -pow(L,2)*J2*(-7.5*pow(y[2],2)/pow(r,6)+1.5/pow(r,4))*y[0]/r/(G*M*L);  //Rのxによる微分項無次元化（÷GML）
    double RYBIBUN = -pow(L,2)*J2*(-7.5*pow(y[2],2)/pow(r,6)+1.5/pow(r,4))*y[1]/r/(G*M*L);  //Rのyによる微分項無次元化（÷GML）
    double RZBIBUN = -pow(L,2)*J2*(3*y[2]/pow(r,5)-7.5*pow(y[2],3)/pow(r,7)+
            1.5*y[2]/pow(r,5))/(G*M*L);	 //Rのzによる微分項無次元化（÷GML）
    if (i == 0) {
        return(y[3]);
    } else if (i == 1) {
        return (y[4]);
    } else if (i == 2) {
        return (y[5]);
    } else if (i == 3) {
        if (JJ == 0) {
            return (-y[0]/pow(r,3));	 //J2項なし
        } else if (JJ == 2) {
            return (-y[0]/pow(r,3)-RXBIBUN);//J2項あり
        }
    } else if (i == 4) {
        if (JJ == 0) {
            return (-y[1]/pow(r,3));	 //J2項なし
        } else if (JJ == 2) {
            return (-y[1]/pow(r,3)-RYBIBUN);//J2項あり
        }
    } else if (i == 5) {
        if (JJ == 0) {
            return (-y[2]/pow(r,3));	 //J2項なし
        } else if (JJ == 2) {
            return (-y[2]/pow(r,3)-RZBIBUN);//J2項あり
        }
    } else {
        printf("unexpected error \n");
        return 0;
    }
}
