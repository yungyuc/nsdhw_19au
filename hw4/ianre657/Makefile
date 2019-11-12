CXX = g++
CXXFLAGS = --std=c++11 -Wall -O3 -fPIC


CONDAROOT = /opt/conda
INCLUDE += -I$(CONDAROOT)/include
LDFLAGS += -L$(CONDAROOT)/lib -lmkl_rt -lpthread -lm -ldl


TARGET = _matrix.*.so

all: $(TARGET)

.PHONY : clean run test

Matrix.o: Matrix.cpp Matrix.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c $< -o $@ $(LDFLAGS)

$(TARGET): Matrix_pybind.cpp Matrix.o Matrix.hpp
	$(CXX) $(CXXFLAGS) $(INCLUDE)   --shared `python3 -m pybind11 --includes` $^ -o _matrix`python3-config --extension-suffix` $(LDFLAGS)

test: $(TARGET)
	pytest test_Matrix.py

bench: $(TARGET)
	./benchmark.py --times 3

clean:
	rm -f *.o $(TARGET)
	rm -rf __pycache__ .pytest_cache
