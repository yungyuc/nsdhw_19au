all: coord.cpp
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python -m pybind11 --includes` coord.cpp -o coord`python-config --extension-suffix`

.PHONY: test
test: test_angle_2d.py
	python -m unittest $<

.PHONY: clean
clean:
	rm -rf coord`python-config --extension-suffix` __pycache__
