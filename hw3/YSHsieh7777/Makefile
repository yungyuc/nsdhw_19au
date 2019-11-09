CXX = g++
CXXFLAGS = -O3 -Wall -shared -std=c++11 -fPIC
PY = python3
TARGET = _matrix.*.so

all: $(TARGET)

$(TARGET): matrix.cpp
	$(CXX) $(CXXFLAGS) `$(PY) -m pybind11 --includes` matrix.cpp -o _matrix`$(PY)-config --extension-suffix`

.PHONY: test
test: $(TARGET)
	$(PY) pybind_test.py

.PHONY: clean
clean:
	rm -rf *.so __pycache__ .pytest_cache