#include <iostream>
#include <mkl.h>
#include <ctime>
#include "Matrix.h"

using namespace std;

int main(int argc, char **argv)
{
    double alpha = 1.0;
    double beta = 0.0;
    //std::clock_t c_start = std::clock();
    size_t Arow = 1000;
    size_t Acol = 1200;
    size_t Brow = 1200;
    size_t Bcol = 1500;
    size_t Crow = 1000;
    size_t Ccol = 1500;
    size_t Drow = Crow;
    size_t Dcol = Ccol;
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
