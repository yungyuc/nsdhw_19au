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
CXXFLAGS := -Wl,--no-as-needed
endif

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 \
	${FLAGS}

# For MKL

MKLLINKLINE := ${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

INCLUDES = 	-I${MKLROOT}/include

# For pybind11

TARGET = _matrix`python3-config --extension-suffix`

INCLUDES += `python3 -m pybind11 --includes`

CXXFLAGS += -shared -Wall -fPIC


build: matrix.cpp
	${CXX} ${CXXFLAGS} ${INCLUDES} -o ${TARGET} $< ${MKLLINKLINE}
	export PRELOAD_MKL=True

test: build
	python3 matrix_test.py

bench: build
	bash setup.sh

.PHONY: clean
clean:
	rm -rf *.so