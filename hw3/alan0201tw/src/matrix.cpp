#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

#include "matrix.hpp"

#include <sstream>

#include <mkl.h>

namespace py = pybind11;

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

    for (size_t i = 0; i < ret.nrow(); ++i)
    {
        for (size_t k = 0; k < ret.ncol(); ++k)
        {
            double v = 0;
            for (size_t j = 0; j < mat1.ncol(); ++j)
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

    double* result = new double [mat1.nrow() * mat2.ncol()];

    cblas_dgemm(
        CblasRowMajor, CblasNoTrans, CblasNoTrans,
        mat1.nrow(), mat2.ncol(), mat1.ncol(),
        1.0, // alpha
        mat1.m_buffer, mat1.ncol(), mat2.m_buffer, mat2.ncol(),
        0.0, // beta
        result, mat2.ncol()
    );

    Matrix ret(mat1.nrow(), mat2.ncol(), result);

    return ret;
}

std::string Matrix::ToString()
{
    std::stringstream result("");
    for (size_t i=0; i<nrow(); ++i)
    {
        result << std::endl << " ";
        for (size_t j=0; j<ncol(); ++j)
        {
            result << " " << std::setw(2) << this->operator()(i, j);
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

/**
 *  Reference :
 *  
 *  Basics : https://pybind11.readthedocs.io/en/stable/classes.html
 *  Operators : https://pybind11.readthedocs.io/en/stable/advanced/classes.html#operator-overloading
 *  Indexing operator : 
 *  https://pybind11.readthedocs.io/en/stable/reference.html#_CPPv4NK10object_apiixE6handle
 *  https://github.com/pybind/pybind11/blob/master/tests/test_sequences_and_iterators.cpp#L182
 */

PYBIND11_MODULE(_matrix, mod)
{
    mod.doc() = "Matrix class implementing naive multiplication and mkl delegation";
    mod.def("multiply_naive", &multiply_naive, "Naive implementation");
    mod.def("multiply_mkl", &multiply_mkl, "MKL implementation delegate");

    py::class_<Matrix>(mod, "Matrix")
        .def( py::init<size_t, size_t>() )
        .def( py::init<size_t, size_t, std::vector<double>>() )
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__str__", &Matrix::ToString )
        .def("__repr__", &Matrix::ToString )
        .def(py::self == py::self)
        .def(py::self != py::self)
        .def("__getitem__",
            [](const Matrix &matrix, std::pair<size_t, size_t> index)
            {
                if (index.first >= matrix.nrow() || index.second >= matrix.ncol()) throw py::index_error();
                return matrix(index.first, index.second);
            }, py::is_operator())
        .def("__setitem__",
            [](Matrix &matrix, std::pair<size_t, size_t> index, double value)
            {
                if (index.first >= matrix.nrow() || index.second >= matrix.ncol()) throw py::index_error();
                matrix(index.first, index.second) = value;
            }, py::is_operator());
}