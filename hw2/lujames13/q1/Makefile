
CC = g++

CFLAGS = -c 


output: main.o
	$(CC) main.o -o output

main.o: main.cpp
	$(CC) -c main.cpp

# check if output exist or not, and make output if it don't exist.
run: output
	./output

clean:
	rm -rf *.o output