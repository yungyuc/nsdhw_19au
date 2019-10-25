

#include <pybind11/pybind11.h>
#include <tuple>
#include <math.h>
#include <iostream>

using namespace std;
namespace py = pybind11;

double angle(std::tuple<double, double> const & vec1, 
	std::tuple<double, double> const & vec2
)
{
	if((get<0>(vec1)==0.0&&get<1>(vec1)==0.0)||(get<0>(vec2)==0.0&&get<1>(vec2)==0.0))		
		return 500;
	double theta = atan2(get<0>(vec1),get<1>(vec1)) 
					- atan2(get<0>(vec2),get<1>(vec2));
	if (theta > M_PI)
		theta -= 2 * M_PI;
	if (theta < -M_PI)
		theta += 2 * M_PI;
 
	theta = theta * 180.0 / M_PI;
	return theta;
}

void talk()
{
	cout<<"Hello all my friends!!!"<<endl;
	return;
}

PYBIND11_MODULE(pybind11_angle, mod)
{
    mod.doc() = "Example C extension module";
    mod.def("angle", &angle, "compute two vectors' angle");
    mod.def("talk", &talk, "A function which talks");
}
