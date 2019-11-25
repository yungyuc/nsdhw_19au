#include "tiling.h"

Block::Block(size_t N): NDIM(N){
    m_buffer = new double[N*N];
}
Block::~Block(){
    delete[] m_buffer;
}

void Block::save(
    Matrix & mat, size_t it, size_t jt
)
{
    const size_t ncol = mat.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_s = i*NDIM;
        const size_t base_t = (it*NDIM + i) * ncol + jt*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            mat.m_buffer[base_t + j] = m_buffer[base_s + j];
        }
    }
}

Tiler::Tiler(size_t N): NDIM(N){
    m_mat1 = new Block(N);
    m_mat2 = new Block(N);
    m_ret = new Block(N); 
}
Tiler::~Tiler(){
    delete m_mat1;
    delete m_mat2;
    delete m_ret;
}

void Tiler::load(
    Matrix const & mat1, size_t it1, size_t jt1
  , Matrix const & mat2, size_t it2, size_t jt2
)
{
    const size_t ncol1 = mat1.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_t = i*NDIM;
        const size_t base_s = (it1*NDIM + i) * ncol1 + jt1*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            (*m_mat1)[base_t + j] = mat1.m_buffer[base_s + j];
        }
    }

    const size_t ncol2 = mat2.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_t = i*NDIM;
        const size_t base_s = (it2*NDIM + i) * ncol2 + jt2*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            (*m_ret)[base_t + j] = mat2.m_buffer[base_s + j];
        }
    }

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base = i*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            (*m_mat2)[j*NDIM + i] = (*m_ret)[base + j];
        }
    }
}

void Tiler::multiply()
{
    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base1 = i*NDIM;

        for (size_t k=0; k<NDIM; ++k)
        {
            const size_t base2 = k*NDIM;

            double v = 0;
            for (size_t j=0; j<NDIM; ++j)
            {
                v += (*m_mat1)[base1 + j] * (*m_mat2)[base2 + j];
            }
            (*m_ret)[base1 + k] = v;
        }
    }
}
