#include "matrix.h"
#include <pybind11/pybind11.h>
#include <sstream>

namespace py = pybind11;

PYBIND11_MODULE(_matrix, m) {
  m.doc() = "simple matrix implementation";
  m.def("multiply_naive", &multiply_naive);
  m.def("multiply_mkl", &multiply_mkl);
  py::class_<Matrix>(m, "Matrix")
      .def(py::init<size_t, size_t>())
      .def_property("nrow", &Matrix::nrow, nullptr)
      .def_property("ncol", &Matrix::ncol, nullptr)
      .def("__repr__",
           [](const Matrix &m) {
             std::stringstream ss;
             ss << m;
             return "<_matrix.Matrix size '" + std::to_string(m.nrow()) + 'x' +
                    std::to_string(m.ncol()) + "'>\n" + ss.str();
           })
      .def("__eq__", [](const Matrix &m, const Matrix &n) { return m == n; })
      .def("__getitem__",
           [](const Matrix &m, std::pair<size_t, size_t> idx) {
             return m(idx.first, idx.second);
           })
      .def("__setitem__",
           [](Matrix &m, std::pair<size_t, size_t> idx, double v) {
             return m(idx.first, idx.second) = v;
           });
}
