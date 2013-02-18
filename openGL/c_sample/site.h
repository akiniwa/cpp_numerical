//
//  temp.h
//  c_sample
//
//  Created by Shinya Akiba on 12/09/21.
//  Copyright (c) 2012年 Shinya Akiba. All rights reserved.
//

#ifndef c_sample_temp_h
#define c_sample_temp_h

#define F_TH 1.0
#define ALPHA 0.23

class Site
{
private:
    int index;
public:
    Site(int index, int earthQuake_count);
    // 格子点(二次元配列)
    double **grid;
    // 地震の発生回数。
    int earthQuake_count;
    // それぞれの地震の強さ。
    int *count;
    // 二次元配列のMAXを探す。二次元配列と配列番号が引数。
    double findMax();
    
    void release(int eq_count);
    void initialQuake(int eq_count, double max_site);
    // カウントを一つ一つ返していく。
    void showForce();
};


#endif
