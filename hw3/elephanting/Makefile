# MKL
# https://software.intel.com/en-us/articles/intel-mkl-link-line-advisor/

MKLROOT = /home/ryan/intel/mkl

MKLLINKLINE := \
	-Wl,--start-group \
	${MKLROOT}/lib/intel64/libmkl_intel_ilp64.a \
	${MKLROOT}/lib/intel64/libmkl_sequential.a \
	${MKLROOT}/lib/intel64/libmkl_core.a \
	-Wl,--end-group \
	-lpthread -lm -ldl

CXX = g++

CXXFLAGS := -std=c++17 -O3 -g -m64 -DMKL_ILP64

out: main.o Matrix.o
	${CXX} $^ ${MKLLINKLINE} -I${MKLROOT}/include ${CXXFLAGS} -o $@

main.o: main.cpp Matrix.h
	${CXX} -c $< ${CXXFLAGS} -o $@

Matrix.o: Matrix.cpp Matrix.h
	${CXX} -c $< ${CXXFLAGS} -o $@

clean:
	rm -f *.o
