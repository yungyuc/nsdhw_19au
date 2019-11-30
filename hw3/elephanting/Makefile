# MKL linker
# https://software.intel.com/en-us/articles/intel-mkl-link-line-advisor

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S),Linux)
MKLROOT ?= ${HOME}/opt/conda
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++

CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 \
	-I${MKLROOT}/include \
	-fPIC -shared \
	${MKLLINKLINE} \
	${FLAGS}

MKL_ROOT:=${HOME}/opt/conda
MKL_LIB_DIR:=${MKL_ROOT}/lib
MKL_LIBS:=${MKL_LIB_DIR}/libmkl_def.so
MKL_LIBS:=${MKL_LIBS}:${MKL_LIB_DIR}/libmkl_avx2.so
MKL_LIBS:=${MKL_LIBS}:${MKL_LIB_DIR}/libmkl_core.so
MKL_LIBS:=${MKL_LIBS}:${MKL_LIB_DIR}/libmkl_intel_lp64.so
MKL_LIBS:=${MKL_LIBS}:${MKL_LIB_DIR}/libmkl_sequential.so

_matrix.*.so: Matrix.cpp Matrix.h
	export LD_PRELOAD=${MKL_LIBS}; ${CXX} ${CXXFLAGS} `python3 -m pybind11 --includes ` $< -o _matrix`python3-config --extension-suffix`

clean:
	@rm -rf *.so *.o  __py*

bench: _matrix.*.so
	@export LD_PRELOAD=${MKL_LIBS}; python3 bench.py

test: _matrix.*.so test_matrix.py
	@python3 bench.py
	@pytest -v test_matrix.py
