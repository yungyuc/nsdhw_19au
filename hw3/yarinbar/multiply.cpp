
#include <matrix.h>
#include <pybind11/pybind11.h>

Matrix multiply(Matrix a, Matrix b){
	
	// making sure the dims work
	if(a.nrow != b.ncol)
		return nullptr;
	
	Matrix c = Matrix(a.nrow, b.ncol)
	
	for(int i = 0; i < a.ncol; i++)
    {
       for(int j = 0; j < b.ncol; j++)
        {
           for(int k = 0; k < c1; k++)//columns of first matrix || rows of second matrix
                  c(i, j) += a(i, k) * b(k, j);
        }
    }
		
	return c;
	
	
}


PYBIND11_MODULE(mat_mul, mod){
    mod.def("multiply", &multiply, "");
}

