#include <iostream>
#include <vector>
#include <pybind11/pybind11.h>
#include <stdexcept>
#include <mkl.h>
#include <pybind11/stl.h>

using namespace std;
namespace py = pybind11;

class Matrix {
public:
    Matrix(size_t nrow, size_t ncol)
      : m_nrow(nrow), m_ncol(ncol)
    {
        size_t nelement = nrow * ncol;
        m_buffer = new double[nelement];
    }
    // TODO: copy and move constructors and assignment operators.

    // No bound check.
    double   operator() (size_t row, size_t col) const { return m_buffer[row*m_ncol + col]; }
    double & operator() (size_t row, size_t col)       { return m_buffer[row*m_ncol + col]; }
    double* get_buffer() { return m_buffer; }
    void set_buffer(const std::vector<int> &v)
    {
	   for(size_t i=0;i<(m_ncol*m_nrow);i++)
	      m_buffer[i] = v[i];
    }

    size_t nrow() const { return m_nrow; }
    size_t ncol() const { return m_ncol; }

    bool operator==(Matrix &rhs)
    {
       if (m_nrow!=rhs.nrow()&&m_ncol!=rhs.ncol())
          return 0;
       bool flag = 1;
       double* temp_buffer = rhs.get_buffer();
       for (size_t i=0;i<m_nrow*m_ncol;i++)
       {
          if(m_buffer[i]!=temp_buffer[i])
	  {
	     flag = 0;
	     break;
	  }
       }
       return flag;
    }

    friend ostream &operator<<(ostream &ostr, Matrix const &mat)
    {
        for (size_t i = 0; i < mat.nrow(); ++i)
        {
            ostr << endl
                 << " ";
            for (size_t j = 0; j < mat.ncol(); ++j)
            {
                ostr << mat(i, j);
            }
        }
        return ostr;
    }


public:
    size_t m_nrow=0;
    size_t m_ncol=0;
    double* m_buffer=nullptr;
};

vector<double> vector_linspace(int start, int stop) //create an increment vector
{
	vector<double> vec;
	for (int i=start; i<stop; i++)
	{
		vec.push_back((double)i);
	}
	return vec;
}

Matrix matrix_linspace(int row, int col)	//create an increment matrix with row majoring
{
	Matrix amatrix(row,col);
	for (size_t i=0; i<amatrix.nrow(); i++) // the i-th row
	{
		for (size_t j=0; j<amatrix.ncol(); j++) // the j-th column
		{
			amatrix(i,j) = i*amatrix.ncol() + j;
		}
	}
	return amatrix;
}

vector<double> dot_vector (Matrix const & mat, vector<double> const & vec)
{	//vector and martix multiplication
	if (mat.ncol() != vec.size())
	{
		throw std::out_of_range("matrix column differs from vector size");
	}

	std::vector<double> ret(mat.nrow());

	for (size_t i=0; i<mat.nrow(); ++i)
	{
		double v = 0;
		for (size_t j=0; j<mat.ncol(); ++j)
		{
			v += mat(i,j) * vec[j];
		}
		ret[i] = v;
	}

	return ret;
}


Matrix multiply_naive (Matrix const & mat1, Matrix const & mat2)
{	//matrix and matrix multiplication
	if (mat1.ncol() != mat2.nrow())
	{
		throw std::out_of_range(
			"the number of first matrix column "
			"differs from that of second matrix row");
	}

	Matrix ret(mat1.nrow(), mat2.ncol());

	for (size_t i=0; i<ret.nrow(); ++i)
	{
		for (size_t k=0; k<ret.ncol(); ++k)
		{
			double v = 0;
			for (size_t j=0; j<mat1.ncol(); ++j)
			{
				v += mat1(i,j) * mat2(j,k);
			}
			ret(i,k) = v;
		}
	}

	return ret;
}

Matrix multiply_mkl (Matrix  & mat1, Matrix & mat2)
{	// matrix and matrix multiplication function implement with cblas
    Matrix mat_result(mat1.nrow(), mat2.ncol());

    cblas_dgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        mat1.nrow(),
        mat2.ncol(),
        mat1.ncol(),
        1.0,
        mat1.get_buffer(),
        mat1.ncol(), //lda
        mat2.get_buffer(),
        mat2.ncol(), //ldb
        0.0,
        mat_result.get_buffer(),
        mat_result.ncol()
    );

    return mat_result;
}

//matrix tiling start
template<size_t N>
struct Block
{
    static constexpr const size_t NDIM = N;

    double   operator[] (size_t idx) const { return m_buffer[idx]; }
    double & operator[] (size_t idx)       { return m_buffer[idx]; }

    Block<N> & operator= (double v)
    {
        for (size_t i=0; i<N*N; ++i) { m_buffer[i] = v; }
        return *this;
    }

    Block<N> & operator+= (Block<N> const & other)
    {
        for (size_t i=0; i<N*N; ++i) { m_buffer[i] += other.m_buffer[i]; }
        return *this;
    }

    void save(Matrix & mat, size_t it, size_t jt);

    double m_buffer[N * N];
};

template<size_t N> void Block<N>::save(
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

template<size_t N>
struct Tiler
{
    static constexpr const size_t NDIM = N;

    void load(
        Matrix const & mat1, size_t it1, size_t jt1
      , Matrix const & mat2, size_t it2, size_t jt2
    );

    void multiply();

    Block<N> m_mat1; // row-major
    Block<N> m_mat2; // column-major
    Block<N> m_ret; // row-major
};

template<size_t N> void Tiler<N>::load(
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
            m_mat1[base_t + j] = mat1.m_buffer[base_s + j];
        }
    }

    const size_t ncol2 = mat2.ncol();

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base_t = i*NDIM;
        const size_t base_s = (it2*NDIM + i) * ncol2 + jt2*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_ret[base_t + j] = mat2.m_buffer[base_s + j];
        }
    }

    for (size_t i=0; i<NDIM; ++i)
    {
        const size_t base = i*NDIM;

        for (size_t j=0; j<NDIM; ++j)
        {
            m_mat2[j*NDIM + i] = m_ret[base + j];
        }
    }
}

template<size_t N> void Tiler<N>::multiply()
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
                v += m_mat1[base1 + j] * m_mat2[base2 + j];
            }
            m_ret[base1 + k] = v;
        }
    }
}

/*
 * Tiled matrix matrix multiplication.
 */
Matrix multiply_tile(Matrix const & mat1, Matrix const & mat2, size_t LSIZE)
{
    Matrix ret(mat1.nrow(), mat2.ncol());

    constexpr const size_t tsize = LSIZE / sizeof(double);

    const size_t nrow1 = mat1.nrow();
    const size_t ncol1 = mat1.ncol();
    const size_t nrow2 = mat2.nrow();
    const size_t ncol2 = mat2.ncol();

    const size_t ntrow1 = nrow1 / tsize;
    const size_t ntcol1 = ncol1 / tsize;
    const size_t ntrow2 = nrow2 / tsize;
    const size_t ntcol2 = ncol2 / tsize;

    Block<tsize> value;
    Tiler<tsize> tiler;

    for (size_t it=0; it<ntrow1; ++it)
    {
        for (size_t kt=0; kt<ntcol2; ++kt)
        {
            value = 0;
            for (size_t jt=0; jt<ntcol1; ++jt)
            {
                tiler.load(mat1, it, jt, mat2, jt, kt);
                tiler.multiply();
                value += tiler.m_ret;
            }
            value.save(ret, it, kt);
        }
    }

    return ret;
}
//matrix tiling stop

/*
   int main()
   {

   }
 */

PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "pybind11 matrix plugin"; // optional module docstring
    m.def("multiply_mkl", &multiply_mkl, "MKL matrix multiplication");
    m.def("multiply_naive", &multiply_naive, "naive matrix multiplication");
	m.def("multiply_tile", &multiply_tile, "tiling matrix multiplication");
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("get_buffer", &Matrix::get_buffer)
        .def("set_buffer", &Matrix::set_buffer)
        .def("__getitem__", [](const Matrix &self, pair<size_t, size_t> idx) { return self(idx.first, idx.second); })
        .def("__setitem__", [](Matrix &self, pair<size_t, size_t> idx, double val) { self(idx.first, idx.second) = val; })
	.def("__eq__",[](Matrix &lhs, Matrix &rhs){return lhs==rhs;});
}
