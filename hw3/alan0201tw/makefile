# Compile Info
CC := g++
SRCEXT := cpp
CFLAGS := -O2 -std=c++17 -g -Wall -shared -fPIC
INCDIR = -I include `python3 -m pybind11 --includes`
LIB := -llapack -lblas -llapacke

# Source Info, target = cpplox, entry should be in cpplox.cpp
SRCDIR := src
BINDIR := bin
TARGET = _matrix`python3-config --extension-suffix`
SOURCES := $(shell find $(SRCDIR) -type f -iname "*.$(SRCEXT)" ! -iname "$(TARGET).$(SRCEXT)")
OBJECTS := $(patsubst $(SRCDIR)/%,$(BINDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Compile main
$(TARGET): $(OBJECTS)
	@echo "Linking..."
	@echo "$(CC) $^ $(CFLAGS) -o $(TARGET) $(LIB)"; $(CC) $^ $(CFLAGS) -o $(TARGET) $(LIB)
	export PRELOAD_MKL=True

$(BINDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BINDIR)
	@echo "$(CC) $(CFLAGS) $(INCDIR) -c -o $@ $<"; $(CC) $(CFLAGS) $(INCDIR) -c -o $@ $<

# Clean all binary files
clean:
	@echo " Cleaning..."; 
	@echo "$(RM) -r $(BINDIR) $(TARGET)"; $(RM) -r $(BINDIR) $(TARGET)
	@echo "$(RM) -r $(TESTBINDIR)"; $(RM) -r $(TESTBINDIR)

test: $(TARGET)
	@echo " Testing..."; 

# Declare clean as utility, not a file
.PHONY: clean test