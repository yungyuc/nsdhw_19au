#include <iostream>
#include <sstream>
#include <cstdio>
#include <iomanip>
#include <vector>
#include <stdexcept>

#include <cassert>

#ifdef NOMKL
#include <cblas.h>
#else // NOMKL
#include <mkl.h>
#endif // NOMKL

class Matrix {

  public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
      reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const & vec)
      : m_nrow(nrow), m_ncol(ncol)
    {
      reset_buffer(nrow, ncol);
      (*this) = vec;
    }

    Matrix & operator=(std::vector<double> const & vec)
    {
      if (size() != vec.size())
      {
        throw std::out_of_range("number of elements mismatch");
      }

      size_t k = 0;
      for (size_t i=0; i<m_nrow; ++i)
      {
        for (size_t j=0; j<m_ncol; ++j)
        {
          (*this)(i,j) = vec[k];
          ++k;
        }
      }

      return *this;
    }

    Matrix(Matrix const & other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
      reset_buffer(other.m_nrow, other.m_ncol);
      for (size_t i=0; i<m_nrow; ++i)
      {
        for (size_t j=0; j<m_ncol; ++j)
        {
          (*this)(i,j) = other(i,j);
        }
      }
    }

    Matrix & operator=(Matrix const & other)
    {
      if (this == &other) { return *this; }
      if (m_nrow != other.m_nrow || m_ncol != other.m_ncol)
      {
        reset_buffer(other.m_nrow, other.m_ncol);
      }
      for (size_t i=0; i<m_nrow; ++i)
      {
        for (size_t j=0; j<m_ncol; ++j)
        {
          (*this)(i,j) = other(i,j);
        }
      }
      return *this;
    }

    Matrix(Matrix && other)
      : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
    {
      reset_buffer(0, 0);
      std::swap(m_buffer, other.m_buffer);
    }

    Matrix & operator=(Matrix && other)
    {
      if (this == &other) { return *this; }
      reset_buffer(0, 0);
      std::swap(m_nrow, other.m_nrow);
      std::swap(m_ncol, other.m_ncol);
      std::swap(m_buffer, other.m_buffer);
      return *this;
    }

    bool operator==(const Matrix &o) const
    {
      if(m_nrow != o.m_nrow || m_ncol != o.m_ncol)
        return false;
      for(size_t i = 0; i < m_nrow; i++)
        for(size_t j = 0; j < m_nrow; j++)
          if(o(i, j) != (*this)(i, j)) return false;
      return true;
    }

    bool operator!=(const Matrix &other) const
    {
      return !(*this == other);
    }

    ~Matrix()
    {
      reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer+size()); }

    double * data() const { return m_buffer; }

    std::string show() const;

  private:

    size_t index(size_t row, size_t col) const
    {
      return row * m_ncol + col;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
      if (m_buffer) { delete[] m_buffer; }
      const size_t nelement = nrow * ncol;
      if (nelement) { m_buffer = new double[nelement]; }
      else          { m_buffer = nullptr; }
      m_nrow = nrow;
      m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;

};

Matrix &operator*(Matrix const & mat1, Matrix const & mat2)
{
  if (mat1.ncol() != mat2.nrow())
  {
    throw std::out_of_range(
        "the number of first matrix column "
        "differs from that of second matrix row");
  }

  Matrix *ret = new Matrix(mat1.nrow(), mat2.ncol());

  for (size_t i=0; i<ret->nrow(); ++i)
  {
    for (size_t k=0; k<ret->ncol(); ++k)
    {
      double v = 0.0;
      for (size_t j=0; j<mat1.ncol(); ++j)
      {
        v += mat1(i,j) * mat2(j,k);
      }
      (*ret)(i,k) = v;
    }
  }

  return *ret;
}

std::ostream & operator << (std::ostream & ostr, Matrix const & mat)
{
  for (size_t i=0; i<mat.nrow(); ++i)
  {
    ostr << std::endl << " ";
    for (size_t j=0; j<mat.ncol(); ++j)
    {
      ostr << " " << std::setw(2) << mat(i, j);
    }
  }
  return ostr;
}

std::ostream & operator << (std::ostream & ostr, std::vector<double> const & vec)
{
  for (size_t i=0; i<vec.size(); ++i)
  {
    std::cout << " " << vec[i];
  }

  return ostr;
}

Matrix &multiply_naive(Matrix &m, Matrix &n)
{
  return m * n;
}

Matrix &multiply_mkl(const Matrix &m, const Matrix &n)
{
  double alpha = 1;
  double beta = 0;

  Matrix *o = new Matrix(m.nrow(), n.ncol());
  assert(m.ncol() == n.nrow());
  cblas_dgemm(CblasRowMajor,
      CblasNoTrans, CblasNoTrans,
      o->nrow(), o->ncol(), m.ncol(),
      alpha,
      m.data(), m.ncol(),
      n.data(), n.ncol(),
      beta,
      o->data(), o->ncol());

  return *o;
}

std::string dtswp(double num, int p){
  std::ostringstream streamObj3;
  streamObj3 << std::fixed << std::setprecision(p) << num;
  return streamObj3.str();
}

#define EXL 5
#define MXL (EXL * 2 + 1)
#define OMS " ... "

std::string Matrix::show() const
{
  std::string repr = "[";
  for (size_t i=0; i<m_nrow; ++i)
  {
    if(i) repr += " ";
    repr += "[";
    if(m_nrow > MXL && i == EXL){
      for(int k = 0; k < MXL; k++)
        repr += (k ? std::string(" ") : std::string("")) + OMS;
      repr += "\n";
      i = m_nrow - EXL - 1;
      continue;
    }
    for (size_t j = 0; j < m_ncol; ++j)
    {
      if(m_ncol > MXL && j == EXL){
        repr += std::string(" ") + OMS;
        j = m_ncol - EXL - 1;
        continue;
      }
      if(j) repr += " ";
      repr += dtswp((*this)(i,j), 3);
    }
    repr += "]";
    if(i != m_nrow - 1) repr += "\n";
  }
  repr += "] (" + std::to_string(m_nrow) + ", " + std::to_string(m_ncol) + ")";
  return repr;
}
