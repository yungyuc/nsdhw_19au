all:  question_2.cpp
	$(CXX) -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` question_2.cpp -o question_2`python3-config --extension-suffix`

test:
	python3 question_2_test.py

clean:
	rm -rf question_2`python3-config --extension-suffix` __pycache__
