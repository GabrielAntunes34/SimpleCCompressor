CFLAGS = -std=c99 -Wall

all: pair.o vector.o bitBuffer.o main.o
	gcc *.o $(CFLAGS) -o main

run: all
	./main


main.o: main.c
	gcc -c main.c

bitBuffer.o: bitBuffer.c
	gcc -c bitBuffer.c 

vector.o: vector.c
	gcc -c vector.c

pair.o: pair.c
	gcc -c pair.c

removeAllO:
	rm *.o

clean: removeAllO
	rm main