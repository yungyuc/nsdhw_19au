# The executable program
TARGET   = Q1_main.exe
# The C++ source programs (.cc) [multiple files]
SOURCES  = Line Q1_main

O_DIR    = O_$(HOSTTYPE)
SYSDIR   = /usr/local
CLASSDIR = $(SYSDIR)/class/oop
BASEDIR  = .
INCDIR   = $(BASEDIR)/include #-I$(SYSDIR)/lib/g++-include
LIBDIR   = -L$(BASEDIR)/lib #-L/usr/openwin/lib -L/usr/ucblib
LIBS     = $(LIBDIR) -lm #-iosteam

# You may not need to modify anything below

CXX      = g++
CXXFLAGS = -g -Wno-deprecated -Wall -I$(INCDIR) #-D__STDC__
TARG     = $(TARGET)
LIB      = lib$(PACKAGE)_$(HOSTTYPE).a

SRCS     = ${SOURCES:%=%.cc}
OBJS     = ${SRCS:%.cc=$(O_DIR)/%.o}
HEADERS  = ${SRCS:%.cc=%.h}
TESTSRC  = ${TEST:%=%.cc}
TESTOBJ  = ${TESTSRC:%.cc=$(O_DIR)/%.o}


BINDTST      = Q2_test.py
BINDTAR      = vec2
BINDNAME     = Vec2Bind
BINDSRCS     = ${BINDNAME:%=%.cc}
BINDLIB      = $(BINDTAR)$(SUFFIX)
BINDALIAS    = $(BINDTAR).so


SRCS     = ${SOURCES:%=%.cc}
INCLUDE += $(shell python3.6-config --includes)
SUFFIX   = $(shell python3.6-config --extension-suffix)
LDFLAGS += $(shell python3.6-config --ldflags)
FLAGS    = -O3 -Wall -shared -std=c++11 -fPIC
BINDOBJ  = $(O_DIR)/$(BINDLIB)

all: $(TARG) $(BINDTAR)

$(TARG) : $(O_DIR) $(OBJS) $(TESTOBJ)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) $(TESTOBJ) $(LIBS)

$(BINDTAR) : $(O_DIR)
	$(CXX) $(FLAGS) $(INCLUDE) $(BINDSRCS) -o $(BINDOBJ) $(LDFLAGS)
	rm -rf $(BINDALIAS)
	ln -s $(BINDOBJ) $(BINDALIAS)

$(O_DIR) :
	mkdir $@

lib : $(O_DIR) $(LIB)
	@echo $@ Built

$(LIB) : $(OBJS)
	rm -f $(LIB)
	ar cr $@ $(OBJS)
	ranlib $@

$(O_DIR)/%.o: %.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(TESTOBJ) $(TARG) core* *~ Makefile.bak
	rm -rf $(O_DIR) $(BINDOBJ) $(BINDALIAS)

tags:
	etags $(SRCS) $(HEADERS)

ztar:
	/usr/local/bin/tar zcvf $(PACKAGE).tgz Makefile $(SRCS) $(HEADERS) $(TESTSRC)

ci:
	ci -l Makefile $(SRCS) $(HEADERS) $(TESTSRC)

run: $(TARG)
	@echo
	@echo "result of Q1:"
	@echo
	@./$(TARGET)

test: $(BINDTAR)
	@echo
	@echo "result of Q2:"
	@echo
	@chmod a+x $(BINDTST)
	@./$(BINDTST)
