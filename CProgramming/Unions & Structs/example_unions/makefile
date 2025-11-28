all: usingUnions

usingUnions: usingUnions.o message.o
	gcc -o usingUnions usingUnions.o message.o

usingUnions.o: usingUnions.c message.h
	gcc -c usingUnions.c -o usingUnions.o

message.o: message.c message.h
	gcc -c message.c -o message.o

rebuild: clean all

clean:
	-rm -f usingUnions usingUnions.o message.o
