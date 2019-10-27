#include "Matrix.hpp"
#include <pybind11/pybind11.h>
namespace py = pybind11;

PYBIND11_MODULE(_matrix, m){
    m.doc() = "Matrix library";
    py::class_<Matrix> Matrix(m, "Matrix");

    Matrix.def(py::init<int, int>());

}