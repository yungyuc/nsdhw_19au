#include "Matrix.hpp"
#include <pybind11/pybind11.h>
namespace py = pybind11;

PYBIND11_MODULE(_matrix, m){
    m.doc() = "Matrix library";
    py::class_<Matrix> Matrix(m, "Matrix");

    Matrix.def(py::init<int, int>());
    Matrix.def("multiply_naive",&multiply_naive,
        "use naive method to multiply matrices.");
    // Matrix.def("multiply_mkl", &multiply_mkl,
    //     "use mkl to multiply matrices.");
}