# MKL linker
# https://software.intel.com/en-us/articles/intel-mkl-link-line-advisor

MKLROOT = /home/ryan/intel/mkl

MKLLINKLINE := \
	-Wl,--start-group \
	${MKLROOT}/lib/intel64/libmkl_intel_ilp64.a \
	${MKLROOT}/lib/intel64/libmkl_sequential.a \
	${MKLROOT}/lib/intel64/libmkl_core.a \
	-Wl,--end-group \
	-lpthread -lm -ldl

CXX = g++

CXXFLAGS := -std=c++11 -O3 -g -shared -m64 -DMKL_ILP64 -fPIC

_matrix.*.so: Matrix.cpp Matrix.h
	${CXX} ${CXXFLAGS} `python3 -m pybind11 --includes ` $< -o _matrix`python3-config --extension-suffix` ${MKLLINKLINE}

clean:
	@rm -f *.o *.so per*

bench: _matrix.*.so bench.py
	@python3 bench.py

test: _matrix.*.so test_matrix.py
	@python3 test_matrix.py
