#include "matrix.h"

#ifndef TILING_H
#define TILING_H

template<size_t N>
struct Block;

template<size_t N> void Block<N>::save(
    Matrix & mat, size_t it, size_t jt
);

template<size_t N>
struct Tiler;

template<size_t N> void Tiler<N>::load(
    Matrix const & mat1, size_t it1, size_t jt1
  , Matrix const & mat2, size_t it2, size_t jt2
);

template<size_t N> void Tiler<N>::multiply();

/*
 * Throw an exception if the shapes of the two matrices don't support
 * multiplication.
 */
void validate_multiplication(Matrix const & mat1, Matrix const & mat2);

/*
 * Tiled matrix matrix multiplication.
 */
template<size_t LSIZE>
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2);

#endif //TILING_H