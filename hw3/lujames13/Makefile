FLAGS ?=

UNAME_S := $(shell uname -s)

MKLROOT ?= ${HOME}/opt/conda
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed -Wall -shared -fPIC

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl
CXX = g++
PYBIND = $(shell python3 -m pybind11 --includes)
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS}
SUFFIX = $(shell python3-config --extension-suffix)
TARGET = _matrix${SUFFIX}

all: $(TARGET)

$(TARGET): matrix.cpp
	${CXX} ${CXXFLAGS} '${PYBIND}' $^ -o $@

.PHONY: clean
clean:
	rm -rf *.so
