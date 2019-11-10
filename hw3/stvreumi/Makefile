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

PYBIND_INCLUDE := $(shell python3 -m pybind11 --includes)
PYBIND_POSTFIX := $(shell python3-config --extension-suffix)

CXX := g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 -Wall -shared -fPIC \
	${PYBIND_INCLUDE} \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS} \

BINS := _matrix${PYBIND_POSTFIX}

.PHONY: default
default: ${BINS}

_matrix${PYBIND_POSTFIX}: matrix.cpp
	${CXX} $< ${CXXFLAGS} -o $@

.PHONY: test
test: ${BINS}
	python3 validate.py

.PHONY: clean
clean:
	rm -rf ${BINS} *.o *.dSYM/
