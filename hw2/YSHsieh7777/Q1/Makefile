CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra

EXEC = line

all: $(EXEC)

# $(EXEC): %.o
# 	$(CXX) $(CXXFLAGS) $^ -o $@	

line.o: line.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: run
run: $(EXEC)
	./$(EXEC)

.PHONY: clean
clean:
	rm -f *.o $(EXEC)