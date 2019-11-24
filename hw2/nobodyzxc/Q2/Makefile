O_DIR    = O_$(HOSTTYPE)
LIBS     = -lm

CXX      = g++
CXXFLAGS = -g -Wno-deprecated -Wall

BINDTST      = Q2_test.py
BINDTAR      = vec2
BINDNAME     = Vec2Bind
BINDSRCS     = ${BINDNAME:%=%.cc}
BINDLIB      = $(BINDTAR)$(SUFFIX)
BINDALIAS    = $(BINDTAR).so

SRCS     = ${SOURCES:%=%.cc}
INCLUDE += $(shell python3.7-config --includes)
SUFFIX   = $(shell python3.7-config --extension-suffix)
LDFLAGS += $(shell python3.7-config --ldflags)
FLAGS    = -O3 -Wall -shared -std=c++11 -fPIC -fno-lto
BINDOBJ  = $(O_DIR)/$(BINDLIB)

$(BINDOBJ) : $(O_DIR)
	$(CXX) $(FLAGS) $(INCLUDE) $(BINDSRCS) -o $(BINDOBJ) $(LDFLAGS)
	rm -rf $(BINDALIAS)
	ln -s $(BINDOBJ) $(BINDALIAS)

$(O_DIR) :
	mkdir $@

$(O_DIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf $(O_DIR) $(BINDOBJ) $(BINDALIAS)

test: $(BINDOBJ)
	@chmod a+x $(BINDTST)
	@python3 $(BINDTST)
