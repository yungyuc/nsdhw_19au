.PHONY: test

HEADER   = matrix.hpp

O_DIR    = obj

CXX      = g++
LDFLAGS  = -lm -llapacke -lcblas
CXXFLAGS = -O3 -std=c++11 -fPIC -g -Wno-deprecated -Wall #-DNOMKL

MKLROOT = /opt/intel/mkl
INCLUDE += -I${MKLROOT}/include
LDFLAGS += -L${MKLROOT}/lib -lpthread -ldl -m64

$(O_DIR) :
	mkdir $@

$(O_DIR)/%.o: %.cc
	$(CXX) $(INCLUDE) $(CXXFLAGS) -c -o $@ $<  $(LDFLAGS)

INCLUDE += $(shell python-config --includes)
SUFFIX   = $(shell python-config --extension-suffix)
LDFLAGS += $(shell python-config --ldflags)
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

test: $(BINDOBJ)
	python -m pytest testing.py -v -s

clean:
	rm -f $(TARGET) $(BINDALIAS)
	rm -rf  $(O_DIR) $(BINDOBJ) $(BINDALIAS)

