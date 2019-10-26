#include <mkl.h>

#include <iostream>
#include <utility>
#include <vector>
#include <iomanip>
#include <stdexcept>
#include <numeric>

using namespace std;

class Matrix
{
public:
    Matrix(size_t r, size_t c) : nrow(r), ncol(c), data(r, vector<double>(c, 0)){};
    Matrix(size_t r, size_t c, vector<double> const &vec) : nrow(r), ncol(c), data(r, vector<double>(c, 0))
    {
        size_t vec_i = 0;
        for (size_t i = 0; i < nrow; i++)
        {
            for (size_t j = 0; j < ncol; j++)
            {
                data[i][j] = vec[vec_i++];
            }
        }
    };
    Matrix(size_t r, size_t c, double *flat) : nrow(r), ncol(c), data(r, vector<double>(c, 0))
    {
        size_t vec_i = 0;
        for (size_t i = 0; i < nrow; i++)
        {
            for (size_t j = 0; j < ncol; j++)
            {
                data[i][j] = flat[vec_i++];
            }
        }
    };
    double operator()(size_t row, size_t col) const { return data[row][col]; }
    double &operator()(size_t row, size_t col)
    {
        return data[row][col];
    }
    bool operator==(const Matrix &other) const
    {
        return nrow == other.nrow && ncol == other.ncol && data == other.data;
    }

    bool operator!=(const Matrix &other) const
    {
        return !(*this == other);
    }
    vector<double> getRow(int row) const
    {
        vector<double> axis(ncol);

        for (size_t i = 0; i < ncol; i++)
        {
            axis[i] = data[row][i];
        }

        return axis;
    }
    vector<double> getCol(int col) const
    {
        vector<double> axis(nrow);

        for (size_t i = 0; i < nrow; i++)
        {
            axis[i] = data[i][col];
        }

        return axis;
    }
    vector<double> getFlatData() const
    {
        vector<double> flat(nrow * ncol);
        size_t k = 0;
        for (size_t i = 0; i < nrow; i++)
        {
            for (size_t j = 0; j < ncol; j++)
            {
                flat[k++] = data[i][j];
            }
        }
        return flat;
    }
    friend ostream &operator<<(ostream &ostr, Matrix const &mat)
    {
        for (size_t i = 0; i < mat.nrow; ++i)
        {
            ostr << endl
                 << " ";
            for (size_t j = 0; j < mat.ncol; ++j)
            {
                ostr << " " << setw(2) << mat(i, j);
            }
        }

        return ostr;
    }

public:
    size_t nrow;
    size_t ncol;

private:
    vector<vector<double>> data;
};

Matrix multiply_naive(Matrix const &A, Matrix const &B)
{
    if (A.ncol != B.nrow)
    {
        throw out_of_range("Matrices can not be multiplied!!!");
    }

    Matrix result = Matrix(A.nrow, B.ncol);

    for (size_t i = 0; i < result.nrow; i++)
    {
        for (size_t j = 0; j < result.ncol; j++)
        {
            auto row = A.getRow(i);
            auto col = B.getCol(j);
            result(i, j) = inner_product(row.begin(), row.end(), col.begin(), 0.0);
        }
    }
    return result;
}

// https://software.intel.com/en-us/mkl-tutorial-c-multiplying-matrices-using-dgemm
Matrix multiply_mkl(Matrix A, Matrix B)
{
    size_t m = A.nrow; // A: m x k
    size_t k = A.ncol; // B: k x n
    size_t n = B.ncol; // C: m x n
    double alpha = 1.0;
    double beta = 0.0;

    auto A_vec = A.getFlatData();
    double *A_flat = A_vec.data();
    auto B_vec = B.getFlatData();
    double *B_flat = B_vec.data();

    Matrix result = Matrix(A.nrow, B.ncol);

    double *C_flat = result.getFlatData().data();

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                m, n, k, alpha, A_flat, k, B_flat, n, beta, C_flat, n);

    result = Matrix(A.nrow, B.ncol, C_flat);
    return result;
}

int main()
{
    size_t nrow = 2;
    size_t ncol = 2;
    Matrix A(nrow, ncol, vector<double>({1, 0, 0, 1}));
    Matrix B(nrow, ncol, vector<double>({1, 2, 3, 4}));
    cout << A << endl;
    cout << B << endl;
    cout << multiply_naive(A, B) << endl;
    cout << multiply_mkl(A, B) << endl;
}