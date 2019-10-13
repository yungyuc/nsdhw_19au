.PHONY: clean

CXX = g++

build:
	c++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` angle.cpp -o angle`python3-config --extension-suffix`

clean:
	rm *.o ./angle