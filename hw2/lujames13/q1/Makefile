
CC = g++

output: main.o Line.o
	$(CC) main.o Line.o -o output

main.o: main.cpp
	$(CC) -c main.cpp

Line.o: Line.cpp Line.h
	$(CC) -c Line.cpp

# check if output exist or not, and make output if it don't exist.
run: output
	./output

clean:
	rm -rf *.o output