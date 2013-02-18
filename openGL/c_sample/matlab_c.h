//
//  matlab_c.h
//  c_sample
//
//  Created by Shinya Akiba on 12/09/21.
//  Copyright (c) 2012年 Shinya Akiba. All rights reserved.
//

#ifndef c_sample_matlab_c_h
#define c_sample_matlab_c_h

int max (int *array, int index) {
    int max_int;
    max_int = array[0];
    
    for (int i=0;i<index;i++) {
        if (max_int < array[i]) {
            max_int = array[i];
        }
    }
    return max_int;
}

double max (double *array, int index) {
    double max_double;
    max_double = array[0];

    for (int i=0;i<index;i++) {
        if (max_double < array[i]) {
            max_double = array[i];
        }
    }
    
    return max_double;
}

// int型の二次元配列
int max (int **array, int index) {
    int max_int;
    max_int = array[0][0];
    
    for (int i=0;i<index;i++) {
        for (int j=0;j<index;j++) {
            if (max_int < array[i][j]) {
                max_int = array[i][j];
            }
        }
    }
    return max_int;
}

double max (double **array, int index) {
    double max_double;
    max_double = array[0][0];
    
    for (int i=0;i<index;i++) {
        for (int j=0;j<index;j++) {
            if (max_double < array[i][j]) {
                max_double = array[i][j];
            }
        }
    }
    return max_double;
}

#endif
