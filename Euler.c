#include <stdio.h>

// (角)速度の微小変化を返す
double dv(double v, double dt) {
    return v * dt;
}

// ここを変化させることで、いろんな系の計算ができる。
// 例えば、バネなら-k*x
double dx(double x, double dt) {
    return -x * dt;
}

int main(void) {
    // 座標と(角)速度を初期化
    double x = 1.0;
    double v = 0.0;
    double dt = 0.01;

    int count = 0;

    FILE* f;
    f = fopen("test.txt", "w");

    for (count = 0; count<10000; count++) {

        v = v + dv(x, dt);
        x = x + dx(v, dt);

        // 1ステップごとにファイルに書き込む
        fprintf(f, "%f %f \n", x, v);
    }
    fclose(f);

    return 0;
}

/* 
 * x = con(t) について解く。
 *
 * 1. 基本となる微分方程式
 * dx/dt = v
 * dv/dt = -x
 *
 * 2. 上の式を変形して
 * dx = v * dt
 * dv = -x * dt
 *
 * 3. 座標xと速度vは微少量を足すことで、近似的に変化していく。
 * x = x + dx
 * v = v + dv
 */
