all:
	flex -o scanner.cpp scanner.l
	bison -o parser.cpp parser.y
	g++ -g  -std=c++11  main.cpp scanner.cpp parser.cpp command.cpp -o mycompiler
	chmod a+x mycompiler

run:
	./mycompiler "multiply();"

clean:
	rm -rf scanner.cpp
	rm -rf parser.cpp parser.hpp location.hh position.hh stack.hh
	rm -rf a.out
	rm -rf a.out.dSYM
	rm -rf mycompiler.dSYM
	rm -rf mycompiler