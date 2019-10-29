#include <pybind11/pybind11.h>
#include <vector>
#include <stddef.h>

#include "mkl.h"

namespace py = pybind11;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    ~Matrix()
    {
        reset_buffer(0, 0);
    }

    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }
    double * buffer() const { return m_buffer; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer+size()); }

private:

    size_t index(size_t row, size_t col) const
    {
        return row + col * m_nrow;
    }

    void reset_buffer(size_t nrow, size_t ncol)
    {
        if (m_buffer) { mkl_free(m_buffer); }
        const size_t nelement = nrow * ncol;
        if (nelement) { m_buffer = (double *)mkl_malloc( nrow*ncol*sizeof( double ), 64 ); }
        else          { m_buffer = nullptr; }
        m_nrow = nrow;
        m_ncol = ncol;
    }

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    double * m_buffer = nullptr;

};

/*
 * Naive matrix matrix multiplication.
 */
Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    for (size_t i=0; i<ret.nrow(); ++i)
    {
        for (size_t k=0; k<ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
        }
    }

    return ret;
}

Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{
    float alpha = 1.0; 
    float beta = 0.0;
    Matrix ret(mat1.nrow(), mat2.ncol());
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, 
                mat1.nrow(), mat2.ncol(), mat1.ncol(), alpha, 
                mat1.buffer(), mat1.ncol(), mat2.buffer(), mat2.ncol(), 
                beta, ret.buffer(), mat2.ncol());

    return ret;
}

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "pybind11 matrix class";      // module doc string
    py::class_<Matrix>(m, "Matrix")
    .def(py::init<size_t, size_t>());
    m.def("multiply_naive",                              // function name
        &multiply_naive,                               // function pointer
        "A function which multiply two matrices in naive way." //function doc string
        );
    m.def("multiply_mkl",                              // function name
        &multiply_mkl,                               // function pointer
        "A function which multiply two matrices with mkl library." //function doc string
       );
}