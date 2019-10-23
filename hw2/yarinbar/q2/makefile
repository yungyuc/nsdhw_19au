
CC=g++
FLAGS=-Wall -std=c++11 -shared -Werror -fPIC -pedantic-errors `python3 -m pybind11 --includes`
OBJS=q2.cpp

all:
	${CC} ${FLAGS} ${OBJS} -o q2`python3-config --extension-suffix`


test:
	python3 q2_test.py


clean:
	echo Cleaning
	rm -f q2.o
	rm -f q2`python3-config --extension-suffix` __pycache__
	${CC} ${FLAGS} ${OBJS} -o q2`python3-config --extension-suffix`

