//
// Created by Yarin on 09-Oct-19.
//

#include <pybind11/pybind11.h>

#include <math.h>
#include <cstdio>
#include <iostream>



#define PI 3.14159265359

float rads(double* v1, double* v2){

    float norm1 = sqrt((v1[0] * v1[0]) + (v1[1] * v1[1]));
    float norm2 = sqrt((v2[0] * v2[0]) + (v2[1] * v2[1]));

    float rad = acos(((v1[0] * v2[0]) + (v1[1] * v2[1])) / (norm1 * norm2));

    if(rad < 0 ) rad = -rad;

    if (rad > PI){
        rad = (2 * PI) - rad;
    }

    return rad;
}

PYBIND11_MODULE(q2, mod){
    mod.doc() = "The angle is";
    mod.def("rads", &rads, "The angle is");
}

