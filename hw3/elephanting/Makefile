CXX = g++
CXXFLAGS=-g -std=c++11 -Wall -pedantic
EXE = main
OUT = output

$(EXE): main.o Matrix.o
	@$(CXX) $(CXXFLAGS) $^ -o $@

main.o: main.cpp Matrix.h
	@$(CXX) -c $(CXXFLAGS) $< -o $@

Matrix.o: Matrix.cpp Matrix.h
	@$(CXX) -c $(CXXFLAGS) $< -o $@

.PHONY: clean
clean:
	@rm -f *o $(EXE) $(OUT)

run: $(EXE)
	@./$(EXE)