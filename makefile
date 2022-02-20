all: encode decode preprocess

decode: decode.o
	g++ -g -Wall decode.o -o decode

encode: encode.o
	g++ -g -Wall encode.o -o encode

preprocess: Preprocess.o
	g++ -g -Wall Preprocess.o -o preprocess

decode.o: decode.cpp defns.h
	g++ -g -Wall -c decode.cpp

encode.o: encode.cpp defns.h
	g++ -g -Wall -c encode.cpp

Preprocess.o: Preprocess.cpp defns.h
	g++ -g -Wall -c Preprocess.cpp

clean:
	rm *.o *.~
