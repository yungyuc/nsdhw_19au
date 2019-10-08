# The executable program
TARGET   = main
# The C++ source programs (.cc) [multiple files]
SOURCES  = Line main

INCLUDE+= $(shell pkg-config --cflags --libs python3)

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

$(TARG) : $(O_DIR) $(OBJS) $(TESTOBJ)
	$(CXX) -o $@ $(CXXFLAGS) $(OBJS) $(TESTOBJ) $(LIBS)
	g++ -O3 -Wall -shared -std=c++11 -fPIC `python3 -m vector --includes` vector.cc -o vector`python3-config --extension-suffix` `python3-config --ldflags`


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
	rm -rf $(O_DIR)

tags:
	etags $(SRCS) $(HEADERS)

ztar:
	/usr/local/bin/tar zcvf $(PACKAGE).tgz Makefile $(SRCS) $(HEADERS) $(TESTSRC)

ci:
	ci -l Makefile $(SRCS) $(HEADERS) $(TESTSRC)

run:
	make
	./$(TARGET)
