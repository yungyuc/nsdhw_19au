CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PY = python3

all: _matrix.*.so

_matrix.*.so: matrix.cpp
	$(CXX) $(CXXFLAGS) `$(PY) -m pybind11 --includes` matrix.cpp -o _matrix`$(PY)-config --extension-suffix`

.PHONY: test
test: _matrix.*.so
	$(PY) pybind_test.py

.PHONY: clean
clean:
	rm -rf *.so __pycache__ .pytest_cache