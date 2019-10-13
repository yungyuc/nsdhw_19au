#include <cmath>
#include <iostream>
#include "../Q1/line.h"

using namespace std;
float dot(Line vec1, Line vec2)
{
    return vec1.x(0) * vec2.x(0) + vec1.y(0) * vec2.y(0);
}
float getAngle(Line vec1, Line vec2)
{
    float dot_result = dot(vec1, vec2);
    float vec1_len = sqrt(dot(vec1, vec1));
    float vec2_len = sqrt(dot(vec2, vec2));
    return acos(dot_result / (vec1_len * vec2_len));
}

int main()
{
    Line vec1(1);
    vec1.x(0) = 0;
    vec1.y(0) = 1;

    Line vec2(1);
    vec2.x(0) = 1;
    vec2.y(0) = 0;
    
    cout << getAngle(vec1, vec2) << endl;
}