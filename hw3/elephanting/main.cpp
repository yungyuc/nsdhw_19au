#include <iostream>
#include <mkl.h>
#include <ctime>
#include "Matrix.h"

using namespace std;

int main(int argc, char **argv)
{
    double alpha, beta = 1.0, 0.0;
    std::clock_t c_start = std::clock();
    size_t Arow, Acol = 1000, 1200;
    size_t Brow, Bcol = 1200, 1500;
    size_t Crow, Ccol = 1000, 1500;
    size_t Drow, Dcol = 1000, 1500;
    Matrix A(Arow, Acol), B(Brow, Bcol), C(Crow, Ccol), D(Drow, Dcol);
    C = A * B;
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                A.nrow(), B.ncol(), A.ncol(), alpha,
                A.buffer(), A.ncol(), B.buffer(), B.ncol(), beta, D.buffer(), D.ncol());
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
