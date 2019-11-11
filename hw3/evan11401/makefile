FLAGS ?=
PY = python

MKLROOT ?= ${HOME}/opt/conda
MKLEXT ?= so
CXXFLAGS := -Wl,--no-as-needed

MKLLINKLINE := \
	${MKLROOT}/lib/libmkl_intel_lp64.${MKLEXT} \
	${MKLROOT}/lib/libmkl_sequential.${MKLEXT} \
	${MKLROOT}/lib/libmkl_core.${MKLEXT} \
	-lpthread -lm -ldl

CXX = g++
CXXFLAGS := ${CXXFLAGS} \
	-std=c++17 -O3 -g -m64 \
	-shared -fPIC `python3 -m pybind11 --includes` \
	-I${MKLROOT}/include \
	${MKLLINKLINE} \
	${FLAGS}

_matrix.so: _matrix.cpp
	${CXX} ${CXXFLAGS} $< -o $@

.PHONY: clean test
clean:
	rm -rf  *.so
test: _matrix.so
	$(PY) test.py
