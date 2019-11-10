.PHONY: test

HEADER   = matrix.hpp

O_DIR    = obj

CXX      = g++
LDFLAGS  = -lm -llapacke -lcblas
CXXFLAGS = -O3 -std=c++11 -fPIC -g -Wno-deprecated -Wall #-DNOMKL

INCLUDE += -I${MKL_ROOT}/include
LDFLAGS += -L${MKL_LIB_DIR} -lpthread -ldl -m64

$(O_DIR) :
	mkdir $@

$(O_DIR)/%.o: %.cc
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c -o $@ $<  $(LDFLAGS)

INCLUDE += $(shell python3-config --includes)
SUFFIX   = $(shell python3-config --extension-suffix)
LDFLAGS += $(shell python3-config --ldflags)
BINDTAR      = _matrix
BINDNAME     = binding
BINDSRCS     = ${BINDNAME:%=%.cc}
BINDLIB      = $(BINDTAR)$(SUFFIX)
BINDALIAS    = $(BINDTAR).so
BINDOBJ  = $(O_DIR)/$(BINDLIB)

$(BINDOBJ): $(O_DIR)
	$(CXX) $(INCLUDE) $(CXXFLAGS) -shared -o $(BINDOBJ) $(BINDSRCS) $(LDFLAGS)
	ln -fs $(BINDOBJ) $(BINDALIAS)

bench: $(BINDOBJ) $(BINDSRCS) $(HEADER) benchmark.py
	python benchmark.py

test: $(BINDOBJ) $(HEADER)
	python -m pytest testing.py -v -s

clean:
	rm -f $(TARGET) $(BINDALIAS)
	rm -rf  $(O_DIR) $(BINDOBJ) $(BINDALIAS)

