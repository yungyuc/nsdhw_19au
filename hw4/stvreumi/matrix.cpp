#include "matrix.h"

// copy from 07_matrix_matrix.cpp
Matrix::Matrix(size_t nrow, size_t ncol)
    : m_nrow(nrow), m_ncol(ncol)
{
    reset_buffer(nrow, ncol);
}

bool Matrix::operator==(Matrix const & mat) const
{
    if (size() != mat.size())
    {
        return false;
    }

    if (nrow() != mat.nrow() || ncol() != mat.ncol())
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

bool Matrix::operator!=(Matrix const & mat) const 
{ 
    return !operator==(mat); 
}

Matrix::Matrix(Matrix const & other)
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

Matrix & Matrix::operator=(Matrix const & other)
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

Matrix::Matrix(Matrix && other)
    : m_nrow(other.m_nrow), m_ncol(other.m_ncol)
{
    reset_buffer(0, 0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_ncol, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
}

Matrix & Matrix::operator=(Matrix && other)
{
    if (this == &other) { return *this; }
    reset_buffer(0, 0);
    std::swap(m_nrow, other.m_nrow);
    std::swap(m_ncol, other.m_ncol);
    std::swap(m_buffer, other.m_buffer);
    return *this;
}

Matrix::~Matrix()
{
    reset_buffer(0, 0);
}

double   Matrix::operator() (size_t row, size_t col) const 
{ 
    return m_buffer[index(row, col)]; 
}
    
double & Matrix::operator() (size_t row, size_t col)
{ 
    return m_buffer[index(row, col)]; 
}

size_t Matrix::nrow() const 
{ 
    return m_nrow; 
}
    
size_t Matrix::ncol() const 
{ 
    return m_ncol; 
}

size_t Matrix::size() const 
{ 
    return m_nrow * m_ncol; 
}
    
double Matrix::buffer(size_t i) const 
{ 
    return m_buffer[i]; 
}

double & Matrix::buffer(size_t i) 
{ 
    return m_buffer[i]; 
}

double * Matrix::data() const 
{ 
    return m_buffer; 
}

size_t Matrix::index(size_t row, size_t col) const
{
    return row*m_ncol + col;
}

void Matrix::reset_buffer(size_t nrow, size_t ncol)
{
    if (m_buffer) { delete[] m_buffer; }
    const size_t nelement = nrow * ncol;
    if (nelement) { m_buffer = new double[nelement]; }
    else          { m_buffer = nullptr; }
    m_nrow = nrow;
    m_ncol = ncol;
}