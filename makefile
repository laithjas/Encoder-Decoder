all: encode preprocess

encode: encode.o
	g++ -g -Wall encode.o -o encode

preprocess: Preprocess.o
	g++ -g -Wall Preprocess.o -o preprocess

Preprocess.o: Preprocess.cpp defns.h
	g++ -g -Wall -c Preprocess.cpp

encode.o: encode.cpp defns.h
	g++ -g -Wall -c encode.cpp

clean:
	rm *.o *.~