question_1: question_1.o
	$(CXX) $^ -o $@

question_1.o: question_1.cpp
	$(CXX) -c $< -o $@

run:	
	g++ question_1.cpp
	./a.out

clean:
	rm -f question_1.o question_1 a.out
