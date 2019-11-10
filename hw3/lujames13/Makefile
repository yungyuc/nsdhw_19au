FLAGS ?=

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /opt/intel/mkl
MKLEXT ?= a
CXXFLAGS :=
endif

ifeq ($(UNAME_S),Linux)
MKLROOT ?= ${HOME}/opt/conda
MKLEXT ?= so
CXXFLAGS := -Wall -shared -fPIC
endif

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++
PYBIND = $(shell python3 -m pybind11 --includes)
CXXFLAGS := ${CXXFLAGS} \
	-std=c++11 -O3  \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS} \
	${PYBIND}

.PHONY: default
default: _matrix.so

_matrix.so: matrix.cpp Makefile
	${CXX} ${CXXFLAGS} matrix.cpp -o _matrix.so

.PHONY: clean
clean:
	rm -rf *.o *.dSYM/ ${BINS}
