FLAGS ?=

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
MKLROOT ?= /home/ryan/intel/mkl
CXXFLAGS :=
endif

ifeq ($(UNAME_S),Linux)
MKLROOT ?= ${HOME}/intel/mkl
CXXFLAGS := -Wl,--no-as-needed
endif

MKLLINKLINE := \
	${MKLROOT}/lib/intel64/libmkl_intel_lp64.a \
	${MKLROOT}/lib//intel64/libmkl_sequential.a \
	${MKLROOT}/lib/intel64/libmkl_core.a \
	${MKLROOT}/lib/intel64/libmkl_blacs_intelmpi_lp64.a \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS}

gg: Matrix.cpp Makefile
	${CXX} ${CXXFLAGS} -o $@ $<


g++ -std=c++17 -L${MKLROOT}/lib/intel64 -Wl,--no-as-needed -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lpthread -lm -ldl -m64 -I${MKLROOT}/include Matrix.cpp -o gg
