CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PY = python3
PYTEST = pytest

all: angle.cpython-37m-x86_64-linux-gnu.so

angle.cpython-37m-x86_64-linux-gnu.so: angle.cpp
	$(CXX) $(CXXFLAGS) `$(PY) -m pybind11 --includes` angle.cpp -o angle`$(PY)-config --extension-suffix`

.PHONY: test
test: angle.cpython-37m-x86_64-linux-gnu.so
	$(PYTEST) pybind_test.py

.PHONY: clean
clean:
	rm -rf *.so __pycache__ .pytest_cache