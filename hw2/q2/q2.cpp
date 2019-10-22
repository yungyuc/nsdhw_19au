//
// Created by Yarin on 09-Oct-19.
//

#include <pybind11/pybind11.h>

#include <math.h>
#include <cstdio>
#include <iostream>
#include <limits>


#define PI 3.14159265359

float rads(std::pair<float, float> v1, std::pair<float, float> v2){

    if((v1.first == 0 and v1.second == 0) or (v2.first == 0 and v2.second == 0))
    	return std::numeric_limits<float>::quiet_NaN();

    if(v1.first / v2.first == v1.second / v2.second)
    	return 0;

    float norm1 = sqrt((v1.first * v1.first) + (v1.second * v1.second));
    float norm2 = sqrt((v2.first * v2.first) + (v2.second * v2.second));

    float rad = acos(((v1.first * v2.first) + (v1.second * v2.second)) / (norm1 * norm2));

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

