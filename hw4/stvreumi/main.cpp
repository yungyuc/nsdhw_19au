#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "matrix.h"
#include "tiling.h"

#include <mkl.h>

namespace py = pybind11;


/*
 * Throw an exception if the shapes of the two matrices don't support
 * multiplication.
 */
void validate_multiplication(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }
}

/*
 * Naive matrix matrix multiplication.
 */
Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2)
{
    validate_multiplication(mat1, mat2);

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
    validate_multiplication(mat1, mat2);

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
        mat1.ncol(), // lda, first dimention of A
        mat2.data(), // matrix B
        mat2.ncol(), // lda, first dimention of B
        0.0, // beta
        ret.data(), // matrix C
        ret.ncol() // lad, the first dimention of C
    );

    return ret;
}

/*
 * Tiled matrix matrix multiplication.
 */
Matrix multiply_tile(
    Matrix const & mat1, Matrix const & mat2, size_t const tsize)
{
    validate_multiplication(mat1, mat2);

    Matrix ret(mat1.nrow(), mat2.ncol());

    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();

    const size_t ntrow1 = (nrow1 / tsize) + 1;
    const size_t ntcol1 = (ncol1 / tsize) + 1;
    const size_t ntrow2 = (nrow2 / tsize) + 1;
    const size_t ntcol2 = (ncol2 / tsize) + 1;

    // after padding size
    const size_t nrow1_padding = ntrow1 * tsize;
    const size_t ncol1_padding = ntcol1 * tsize;
    const size_t nrow2_padding = ntrow2 * tsize;
    const size_t ncol2_padding = ntcol2 * tsize;

    // record padding idx
    Matrix padding_mat1(nrow1_padding, ncol1_padding);
    Matrix padding_mat2(nrow2_padding, ncol2_padding);
    Matrix padding_ret(nrow1_padding, ncol2_padding);

    for (size_t it=0; it<nrow1; ++it)
    {
        for (size_t kt=0; kt<ncol1; ++kt)
        {
            padding_mat1(it,kt) = mat1(it,kt);
        }
    }

    for (size_t it=0; it<nrow2; ++it)
    {
        for (size_t kt=0; kt<ncol2; ++kt)
        {
            padding_mat2(it,kt) = mat2(it,kt);
        }
    }

    Block value(tsize);
    Tiler tiler(tsize);

    for (size_t it=0; it<ntrow1; ++it)
    {
        for (size_t kt=0; kt<ntcol2; ++kt)
        {
            value = 0;
            for (size_t jt=0; jt<ntcol1; ++jt)
            {
                tiler.load(padding_mat1, it, jt, padding_mat2, jt, kt);
                tiler.multiply();
                value += *tiler.m_ret;
            }
            value.save(padding_ret, it, kt);
        }
    }

    for (size_t it=0; it<nrow1; ++it)
    {
        for (size_t kt=0; kt<ncol2; ++kt)
        {
            ret(it,kt) = padding_ret(it,kt);
        }
    }
    return ret;
}

PYBIND11_MODULE(_matrix, mod)
{

    mod.doc() = 
        "matrix class and multiplication operation";

    // export class
    py::class_<Matrix>(mod, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<Matrix>())
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
    mod.def("multiply_tile", 
        &multiply_tile, "tiled method of multiplication");

}