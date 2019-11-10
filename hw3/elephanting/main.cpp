#include <iostream>
#include <ctime>
#include "Matrix.h"

int main(int argc, char **argv)
{
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
    DGEMM(A, B, D);
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
