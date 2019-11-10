#include <iostream>
#include <random>
#include <stdexcept>
#include <cmath>
#include <ctime>
#include "mkl.h"

using namespace std;

class Matrix
{
public:
    Matrix(size_t, size_t);
    ~Matrix();
    Matrix& operator=(Matrix const &);
    bool operator==(Matrix const &);
    Matrix(const Matrix &);
    double& operator()(size_t const, size_t const);
    double operator()(size_t const, size_t const) const;
    double* buffer() const;
    size_t nrow() const;
    size_t ncol() const;
private:
    size_t m_ncol;
    size_t m_nrow;
    double* m_buffer = nullptr;
    void resetbuf(size_t, size_t);
};

Matrix operator*(Matrix const &A1, Matrix const &A2);

/* broadcasting */
Matrix operator*(double const &c, Matrix const &A);

Matrix operator*(Matrix const &A, double const &c);


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
    for (size_t idx = 0; idx < m_nrow * m_ncol; idx++)
    {
        if (abs(this->m_buffer[idx] - other.m_buffer[idx]) > 0.01)
        {
            ret = false;
            break;
        }        
    }
    return ret;
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
    return m_buffer[(row - 1) * m_ncol + col - 1];
}

double Matrix::operator()(size_t row, size_t col) const
{
    return m_buffer[(row - 1) * m_ncol + col - 1];
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

size_t Matrix::nrow() const
{
    return m_nrow;
}

size_t Matrix::ncol() const
{
    return m_ncol;
}

Matrix operator*(Matrix const &A1, Matrix const &A2)
{
    if (A1.ncol() != A2.nrow())
    {
        throw std::out_of_range("invalid matrix size of matrix multiplication.");
    }
    Matrix ret(A1.nrow(), A2.ncol());
    for (size_t row = 1; row <= ret.nrow(); row++)
    {
        for (size_t col = 1; col <= ret.ncol(); col++)
        {
	    double tmp = 0;
            for (size_t k = 1; k <= A1.ncol(); k++)
            {
                tmp += (A1(row, k) * A2(k, col));
            }
	    ret(row, col) = tmp;
        }
    }
    return ret;
}

Matrix operator*(double const &c, Matrix const &A)
{
    Matrix ret(A.nrow(), A.ncol());
    for (size_t row = 1; row <= ret.nrow(); row++)
    {
        for (size_t col = 1; col <= ret.ncol(); col++)
        {
            ret(row, col) = c * ret(row, col);
        }
    }
}

Matrix operator*(Matrix const &A, double const &c)
{
    Matrix ret(A.nrow(), A.ncol());
    for (size_t row = 1; row <= ret.nrow(); row++)
    {
        for (size_t col = 1; col <= ret.ncol(); col++)
        {
            ret(row, col) = c * ret(row, col);
        }
    }
}

int main(int argc, char **argv)
{
    double alpha = 1.0;
    double beta = 0.0;
    size_t Arow = 1000;
    size_t Acol = 1200;
    size_t Brow = 1200;
    size_t Bcol = 1500;
    size_t Crow = 1000;
    size_t Ccol = 1500;
    size_t Drow = Crow;
    size_t Dcol = Ccol;
    Matrix A(Arow, Acol), B(Brow, Bcol), C(Crow, Ccol), D(Drow, Dcol);
    size_t c_start = std::clock();
    C = A * B;
    size_t c_end = std::clock();
    cout << c_end - c_start <<endl;
    size_t c_start2 = std::clock();    
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                A.nrow(), B.ncol(), A.ncol(), alpha,
                A.buffer(), A.ncol(), B.buffer(), B.ncol(), beta, D.buffer(), D.ncol());
    size_t c_end2 = std::clock();
    cout << c_end2 - c_start2 <<endl;
    bool ret = false;
    ret = (C == D);
    if (ret == true)
    {
        cout << "true" << endl;
    }
    else
    {
        cout << "false" << endl;
    }
    return 0;
}

