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

Matrix multiply_tile(Matrix &lhs, Matrix &rhs, size_t tsize) {
  if (lhs.ncol() != rhs.nrow()) {
    throw std::invalid_argument("dimension mismatch");
  }
  Matrix ret(lhs.nrow(), rhs.ncol());
  size_t s_i = (lhs.nrow() + tsize - 1) / tsize;
  size_t s_j = (rhs.ncol() + tsize - 1) / tsize;
  size_t s_k = (rhs.ncol() + tsize - 1) / tsize;
  for (size_t t_k = 0; t_k < s_k; ++t_k) {
    for (size_t t_i = 0; t_i < s_i; ++t_i) {
      for (size_t t_j = 0; t_j < s_j; ++t_j) {
        size_t r_i = std::min((t_i + 1) * tsize, lhs.nrow());
        size_t r_j = std::min((t_j + 1) * tsize, rhs.ncol());
        size_t r_k = std::min((t_k + 1) * tsize, lhs.ncol());
        for (size_t i = t_i * tsize; i < r_i; ++i) {
          for (size_t j = t_j * tsize; j < r_j; ++j) {
            double v = 0;
            for (size_t k = t_k * tsize; k < r_k; ++k) {
              v += lhs(i, k) * rhs(k, j);
            }
            ret(i, j) += v;
          }
        }
      }
    }
  }
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
