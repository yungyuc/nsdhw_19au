CXX = g++
CXXFLAGS = --std=c++11 -Wall -O3

TARGET = _matrix.*.so
all: $(TARGET)
.PHONY : clean run test

Matrix.o: Matrix.cpp Matrix.hpp
	$(CXX) $(CXXFLAGS) -c  $< -o $@

$(TARGET): Matrix_pybind.cpp Matrix.o Matrix.hpp
	$(CXX) $(CXXFLAGS) --shared -fPIC `python3 -m pybind11 --includes` $^ -o _matrix`python3-config --extension-suffix`

test: $(TARGET)
	pytest test_Matrix.py

clean:
	rm -f *.o $(TARGET)
	rm -rf __pycache__ .pytest_cache
