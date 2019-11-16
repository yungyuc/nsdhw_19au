#ifdef PYTHON_LIB
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#endif
#include <mkl.h>

#include <iostream>
// #include <utility>
#include <vector>
#include <stdexcept>
#include <numeric>
#include <algorithm>

using namespace std;

#ifdef PYTHON_LIB
namespace py = pybind11;
#endif

class Matrix
{
public:
    Matrix(size_t r, size_t c) : m_nrow(r), m_ncol(c), data(r, vector<double>(c, 0)){};
    Matrix(size_t r, size_t c, vector<double> const &vec) : m_nrow(r), m_ncol(c), data(r, vector<double>(c, 0))
    {
        size_t vec_i = 0;
        for (size_t i = 0; i < nrow(); i++)
        {
            for (size_t j = 0; j < ncol(); j++)
            {
                data[i][j] = vec[vec_i++];
            }
        }
    };
    Matrix(size_t r, size_t c, double *flat) : m_nrow(r), m_ncol(c), data(r, vector<double>(c, 0))
    {
        size_t vec_i = 0;
        for (size_t i = 0; i < nrow(); i++)
        {
            for (size_t j = 0; j < ncol(); j++)
            {
                data[i][j] = flat[vec_i++];
            }
        }
    };
    Matrix(vector<vector<double>> d) : m_nrow(d.size()), m_ncol((d.size()) ? d[0].size() : 0), data(d)
    {
    }
    Matrix(const Matrix &) = default;
    Matrix(Matrix &&) = default;
    ~Matrix() = default;
    Matrix &operator=(const Matrix &) = default;
    Matrix &operator=(Matrix &&) = default;
    double operator()(size_t row, size_t col) const { return data[row][col]; }
    double &operator()(size_t row, size_t col)
    {
        return data[row][col];
    }
    bool operator==(const Matrix &other) const
    {
        return nrow() == other.nrow() && ncol() == other.ncol() && data == other.data;
    }

    bool operator!=(const Matrix &other) const
    {
        return !(*this == other);
    }
    bool is_same_size(const Matrix &other) const
    {
        return (nrow() == other.nrow()) && (ncol() == other.ncol());
    }
    Matrix &operator+=(const Matrix &other)
    {
        if (!is_same_size(other))
            throw out_of_range("Matrices can not be added!!!");

        for (size_t i = 0; i < nrow(); i++)
        {
            for (size_t j = 0; j < ncol(); j++)
            {
                data[i][j] += other(i, j);
            }
        }
        return *this;
    }
    vector<double> getRow(int row) const
    {
        vector<double> axis(ncol());

        for (size_t i = 0; i < ncol(); i++)
        {
            axis[i] = data[row][i];
        }

        return axis;
    }
    vector<double> getCol(int col) const
    {
        vector<double> axis(nrow());

        for (size_t i = 0; i < nrow(); i++)
        {
            axis[i] = data[i][col];
        }

        return axis;
    }
    vector<double> getFlatData() const
    {
        vector<double> flat(nrow() * ncol());
        size_t k = 0;
        for (size_t i = 0; i < nrow(); i++)
        {
            for (size_t j = 0; j < ncol(); j++)
            {
                flat[k++] = data[i][j];
            }
        }
        return flat;
    }

    vector<double> loadBlock(size_t u, size_t v, size_t tile) const
    {
        vector<double> flat(tile * tile);
        size_t k = 0;
        for (size_t i = 0; i < tile; i++)
        {
            for (size_t j = 0; j < tile; j++)
            {
                flat[k++] = data[u + i][v + j];
            }
        }
        return flat;
    }
    void saveBlock(size_t u, size_t v, Matrix const &block)
    {
        size_t tile_size = block.ncol();
        for (size_t i = 0; i < tile_size; i++)
        {
            for (size_t j = 0; j < tile_size; j++)
            {
                data[u + i][v + j] = block(i, j);
            }
        }
    }
    string repr()
    {
        string s = "";
        for (size_t i = 0; i < nrow(); ++i)
        {
            for (size_t j = 0; j < ncol(); ++j)
            {
                s += to_string(this->operator()(i, j)) + " ";
            }
            s += "\n";
        }

        return s;
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
    size_t nrow() const { return m_nrow; };
    size_t ncol() const { return m_ncol; };

private:
    size_t m_nrow;
    size_t m_ncol;
    vector<vector<double>> data;
};

Matrix multiply_naive(Matrix const &A, Matrix const &B)
{
    if (A.ncol() != B.nrow())
    {
        throw out_of_range("Matrices can not be multiplied!!!");
    }

    Matrix result = Matrix(A.nrow(), B.ncol());

    // for (size_t i = 0; i < result.nrow(); i++)
    // {
    //     for (size_t j = 0; j < result.ncol(); j++)
    //     {
    //         auto row = A.getRow(i);
    //         auto col = B.getCol(j);
    //         result(i, j) = inner_product(row.begin(), row.end(), col.begin(), 0.0);
    //     }
    // }

    double value = 0;
    for (size_t i = 0; i < result.nrow(); i++)
    {
        for (size_t j = 0; j < result.ncol(); j++)
        {
            value = 0;
            for (size_t k = 0; k < A.ncol(); k++)
            {
                value += A(i, k) * B(k, j);
            }
            result(i, j) = value;
        }
    }

    return result;
}

Matrix multiply_tile(Matrix const &A, Matrix const &B, size_t tile_size)
{
    if (A.ncol() != B.nrow())
    {
        throw out_of_range("Matrices can not be multiplied!!!");
    }
    auto sizes = {A.nrow(), A.ncol(), B.ncol()};
    if (*min_element(sizes.begin(), sizes.end()) < tile_size)
    {
        throw out_of_range("Tile size is too big!!!");
    }
    Matrix result = Matrix(A.nrow(), B.ncol());

    for (size_t i = 0; i < result.nrow(); i += tile_size)
    {
        for (size_t j = 0; j < result.ncol(); j += tile_size)
        {
            Matrix block_result = Matrix(tile_size, tile_size);
            for (size_t k = 0; k < A.ncol(); k += tile_size)
            {
                Matrix blockA = Matrix(tile_size, tile_size, A.loadBlock(i, k, tile_size));
                Matrix blockB = Matrix(tile_size, tile_size, B.loadBlock(k, j, tile_size));
                // cout << blockA << "," << blockB << endl;
                Matrix partial_result = multiply_naive(blockA, blockB);
                block_result += partial_result;
            }
            result.saveBlock(i, j, block_result);
        }
    }
    return result;
}

// https://software.intel.com/en-us/mkl-tutorial-c-multiplying-matrices-using-dgemm
Matrix multiply_mkl(Matrix A, Matrix B)
{
    size_t m = A.nrow(); // A: m x k
    size_t k = A.ncol(); // B: k x n
    size_t n = B.ncol(); // C: m x n
    double alpha = 1.0;
    double beta = 0.0;

    auto A_vec = A.getFlatData();
    double *A_flat = A_vec.data();
    auto B_vec = B.getFlatData();
    double *B_flat = B_vec.data();

    double *C_flat = new double[m * n];

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
                m, n, k, alpha, A_flat, k, B_flat, n, beta, C_flat, n);

    Matrix result(m, n, C_flat);
    delete C_flat;

    return result;
}

#ifdef PYTHON_LIB
PYBIND11_MODULE(_matrix, m)
{
    m.doc() = "pybind11 matrix plugin"; // optional module docstring
    m.def("multiply_mkl", &multiply_mkl, "A function which calculates product of two matrices using MKL");
    m.def("multiply_naive", &multiply_naive, "A function which calculates product of two matrices using naive method");
    m.def("multiply_tile", &multiply_tile, "A function which calculates product of two matrices using tile method");
    py::class_<Matrix>(m, "Matrix")
        .def(py::init<size_t, size_t>())
        .def(py::init<size_t, size_t, vector<double>>())
        .def(py::init<vector<vector<double>>>())
        .def_property_readonly("nrow", &Matrix::nrow)
        .def_property_readonly("ncol", &Matrix::ncol)
        .def("__repr__", &Matrix::repr)
        .def("__eq__", [](const Matrix &self, const Matrix &other) { return self == other; })
        .def("__ne__", [](const Matrix &self, const Matrix &other) { return self != other; })
        .def("__getitem__", [](const Matrix &self, pair<size_t, size_t> idx) { return self(idx.first, idx.second); })
        .def("__setitem__", [](Matrix &self, pair<size_t, size_t> idx, double val) { self(idx.first, idx.second) = val; });
}
#else
int main()
{
    Matrix A({{1, 0},
              {0, 1}});
    Matrix B({{1, 2},
              {3, 4}});

    cout << A << endl;
    cout << B << endl;
    cout << multiply_tile(A, B, 1) << endl;
    // cout << multiply_mkl(A, B) << endl;

    A = Matrix({{1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1}});
    B = Matrix({{1, 2, 3, 4},
                {5, 6, 7, 8},
                {9, 10, 11, 12},
                {13, 14, 15, 16}});
    cout << A << endl;
    cout << B << endl;
    cout << multiply_tile(A, B, 2) << endl;
    return 0;
}
#endif