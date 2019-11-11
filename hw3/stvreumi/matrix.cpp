#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <iostream>
#include <iomanip>
#include <stdexcept>

#ifdef NOMKL
#include <cblas.h>
#else // NOMKL
#include <mkl_cblas.h>
#endif // NOMKL

namespace py = pybind11;

// copy from 07_matrix_matrix.cpp
class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    bool operator==(Matrix const & mat) const
    {
        if (size() != mat.size())
        {
            return false;
        }

        for (size_t i=0; i<m_nrow; ++i)
        {
            for (size_t j=0; j<m_ncol; ++j)
            {
                if((*this)(i,j) != mat(i,j))
                {
                    return false;
                }
            }
        }

        return true;
    }

    bool operator!=(Matrix const & mat) const { return !operator==(mat); }

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
        std::swap(m_nrow, other.m_nrow);
        std::swap(m_ncol, other.m_ncol);
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
    double * data() const { return m_buffer; }
private:

    size_t index(size_t row, size_t col) const
    {
        return row + col * m_nrow;
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
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    // reference:
    // https://stackoverflow.com/questions/23324480/matrix-operations-in-c-using-blas-lapack-or-some-other-alternative
    // https://developer.apple.com/documentation/accelerate/1513282-cblas_dgemm?language=objc
    // http://sep.stanford.edu/sep/claudio/Research/Prst_ExpRefl/ShtPSPI/intel/mkl/10.0.3.020/include/mkl_cblas.h
    // http://www.netlib.org/lapack/explore-html/dc/d18/cblas__dgemm_8c.html
    cblas_dgemm( // C = (alpha)AB + (beta)C
        CblasRowMajor, CblasNoTrans, CblasNoTrans,
        mat1.nrow(), //M, number of rows in A and C
        mat2.ncol(), //N, number of columns in B and C
        mat1.ncol(), //K, number fo columns in A; number of rows in B
        1.0, // alpha
        mat1.data(), // matrix A 
        mat1.nrow(), // lda, first dimention of A
        mat2.data(), // matrix B
        mat2.nrow(), // lda, first dimention of B
        1.0, // beta
        ret.data(), // matrix C
        ret.nrow() // lad, the first dimention of C
    );

    return ret;
}

PYBIND11_MODULE(_matrix, mod)
{

    mod.doc() = 
        "matrix class and multiplication operation";

    // export class
    py::class_<Matrix>(mod, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<Matrix &&>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        // ref: https://github.com/pybind/pybind11/blob/master/tests/test_sequences_and_iterators.cpp#L182
        .def("__getitem__", [](const Matrix &m, 
            std::tuple<size_t, size_t> const & idx)
        {
            size_t row = std::get<0>(idx);
            size_t col = std::get<1>(idx);
            if (row >= m.nrow()) throw py::index_error();
            if (col >= m.ncol()) throw py::index_error();
            return m(row, col);
        })
        .def("__setitem__", [](Matrix &m, 
            std::tuple<size_t, size_t> const & idx, double v) 
        {
            size_t row = std::get<0>(idx);
            size_t col = std::get<1>(idx);
            if (row >= m.nrow()) throw py::index_error();
            if (col >= m.ncol()) throw py::index_error();
            m(row, col) = v;
        })
        .def(py::self == py::self)
        .def(py::self != py::self)
        ;
    
    // export free function
    mod.def("multiply_naive", 
        &multiply_naive, "native method of multiplication");
    mod.def("multiply_mkl", 
        &multiply_mkl, "mkl-BLAS method of multiplication");

}
