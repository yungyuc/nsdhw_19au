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

///////////////////////////////////////////
//      Blocks and Tiles for Tiling      //
///////////////////////////////////////////

template<size_t N> void Block<N>::save(
    Matrix & mat, size_t it, size_t jt
)
{
    const size_t ncol = mat.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_s = i*NDIM;
        const size_t base_t = (it*NDIM + i) * ncol + jt*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            mat.m_buffer[base_t + j] = m_buffer[base_s + j];
        }
    }
}

template<size_t N> void Tiler<N>::load(
    Matrix const & mat1, size_t it1, size_t jt1
  , Matrix const & mat2, size_t it2, size_t jt2
)
{
    const size_t ncol1 = mat1.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_t = i*NDIM;
        const size_t base_s = (it1*NDIM + i) * ncol1 + jt1*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_mat1[base_t + j] = mat1.m_buffer[base_s + j];
        }
    }

    const size_t ncol2 = mat2.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_t = i*NDIM;
        const size_t base_s = (it2*NDIM + i) * ncol2 + jt2*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_ret[base_t + j] = mat2.m_buffer[base_s + j];
        }
    }

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base = i*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_mat2[j*NDIM + i] = m_ret[base + j];
        }
    }
}

template<size_t N> void Tiler<N>::multiply()
{
    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base1 = i*NDIM;

        for (size_t k=0; k<NDIM; ++k)
        {
            const size_t base2 = k*NDIM;

            double v = 0;
            for (size_t j=0; j<NDIM; ++j)
            {
                v += m_mat1[base1 + j] * m_mat2[base2 + j];
            }
            m_ret[base1 + k] = v;
        }
    }
}

Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, const unsigned int tsize)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
            "the number of first matrix column "
            "differs from that of second matrix row");
    }

    // return multiply_mkl(mat1, mat2);

    Matrix ret(mat1.nrow(), mat2.ncol());

    const int block_size = static_cast<int>(tsize);
    const int N = mat1.nrow();
    const int M = mat1.ncol();
    const int K = mat2.ncol();

    for(int i=0; i<N; i++) {
        for(int j=0; j<K; j++) {
            ret(i, j) = 0;
        }
    }

    for (int i0 = 0; i0 < N; i0 += block_size) {
        int imax = i0 + block_size > N ? N : i0 + block_size;

        for (int j0 = 0; j0 < M; j0 += block_size) {
            int jmax = j0 + block_size > M ? M : j0 + block_size;

            for (int k0 = 0; k0 < K; k0 += block_size) {
                int kmax = k0 + block_size > K ? K : k0 + block_size;

                for (int i1 = i0; i1 < imax; ++i1) {
                    for (int j1 = j0; j1 < jmax; ++j1) {
                        for (int k1 = k0; k1 < kmax; ++k1) {
                            // C[kij] += A[mi + k1] * B[sj + k1];
                            ret(i1, j1) += mat1(i1, k1) * mat2(k1, j1);
                        }
                    }
                }
            }
        }
    }
/*
    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    // const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();

    const size_t ntrow1 = nrow1 / tsize;
    const size_t ntcol1 = ncol1 / tsize;
    // const size_t ntrow2 = nrow2 / tsize;
    const size_t ntcol2 = ncol2 / tsize;

    Block<tsize> value;
    Tiler<tsize> tiler;

    for (size_t it=0; it<ntrow1; ++it)
    {
        for (size_t kt=0; kt<ntcol2; ++kt)
        {
            value = 0;
            for (size_t jt=0; jt<ntcol1; ++jt)
            {
                tiler.load(mat1, it, jt, mat2, jt, kt);
                tiler.multiply();
                value += tiler.m_ret;
            }
            value.save(ret, it, kt);
        }
    }
*/
    return ret;
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

    mod.def("multiply_tile", &multiply_tile, "Matrix Tiling implementation");

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