CXX = c++
CXXFLAGS= -O3 -Wall -shared -std=c++11 -fPIC
MODULE = Vector.*.so
TEST = test_angle.py

$(MODULE): Vector.cpp Vector.h
	@$(CXX) $(CXXFLAGS) `python3 -m pybind11 --includes ` $< -o Vector`python3-config --extension-suffix`

test: $(MODULE)
	@pytest $(TEST)

.PHONY: clean
clean:
	@rm -rf *so *cache* .*cache