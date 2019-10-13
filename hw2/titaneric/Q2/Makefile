.PHONY: clean

CXX = g++

angle: angle.o line.o
	$(CXX) $^ -o $@

angle.o: angle.cpp
	$(CXX) -c $< -o $@

line.o:	../Q1/line.cpp
	$(CXX) -c $< -o $@

run: angle
	./angle

clean:
	rm *.o ./angle