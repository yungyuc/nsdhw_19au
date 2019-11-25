#include <iostream>

#ifndef MATRIX_H
#define MATRIX_H
// copy from 07_matrix_matrix.cpp
class Matrix {

public:

    Matrix(size_t nrow, size_t ncol);

    bool operator==(Matrix const & mat) const;

    bool operator!=(Matrix const & mat) const;

    Matrix(Matrix const & other);

    Matrix & operator=(Matrix const & other);

    Matrix(Matrix && other);

    Matrix & operator=(Matrix && other);

    ~Matrix();

    double   operator() (size_t row, size_t col) const;
    double & operator() (size_t row, size_t col);

    size_t nrow() const;
    size_t ncol() const;

    size_t size() const;
    double buffer(size_t i) const;
    double & buffer(size_t i);
    double * data() const;
    double * m_buffer = nullptr;
private:

    size_t index(size_t row, size_t col) const;

    void reset_buffer(size_t nrow, size_t ncol);

    size_t m_nrow = 0;
    size_t m_ncol = 0;
    

};
#endif //MATRIX_H