#include <pybind11/pybind11.h>

#include <complex>
#include <tuple>
#include <vector>

double calc_angle(std::tuple<float, float> const &vector_1,
                  std::tuple<float, float> const &vector_2) {

  const float vector_1_angle =
      atan2(std::get<1>(vector_1), std::get<0>(vector_1));
  const float vector_2_angle =
      atan2(std::get<1>(vector_2), std::get<0>(vector_2));

  return vector_2_angle - vector_1_angle;
}

PYBIND11_MODULE(calc_angle, mod) {

  mod.doc() = "calculate angle between two 2-dimensional vectors";

  mod.def("calc_angle", &calc_angle, "calculate angle");
}