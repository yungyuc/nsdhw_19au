#include "matrix.hpp"

#include <sstream>

#include <lapacke.h>
#include <cblas.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

/*
 * Naive matrix matrix multiplication.
 */
Matrix operator*(Matrix const & mat1, Matrix const & mat2)
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

Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2)
{
    // redirect to operator *
    return mat1 * mat2;
}

// This example computes real matrix C=alpha*A*B+beta*C
//
// Reference : MKL Get started
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    double* result = (double *)aligned_alloc(64 , 
        mat1.nrow() * mat2.ncol() * sizeof(double));

    cblas_dgemm(
        CblasRowMajor, CblasNoTrans, CblasNoTrans,
        mat1.nrow(), mat2.ncol(), mat1.ncol(),
        1.0, // alpha
        mat1.m_buffer, mat1.ncol(), mat2.m_buffer, mat2.ncol(),
        0.0, // beta
        result, mat2.ncol()
    );

    // Reference : https://www.techiedelight.com/convert-array-vector-cpp/
    std::vector<double> result_vec(result, result + mat1.nrow() * mat2.ncol());

    return Matrix(mat1.nrow(), mat2.ncol(), result_vec);
}

std::string Matrix::ToString()
{
    std::stringstream result("");
    for (size_t i=0; i<mat.nrow(); ++i)
    {
        result << std::endl << " ";
        for (size_t j=0; j<mat.ncol(); ++j)
        {
            result << " " << std::setw(2) << mat(i, j);
        }
    }

    return result.str();
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

PYBIND11_MODULE(_matrix, mod)
{
    mod.doc() = "Matrix class implementing naive multiplication and mkl delegation"
    mod.def("multiply_naive", &multiply_naive, "Naive implementation");
    mod.def("multiply_mkl", &multiply_mkl, "MKL implementation delegate");

    pybind11::class_<Matrix>( mod, "Matrix" )
        .def( py::init<size_t, size_t>() )
        .def( py::init<size_t, size_t, std::vector<double>>() )
        .def("__repr__", &Matrix::ToString );

}