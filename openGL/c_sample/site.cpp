//
//  temp.cpp
//  c_sample
//
//  Created by Shinya Akiba on 12/09/21.
//  Copyright (c) 2012年 Shinya Akiba. All rights reserved.
//

#include <iostream>
#include "site.h"
#include <math.h>
#include "matlab_c.h"

Site::Site(int index, int earthQuake_count){
    //コンストラクタで初期化
    // 二次元配列の初期化。少々面倒。
    this->index = index;
    this->grid = new double*[index];

    for (int i=0; i<this->index; i++) {
        this->grid[i] = new double[index];
    }
    
    for (int i=0;i<this->index;i++) {
        for (int j=0;j<this->index;j++) {
            if (i==0 || j==0 || i==(this->index-1) || j==(this->index-1)) {
                this->grid[i][j] = 0.0;
            } else {
                this->grid[i][j] = rand() * (1.0) / (1.0 + RAND_MAX);
            }
        }
    }
    
    this->earthQuake_count = earthQuake_count;
    this->count = new int[earthQuake_count];
    for (int i=0;i<this->earthQuake_count;i++) {
        this->count[i] = 0;
    }
}

double Site::findMax() {
    double max_site;
    // 関数maxはmatlab_c.hの関数。
    max_site = max(this->grid, this->index);

    return max_site;
}

void Site::release(int eq_count) {
    for (int i=0;i<this->index;i++) {
        for (int j=0;j<this->index;j++) {
            if (this->grid[i][j] >= F_TH) {
                this->grid[i-1][j] += ALPHA * this->grid[i][j];
                this->grid[i+1][j] += ALPHA * this->grid[i][j];
                this->grid[i][j-1] += ALPHA * this->grid[i][j];
                this->grid[i][j+1] += ALPHA * this->grid[i][j];
                this->grid[i][j] = 0.0;

                this->count[eq_count]++;
                //printf("count:%d force:%d\n", eq_count, count[eq_count]);
            }
            if (i==0 || j==0 || i==(index-1) || j==(index-1)) {
                grid[i][j] = 0.0;
            }
        }
    }
}

void Site::initialQuake(int eq_count, double max_site) {
    double plus;
    plus = F_TH - max_site;
    // 全部に値を加える。
    for (int i=0;i<index;i++) {
        for (int j=0;j<index;j++) {
            grid[i][j] += plus;
        }
    }
    count[eq_count]++;
   // printf("count:%d force:%d\n", eq_count, count[eq_count]);
}

void Site::showForce() {
    
}
