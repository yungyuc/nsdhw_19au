CXX := g++
CXXFLAGS ?= -std=c++17 -O3 -Wall
SRC = matrix.cpp
TARGET = _matrix.cpython-37m-x86_64-linux-gnu.so

# MKL
MKLROOT ?= ${HOME}/opt/conda
INCLUDES += -m64 -I${MKLROOT}/include
LDFLAGS += -L${MKLROOT}/lib -Wl,--no-as-needed \
	-lmkl_rt -lpthread -lm -ldl

# pybind11
INCLUDES += `python -m pybind11 --includes`
CXXFLAGS += -shared -fPIC

.PHONY: all test format clean
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $(TARGET) $(LDFLAGS)

format: $(SRC)
	clang-format -i $(SRC)

test: $(TARGET)
	../validate.py

clean:
	rm -rf $(TARGET)

