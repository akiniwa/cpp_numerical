#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "openGLsample.hpp"
#include "satellite.hpp"
#include <time.h>
//---- main function
int main(int argc, char *argv[])
{
    clock_t start, end;
    start = clock();

    FILE *fp;
    FILE *fp2;
    if (argc <= 1) {
        printf("not enough argument\n");
        exit(0);
    }
    if ((fp = fopen(argv[1], "w")) == NULL) {
        printf("file cannot open\n");
        exit(0);
    }
    if ((fp2 = fopen(argv[2], "w")) == NULL) {
        printf("file cannot open\n");
        exit(0);
    }
    Satellite* debris;
    debris = new Satellite();
    double previous;
    int count = 0;

    for(int t=0; t<1000000; t++){
        double r = hypot(hypot(debris->qx, debris->qy), debris->qz);
        double p = hypot(hypot(debris->px, debris->py), debris->pz);

        double q = hypot(debris->qz , debris->qx);
        double theta = atan2(debris->qy, debris->qx) * 180.0 / M_PI;
        if (theta<0) {
            theta += 360;
        }
        if (t%50==0) {
            if (previous>359.0 && theta<1.0) {
                count++;
            }
            double KE = debris->getKineticE();
            double UE = debris->getPotentialE();
            fprintf(fp, "%f %f %f %f %f %f %f\n", debris->qx, debris->qy, debris->qz, debris->px, debris->py, debris->pz, KE+UE);
            fprintf(fp2, "%f %f %f %f %f\n", theta+count*360, p, KE, UE, KE + UE);
            previous = theta;
        }
       debris->motion();
   }
    fclose(fp);

    end = clock();
    printf("%.10lf[s]\n", (double)(end - start)/CLOCKS_PER_SEC);

    /*
    glutInit(&argc, argv);
    openGL(argv[1]);
    */
}
