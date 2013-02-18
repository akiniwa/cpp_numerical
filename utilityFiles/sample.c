#include <stdio.h>

int main(void)
{
FILE *gp;

gp = popen("gnuplot -persist", "w");	// パイプを開き、gnuplotの立ち上げ

fprintf(gp, "plot sin(x)\n");	// パイプを通してgnuplotにコマンドを送る

fprintf(gp, " exit\n");	 // gnuplotの終了
fflush(gp);
pclose(gp);	// パイプを閉じる

return 0;
}
