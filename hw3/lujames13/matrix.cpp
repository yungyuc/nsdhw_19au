#include <iostream>
#include <iomanip>
#include <vector>
#include <stdexcept>

#include <mkl.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
namespace py = pybind11;

class Matrix {

public:

    Matrix(size_t nrow, size_t ncol)
            : m_nrow(nrow), m_ncol(ncol)
    {
        reset_buffer(nrow, ncol);
    }

    Matrix(size_t nrow, size_t ncol, std::vector<double> const &vec)
            : m_nrow(nrow), m_ncol(ncol) {
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
    bool operator==(Matrix &right) {
        if (m_nrow != right.nrow() && m_ncol != right.ncol()) {
            return false;
        }
        const double *right_buf = right.data();
        size_t len = m_nrow * m_ncol;
        for (size_t i = 0; i < len; ++i) {
            if (m_buffer[i] != right_buf[i])
                return false;
        }
        return true;
    }
    double   operator() (size_t row, size_t col) const { return m_buffer[index(row, col)]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[index(row, col)]; }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    size_t size() const { return m_nrow * m_ncol; }
    double buffer(size_t i) const { return m_buffer[i]; }
    std::vector<double> buffer_vector() const { return std::vector<double>(m_buffer, m_buffer+size()); }

    double * data() const { return m_buffer; }

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
            double v = 0.0;
            for (size_t j=0; j<mat1.ncol(); ++j)
            {
                v += mat1(i,j) * mat2(j,k);
            }
            ret(i,k) = v;
	}
	//printf("[%zu,%zu]: %f", i, 13, mat1(i,13));

    }

    return ret;
}

/*
Matrix multiply_naive(Matrix const & mat1, Matrix const & mat2)
{
    return mat1 * mat2; // use existing function
}
*/
Matrix multiply_mkl(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.ncol() != mat2.nrow())
    {
        throw std::out_of_range(
                "the number of first matrix column "
                "differs from that of second matrix row");
    }

    Matrix ret(mat1.nrow(), mat2.ncol());

    size_t m = mat1.nrow();
    size_t n = mat2.ncol();
    size_t k = mat1.ncol();
    double alpha = 1.0;
    double beta = 0;
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                    m, n, k, alpha, mat1.data(), k, mat2.data(), n, beta, ret.data(), n);

    return ret;
}

bool assertEqual(Matrix const & mat1, Matrix const & mat2)
{
    if (mat1.nrow() != mat2.nrow())
        return false;
    if (mat1.ncol() != mat2.ncol())
        return false;
    for (size_t i=0; i<mat1.nrow();++i)
    {
        for (size_t j=0; j<mat1.ncol();++j)
        {
            if (mat1(i,j) != mat2(i,j))
                return false;
        }
    }
    return true;
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

    ostr << std::endl << " data: ";
    for (size_t i=0; i<mat.size(); ++i)
    {
        ostr << " " << std::setw(2) << mat.data()[i];
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
/*
int main(int argc, char ** argv)
{
    const size_t n = 3;
    int status;

    std::cout << ">>> Solve Ax=b (row major)" << std::endl;
    Matrix mat(n, n);
    mat(0,0) = 3; mat(0,1) = 5; mat(0,2) = 2;
    mat(1,0) = 2; mat(1,1) = 1; mat(1,2) = 3;
    mat(2,0) = 4; mat(2,1) = 3; mat(2,2) = 2;
    Matrix b(n, 2);
    b(0,0) = 57; b(0,1) = 23;
    b(1,0) = 22; b(1,1) = 12;
    b(2,0) = 41; b(2,1) = 84;
    std::vector<int> ipiv(n);

    std::cout << "A:" << mat << std::endl;
    std::cout << "b:" << b << std::endl;

    Matrix result1(mat.nrow(), b.ncol());
    Matrix result2(mat.nrow(), b.ncol());
    result1 = multiply_naive(mat, b);
    std::cout << "result1:" << result1 << std::endl;
    result2 = multiply_mkl(mat, b);
    std::cout << "result2:" << result2 << std::endl;

    return 0;
}
*/
PYBIND11_MODULE(_matrix, m){
    m.doc() = "pybin11 plugin to calculate the matrix matrix multiply";
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__getitem__", [](Matrix &self, std::pair<size_t, size_t> index)
            { return self(index.first, index.second); })
        .def("__setitem__", [](Matrix &self, std::pair<size_t, size_t> index, double value)
                { self(index.first, index.second) = value;})
        .def("__eq__", [](Matrix & mat1, Matrix & mat2){ return assertEqual(mat1, mat2); })
        .def("__ne__", [](Matrix & mat1, Matrix & mat2){ return !assertEqual(mat1, mat2); });
//	.def(py::self == py::self)
//	.def(py::self != py::self);
    m.def("multiply_naive", &multiply_naive, "Naive matrix-matrix multiply");
    m.def("multiply_mkl", &multiply_mkl, "mkl matrix-matrix multiply");
}
