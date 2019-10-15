main:
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m pybind11 --includes` vector.cpp -o vector`python3-config --extension-suffix`

clean:
	@echo " Cleaning...";
	@echo "$(RM) *.so"; $(RM) *.so

run:
	nosetests -v -s pybind_test.py

.PHONY: clean run