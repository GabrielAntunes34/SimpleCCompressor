CFLAGS = -std=c99 -Wall

all: bitBuffer.o main.o
	gcc *.o $(CFLAGS) -o main

run: all
	./main


main.o:
	gcc -c main.c

bitBuffer.o: bitBuffer.c
	gcc -c bitBuffer.c 

removeAllO:
	rm *.o

clean: removeAllO
	rm main