# modify from:
# https://github.com/yungyuc/nsd/blob/master/notebook/matrix/Makefile
FLAGS ?=

UNAME_S := $(shell uname -s)

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

# set LD_PRELOAD workaround for mkl
MKL_ROOT:=${HOME}/opt/conda
MKL_LIB_DIR:=${MKL_ROOT}/lib
MKL_LIBS:=${MKL_LIB_DIR}/libmkl_def.so
MKL_LIBS:=${MKL_LIBS}:${MKL_LIB_DIR}/libmkl_avx2.so
MKL_LIBS:=${MKL_LIBS}:${MKL_LIB_DIR}/libmkl_core.so
MKL_LIBS:=${MKL_LIBS}:${MKL_LIB_DIR}/libmkl_intel_lp64.so
MKL_LIBS:=${MKL_LIBS}:${MKL_LIB_DIR}/libmkl_sequential.so

PYBIND_INCLUDE := $(shell python3 -m pybind11 --includes)
PYBIND_POSTFIX := $(shell python3-config --extension-suffix)

CXX := g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 -Wall -shared -fPIC \
	${PYBIND_INCLUDE} \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS} \

MATRIX_TARGET := _matrix${PYBIND_POSTFIX}

.PHONY: default
default: ${MATRIX_TARGET}

_matrix${PYBIND_POSTFIX}: matrix.cpp
	${CXX} $< ${CXXFLAGS} -o $@

.PHONY: test
test: ${MATRIX_TARGET}
	python3 -m pytest -v -s

.PHONY: bench
bench: ${MATRIX_TARGET}
	export LD_PRELOAD=${MKL_LIBS}; python3 bench.py

.PHONY: clean
clean:
	rm -rf ${MATRIX_TARGET} *.o *.dSYM/
