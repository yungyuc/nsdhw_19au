#include "matrix.h"
#include <iomanip>
#include <iostream>
#include <mkl.h>
#include <stdexcept>

Matrix multiply_naive(Matrix &lhs, Matrix &rhs) {
  if (lhs.ncol() != rhs.nrow()) {
    throw std::invalid_argument("dimension mismatch");
  }
  Matrix ret(lhs.nrow(), rhs.ncol());
  for (size_t i = 0; i < lhs.nrow(); ++i) {
    for (size_t j = 0; j < rhs.ncol(); ++j) {
      double v = 0;
      for (size_t k = 0; k < lhs.ncol(); ++k) {
        v += lhs(i, k) * rhs(k, j);
      }
      ret(i, j) = v;
    }
  }
  return ret;
}
Matrix multiply_mkl(Matrix &lhs, Matrix &rhs) {
  if (lhs.ncol() != rhs.nrow()) {
    throw std::invalid_argument("dimension mismatch");
  }
  Matrix ret(lhs.nrow(), rhs.ncol());
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, lhs.nrow(), rhs.ncol(),
              lhs.ncol(), 1, lhs.data(), lhs.ncol(), rhs.data(), rhs.ncol(), 0,
              ret.data(), ret.ncol());
  return ret;
}

std::ostream &operator<<(std::ostream &os, Matrix const &m) {
  for (size_t i = 0; i < m.nrow(); ++i) {
    os << '\n';
    for (size_t j = 0; j < m.ncol(); ++j) {
      os << ' ' << std::setw(3) << m(i, j);
    }
  }
  return os;
}
