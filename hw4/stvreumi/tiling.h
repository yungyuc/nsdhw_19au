#include "matrix.h"

#ifndef TILING_H
#define TILING_H

class Block
{
public:
    Block(size_t N);
    ~Block();

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    Block & operator= (double v)
    {
        for (size_t i=0; i<NDIM*NDIM; ++i) { m_buffer[i] = v; }
        return *this;
    }

    Block & operator+= (Block const & other)
    {
        for (size_t i=0; i<NDIM*NDIM; ++i) { m_buffer[i] += other.m_buffer[i]; }
        return *this;
    }

    void save(Matrix & mat, size_t it, size_t jt);

    size_t NDIM;
    double *m_buffer;
};

class Tiler
{
public:
    Tiler(size_t N);
    ~Tiler();
    double NDIM;

    void load(
        Matrix const & mat1, size_t it1, size_t jt1
      , Matrix const & mat2, size_t it2, size_t jt2
    );

    void multiply();

    Block *m_mat1; // row-major
    Block *m_mat2; // column-major
    Block *m_ret; // row-major
};


#endif //TILING_H