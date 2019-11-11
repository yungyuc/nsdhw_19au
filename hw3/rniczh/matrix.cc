#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <mkl.h>
// #include <omp.h>

#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include <cmath>
#define ceild(n, d) ceil(((double)(n)) / ((double)(d)))
#define floord(n, d) floor(((double)(n)) / ((double)(d)))
#define max(x, y) ((x) > (y) ? (x) : (y))
#define min(x, y) ((x) < (y) ? (x) : (y))

namespace py = pybind11;

class Matrix {

public:
  Matrix(size_t nrow, size_t ncol) : m_nrow(nrow), m_ncol(ncol) {
    reset_buffer(nrow, ncol);
  }

  Matrix(size_t nrow, size_t ncol, std::vector<double> const &vec)
      : m_nrow(nrow), m_ncol(ncol) {
    reset_buffer(nrow, ncol);
    (*this) = vec;
  }

  Matrix &operator=(std::vector<double> const &vec) {
    if (size() != vec.size()) {
      throw std::out_of_range("number of elements mismatch");
    }

    size_t k = 0;
    for (size_t i = 0; i < m_nrow; ++i) {
      for (size_t j = 0; j < m_ncol; ++j) {
        (*this)(i, j) = vec[k];
        ++k;
      }
    }

    return *this;
  }

  Matrix(Matrix const &other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol) {
    reset_buffer(other.m_nrow, other.m_ncol);
    for (size_t i = 0; i < m_nrow; ++i) {
      for (size_t j = 0; j < m_ncol; ++j) {
        (*this)(i, j) = other(i, j);
      }
    }
  }

  Matrix &operator=(Matrix const &other) {
    if (this == &other) {
      return *this;
    }
    if (m_nrow != other.m_nrow || m_ncol != other.m_ncol) {
      reset_buffer(other.m_nrow, other.m_ncol);
    }
    for (size_t i = 0; i < m_nrow; ++i) {
      for (size_t j = 0; j < m_ncol; ++j) {
        (*this)(i, j) = other(i, j);
      }
    }
    return *this;
  }


  Matrix(Matrix &&other) : m_nrow(other.m_nrow), m_ncol(other.m_ncol) {
    reset_buffer(0, 0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_ncol, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
  }

  Matrix &operator=(Matrix &&other) {
    if (this == &other) {
      return *this;
    }
    reset_buffer(0, 0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_ncol, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
    return *this;
  }

  bool operator==(Matrix &rhs) {
    if (m_nrow != rhs.nrow() && m_ncol != rhs.ncol()) {
      return false;
    }
    const double *rhs_buf = rhs.data();
    size_t len = m_nrow * m_ncol;
    for (size_t i = 0; i < len; ++i) {
      if (m_buffer[i] != rhs_buf[i])
        return false;
    }
    return true;
  }
  
  ~Matrix() { reset_buffer(0, 0); }

  double operator()(size_t row, size_t col) const {
    return m_buffer[index(row, col)];
  }
  double &operator()(size_t row, size_t col) {
    return m_buffer[index(row, col)];
  }

  size_t nrow() const { return m_nrow; }
  size_t ncol() const { return m_ncol; }

  size_t size() const { return m_nrow * m_ncol; }
  double buffer(size_t i) const { return m_buffer[i]; }
  std::vector<double> buffer_vector() const {
    return std::vector<double>(m_buffer, m_buffer + size());
  }

  double *data() { return m_buffer; }
  
  std::string show() {
    std::string ret;

    ret += "[";
    for (size_t i = 0; i < m_nrow; ++i) {
      ret += "[";
      for (size_t j = 0; j < m_ncol; ++j) {
        ret += std::to_string((*this)(i, j));
        ret += " ";
      }
      ret += "]\n";
    }
    ret += "]\n";
    return ret;
  }

private:
  size_t index(size_t row, size_t col) const { return row * m_ncol + col; }

  void reset_buffer(size_t nrow, size_t ncol) {
    if (m_buffer) {
      delete[] m_buffer;
    }
    const size_t nelement = nrow * ncol;
    if (nelement) {
      m_buffer = new double[nelement];
    } else {
      m_buffer = nullptr;
    }
    m_nrow = nrow;
    m_ncol = ncol;
  }

  size_t m_nrow = 0;
  size_t m_ncol = 0;
  double *m_buffer = nullptr;
};

std::ostream &operator<<(std::ostream &ostr, Matrix const &mat) {
  for (size_t i = 0; i < mat.nrow(); ++i) {
    ostr << std::endl << " ";
    for (size_t j = 0; j < mat.ncol(); ++j) {
      ostr << " " << std::setw(2) << mat(i, j);
    }
  }

  return ostr;
}

Matrix naive_mul(const Matrix &A, const Matrix &B) {
  if (A.ncol() != B.nrow()) {
    throw std::out_of_range("the number of first matrix column "
                            "differs from that of second matrix row");
  }

  size_t m = A.nrow(), n = B.ncol(), o = A.ncol();

  Matrix res(m, n);

  // ================================
  //  Original naive version
  // ================================
  for (size_t i = 0; i < m; ++i) {
    for (size_t j = 0; j < n; ++j) {
      double sum = 0.0;
      for (size_t k = 0; k < o; ++k) {
        sum += A(i, k) * B(k, j);
      }
      res(i,j) = sum;
    }
  }

  return res;
}

Matrix mkl_mul(Matrix &A, Matrix &B) {
  if (A.ncol() != B.nrow()) {
    throw std::out_of_range("the number of first matrix column "
                            "differs from that of second matrix row");
  }

  Matrix C(A.nrow(), B.ncol());

  // Assume that Matrix A is m * k and B is k * n matrix
  size_t m = C.nrow();
  size_t n = C.ncol();
  size_t k = A.ncol();

  double alpha = 1, beta = 0;
  // mkl implementation
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha,
              A.data(), k, B.data(), n, beta, C.data(), n);

  return C;
}

PYBIND11_MODULE(_matrix, m) {
  m.doc() = "pybind11 matrix plugin";
  py::class_<Matrix>(m, "Matrix")
      .def(py::init<size_t, size_t>())
      .def_property_readonly("nrow", &Matrix::nrow)
      .def_property_readonly("ncol", &Matrix::ncol)
      .def("__eq__", [](Matrix &a, Matrix &b) { return a == b; })
      .def("__getitem__",
           [](Matrix &m, std::pair<size_t, size_t> i) {
             return m(i.first, i.second);
           })
      .def("__setitem__", [](Matrix &m, std::pair<size_t, size_t> i,
                             double v) { m(i.first, i.second) = v; })
      .def("__repr__", &Matrix::show);

  m.def("multiply_naive", &naive_mul, "Naive multiply of two matrix");
  m.def("multiply_mkl", &mkl_mul, "MKL's cblas multiply of two matrix");
}
