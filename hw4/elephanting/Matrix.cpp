#include <iostream>
#include <random>
#include <stdexcept>
#include <cmath>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include "mkl.h"
#include "Matrix.h"

namespace py = pybind11;

/* generate matrix entries using uniform distribution between -1 and 1 */
Matrix::Matrix(size_t nrow, size_t ncol)
{
    resetbuf(nrow, ncol);
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_real_distribution<double> unif(-1, 1);
    
    for (size_t i = 0; i < nrow * ncol; ++i)
    {
        m_buffer[i] = unif(generator);
    }
}

std::string Matrix::repr() const
{
    std::string ret = "";
    for (size_t row = 0; row < m_nrow; row++)
    {
        for (size_t col = 0; col < m_ncol; col++)
        {
            std::string tmp = "";
            tmp += std::to_string(m_buffer[row * m_ncol + col]);
            tmp = tmp.substr(0, 2);
            ret += tmp;
            ret += " ";
        }
        ret += "\n";
    }
    return ret;
}

Matrix::~Matrix()
{
    resetbuf(0, 0);
}

Matrix &Matrix::operator=(Matrix const &other)
{
    if (this == &other)
    {
        return *this;
    }
    if (this->m_nrow != other.m_nrow || this->m_ncol != other.m_ncol)
    {
        resetbuf(other.m_nrow, other.m_ncol);
    }
    for (size_t i = 0; i < m_nrow * m_ncol; i++)
    {
        this->m_buffer[i] = other.m_buffer[i];
    }
    return *this;
}

bool Matrix::operator==(Matrix const &other)
{
    bool ret = true;
    if (this == &other)
    {
        ret = true;
    }
    if (this->m_nrow != other.m_nrow || this->m_ncol != other.m_ncol)
    {
        ret = false;
    }
    for (size_t idx = 0; idx < this->m_nrow * this->m_ncol; idx++)
    {
        if (abs(this->m_buffer[idx] - other.m_buffer[idx]) > 0.01)
        {
            ret = false;
            break;
        }        
    }
    return ret;
}

size_t Matrix::nrow() const
{
    return m_nrow;
}

size_t Matrix::ncol() const
{
    return m_ncol;
}

double* Matrix::buffer() const
{
    return m_buffer;
}

Matrix::Matrix(const Matrix &other)
{
    resetbuf(other.m_nrow, other.m_ncol);
    for (size_t i = 0; i < m_nrow * m_ncol; i++)
    {
        this->m_buffer[i] = other.m_buffer[i];
    }
}

double &Matrix::operator()(size_t row, size_t col)
{
    if (row >= this->m_nrow || col >= this->m_ncol)
    {
        throw std::out_of_range("index out of range");
    }
    return m_buffer[row * m_ncol + col];
}

double Matrix::operator()(size_t row, size_t col) const
{
    if (row >= this->m_nrow || col >= this->m_ncol)
    {
        throw std::out_of_range("index out of range");
    }
    return m_buffer[row * m_ncol + col];
}

void Matrix::resetbuf(size_t row, size_t col)
{
    if (m_buffer)
    {
        delete[] m_buffer;
    }
    if (row == 0 || col == 0)
    {
        m_buffer = nullptr;
    }
    else
    {
        m_buffer = new double[row * col];
    }
    m_nrow = row;
    m_ncol = col;    
}

Matrix mkl_MM(Matrix const &A, Matrix const &B)
{
    if (A.ncol() != B.nrow())
    {
        throw std::out_of_range("invalid matrix size of matrix multiplication.");
    }
    double alpha = 1.0;
    double beta = 0.0;
    Matrix ret(A.nrow(), B.ncol());
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                A.nrow(), B.ncol(), A.ncol(), alpha,
                A.buffer(), A.ncol(), B.buffer(), B.ncol(), beta, ret.buffer(), ret.ncol());
    return ret;
}

bool isequal(Matrix& A, Matrix& B)
{
    if (A == B)
    {
        return true;
    }
    else
    {
        return false;
    }
}

Matrix operator*(Matrix const &A1, Matrix const &A2)
{
    if (A1.ncol() != A2.nrow())
    {
        throw std::out_of_range("invalid matrix size of matrix multiplication.");
    }
    Matrix ret(A1.nrow(), A2.ncol());
    for (size_t row = 0; row < ret.nrow(); row++)
    {
        for (size_t col = 0; col < ret.ncol(); col++)
        {
	        double tmp = 0;
            for (size_t k = 0; k < A1.ncol(); k++)
            {
                tmp += (A1(row, k) * A2(k, col));
            }
            if (tmp == -0.0)
            {
                tmp = 0.0;
            }
	        ret(row, col) = tmp;
        }
    }
    return ret;
}


Matrix multiply_naive(Matrix const &A1, Matrix const &A2)
{
    return A1 * A2;
}

Matrix multiply_tile(Matrix const &A1, Matrix const &A2, size_t size)
{
    
    if (A1.ncol() != A2.nrow())
    {
        throw std::out_of_range("invalid matrix size of matrix multiplication.");
    }
    Matrix res(A1.nrow(), A2.ncol())
}

Matrix operator*(double const &c, Matrix const &A)
{
    Matrix ret(A.nrow(), A.ncol());
    for (size_t row = 0; row < ret.nrow(); row++)
    {
        for (size_t col = 0; col < ret.ncol(); col++)
        {
            ret(row, col) = c * ret(row, col);
            if (ret(row, col) == -0.0)
            {
                ret(row, col) == 0.0;
            }
        }
    }
    return ret;
}

Matrix operator*(Matrix const &A, double const &c)
{
    Matrix ret(A.nrow(), A.ncol());
    for (size_t row = 0; row < ret.nrow(); row++)
    {
        for (size_t col = 0; col < ret.ncol(); col++)
        {
            ret(row, col) = c * ret(row, col);
            if (ret(row, col) == -0.0)
            {
                ret(row, col) == 0.0;
            }
        }
    }
    return ret;
}

PYBIND11_MODULE(_matrix, m) {
    m.doc() = "matrix class implements matrix multiplication";
    m.def("multiply_mkl", &mkl_MM, "mkl matrix multiplication");
    m.def("multiply_naive", &multiply_naive);
    m.def("isequal", &isequal, "compare two matrics");
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def(py::self * py::self)
        .def("__eq__", [](Matrix &A, Matrix const &B){
            return A == B;
        })
        .def("__getitem__", [](Matrix const &A, std::pair<size_t, size_t> const idx){
            return A(idx.first, idx.second);
        })
        //.def("__repr__", &Matrix::repr)
        .def("__setitem__", [](Matrix &A, std::pair<size_t, size_t> const idx, double const &v){
            A(idx.first, idx.second) = v;
        })
        .def(py::self * double())
        .def(double() * py::self);
}