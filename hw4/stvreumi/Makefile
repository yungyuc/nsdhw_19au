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
CXXFLAGS := -std=c++17 -O3 -g -m64 -Wall -fPIC
CXX_MAINFLAGS := ${CXXFLAGS} \
	${PYBIND_INCLUDE} \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS}
CXX_ASSEMBLEFLAGS := -shared

TARGET := _matrix${PYBIND_POSTFIX}
OBJECT_FILE := main.o matrix.o tiling.o

.PHONY: default
default: ${TARGET}

_matrix${PYBIND_POSTFIX}: ${OBJECT_FILE}
	${CXX} $^ ${CXX_ASSEMBLEFLAGS} -o $@

main.o: main.cpp
	${CXX} -c $< ${CXX_MAINFLAGS} -o $@

matrix.o: matrix.cpp matrix.h
	${CXX} -c $< ${CXXFLAGS} -o $@

tiling.o: tiling.cpp tiling.h
	${CXX} -c $< ${CXXFLAGS} -o $@

.PHONY: clean
clean:
	rm -rf ${TARGET} ${OBJECT_FILE} *.o *.dSYM/