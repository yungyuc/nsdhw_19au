#include <iostream>
#include <random>
#include <stdexcept>
#include <cmath>
#include <ctime>
//#include <pybind11/pybind11.h>
//#include <pybind11/operators.h>
//#include "mkl.h"
#include "Matrix.h"

//namespace py = pybind11;

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
            tmp = tmp.substr(0, 4);
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

/*
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
*/

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

struct Block
{
    size_t size;
    double* ptr;
    Block(size_t _size)
    {
        size = _size;
        ptr = new double[size * size];
    }
};

void block_assign(Matrix const &A, size_t row, size_t col, Block &B)
{
    size_t size = B.size;
    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0; j < size; ++j)
        {
            B.ptr[i * size + j] = A(row + i, col + j);
        }
    }
}

void matrix_assign(Matrix &A, size_t row, size_t col, Block const &B)
{
    size_t size = B.size;
    for (size_t i = 0; i < size; ++i)
    {
        for (size_t j = 0; j < size; ++j)
        {
            A(row + i, col + j) = B.ptr[i * size + j];
        }
    }
}

Matrix multiply_tile(Matrix const &A1, Matrix const &A2, size_t size)
{
    
    if (A1.ncol() != A2.nrow())
    {
        throw std::out_of_range("invalid matrix size of matrix multiplication.");
    }   
    Matrix res(A1.nrow(), A2.ncol());
    size_t block_nrow = 0;
    size_t block_ncol = 0;
    size_t block_ncol2 = 0;
    bool flag1 = false;
    bool flag2 = false;
    bool flag3 = false;

    if (A1.nrow() % size != 0)
    {
        flag1 = true;
        block_nrow += 1;
    }
    if (A1.ncol() % size != 0)
    {
        flag2 = true;
        block_ncol += 1;
    }
    if (A2.ncol() % size != 0)
    {
        flag3 = true;
        block_ncol2 += 1;
    }

    block_ncol += (A1.ncol() / size);
    block_nrow += (A1.nrow() / size);
    block_ncol2 += (A2.ncol() / size);

    for (size_t i = 0; i < block_nrow; ++i)
    {
        for (size_t k = 0; k < block_ncol2; ++k)
        {
            Block bres(size);
            for (size_t j = 0; j < block_ncol; ++j)
            {
                Block left(size);
                Block right(size);
                block_assign(A1, i * size, j * size, left);
                block_assign(A2, j * size, k * size, right);
                for (size_t z = 0; z < size * size; ++z)
                {
                    bres.ptr[z] += left.ptr[z] * right.ptr[z];
                }            
                //delete lptr;
                //delete rptr;
            }
            matrix_assign(res, i * size, k * size, bres);
            //delete bptr;
        }
    }
    return res;
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

/*

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
*/

int main()
{
    time_t start, end;
    Matrix a(100, 120);
    Matrix b(120, 150);
    Matrix c(100, 150);
    Matrix d(100, 150);
    start = time(NULL);
    d = a * b;
    end = time(NULL);
    double diff2 = difftime(end, start);
    start = time(NULL);
    c = multiply_tile(a, b, 10);
    end = time(NULL);
    double diff1 = difftime(end, start);
    std::cout << diff1 << std::endl;
    std::cout << diff2 << std::endl;
    return 0;
}