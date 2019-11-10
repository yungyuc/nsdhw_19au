#include <iostream>
#include <ctime>
#include "Matrix.h"
#include "mkl.h"

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
    std::cout << c_end - c_start << std::endl;
    size_t c_start2 = std::clock();
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                A.nrow(), B.ncol(), A.ncol(), alpha,
                A.buffer(), A.ncol(), B.buffer(), B.ncol(), beta, D.buffer(), D.ncol());
    size_t c_end2 = std::clock();
    std::cout << c_end2 - c_start2 << std::endl;
    bool ret = false;
    ret = (C == D);
    if (ret == true)
    {
        std::cout << "true" << std::endl;
    }
    else
    {
        std::cout << "false" << std::endl;
    }
}
