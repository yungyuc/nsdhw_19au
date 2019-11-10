#include <iostream>

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