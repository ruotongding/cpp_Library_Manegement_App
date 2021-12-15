all: prog

prog: test-suite.o library.o catch.o
	  g++ test-suite.o library.o catch.o -o prog

test-suite.o: test-suite.cpp catch.h library.h
	  g++ -c test-suite.cpp -o test-suite.o

library.o: library.cpp library.h
	  g++ -c library.cpp -o library.o

catch.o: catch.cpp catch.h
	  g++ -c catch.cpp -o catch.o

clean:
	  rm -rf *.o prog
