UNAME_S := $(shell uname -s)
MKLROOT ?= ${HOME}/opt/conda
MKLEXT ?= so
CXXFLAGS := -Wall -shared -fPIC

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++11 -O3 -g -m64 \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS}
	
PY = python3
SUFFIX = $(shell python3-config --extension-suffix)
TARGET = _matrix$(SUFFIX)

all: $(TARGET)

$(TARGET): matrix.cpp
	$(CXX) $(CXXFLAGS) `$(PY) -m pybind11 --includes` $^ -o $@

.PHONY: test
test: $(TARGET)
	$(PY) matrix_test.py

.PHONY: bench
bench: $(TARGET)
	./matrix_benchmark.py

.PHONY: clean
clean:
	rm -rf *.so
