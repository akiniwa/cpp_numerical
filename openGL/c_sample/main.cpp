//
//  main.cpp
//  c_sample
//
//  Created by Shinya Akiba on 12/09/21.
//  Copyright (c) 2012年 Shinya Akiba. All rights reserved.
//

#include <iostream>
#include <boost/random.hpp>
#include <math.h>
#include "site.h"

using namespace std;

int main()
{
    cout << "Spring Blockのプログラムを起動します。SITEの格子数を入力してください。" << endl;
    int index;
    cin >> index;
    
    boost::minstd_rand gen(42);
    
    
    cout << "地震の回数を入力してください。" << endl;
    
    int earthQuake_count;
    cin >> earthQuake_count;
    
    // Siteクラスのsiteインスタンス生成。
    Site site(index, earthQuake_count);
    close(index);
    
    cout << "Siteクラスのsiteインスタンス生成。" << endl;

    for (int eq_count=0;eq_count<=earthQuake_count;eq_count++) {
        while (1) {
            // 最大値を調べる。
            double max_site = site.findMax();
            if (max_site >= F_TH) {
                // 最大値がしきい値を超えてたら
                // 現在の地震が何番目かを教える。
                site.release(eq_count);
            } else {
                // 最大値がしきい値を超えていなかったら
                site.initialQuake(eq_count, max_site);
                close(max_site);
                break;
            }
        }//end while
    }//end for EARTH_COUNT
    
    // 時系列シミュレーション
    FILE *fp = popen("/Applications/gnuplot.app/gnuplot -persist", "w");
    if (fp == NULL) {
        return -1;
    } else {
        fprintf(fp, "plot '-' with lines linetype 1\n");
        for (int i=0;i< earthQuake_count;i++) {
            fprintf(fp, "%d %d\n", i, site.count[i]);
        }
        pclose(fp);
    }

    // パワーローのvidualize
    int x[earthQuake_count];
    for (int i=0;i<earthQuake_count;i++) {
        x[i]=0;
    }
    for (int i=0;i< earthQuake_count;i++) {
        x[site.count[i]]++;
    }
    
    FILE *fp2 = popen("/Applications/gnuplot.app/gnuplot -persist", "w");
    
    fprintf(fp2, "set logscale xy \n");
    fprintf(fp2, "plot '-' with points pt 7 ps 1.5\n");
    for (int i=0;i< (sizeof x /sizeof x[0]);i++) {
        fprintf(fp2, "%d %f\n", i, (double)x[i]/((double)earthQuake_count));
    }
    
    pclose(fp2);
}
