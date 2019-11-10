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

void mkl_MM(Matrix const &, Matrix const &, Matrix &);

bool ifequal(Matrix &, Matrix &);

Matrix operator*(Matrix const &, Matrix const &);

/* broadcasting */
Matrix operator*(double const &, Matrix const &);

Matrix operator*(Matrix const &, double const &);