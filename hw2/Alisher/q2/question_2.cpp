#include <pybind11/pybind11.h>
#include <cmath>

typedef std::pair<double, double> Vector;
double angle(Vector v1, Vector v2){
    double dot=v1.first*v2.first+v1.second*v2.second;
    double v1_mod=sqrt(v1.first*v1.first+v1.second*v1.second);
    double v2_mod=sqrt(v2.first*v2.first+v2.second*v2.second);
    return (double)acos(dot/(v1_mod*v2_mod));
}
PYBIND11_MODULE(question_2, mod){
    mod.doc() = "The angle (in radians)";
    mod.def("angle", &angle, "The angle (in radians) between two vectors");
}
