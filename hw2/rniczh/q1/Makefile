CXX = g++

TARGET = bin/q1
SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.cc)
OBJ = $(SRC:$(SRC_DIR)/%.cc=$(OBJ_DIR)/%.o)

CPPFLAGS += -Iinclude
CFLAGS += -O3 -Wall
LDFLAGS += -lm

.PHONY: all clean

all: $(SRC) $(TARGET)

# link each object into target file
$(TARGET): $(OBJ)
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

# compile each file into object file
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc
	$(CXX) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

run: all
	@./$(TARGET)

clean:
	$(RM) $(OBJ)
	$(RM) $(TARGET)
