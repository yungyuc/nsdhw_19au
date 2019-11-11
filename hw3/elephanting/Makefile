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

${PRELOAD_MKL} = true

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

_matrix.*.so: Matrix.cpp Matrix.h
	${CXX} ${CXXFLAGS} `python3 -m pybind11 --includes ` $< -o _matrix`python3-config --extension-suffix`

clean:
	@rm -f *.o *.so

bench: _matrix.*.so bench.py
	@python3 bench.py

test: _matrix.*.so test_matrix.py
	@python3 bench.py
	@pytest -v test_matrix.py
