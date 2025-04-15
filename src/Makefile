CFLAGS = -std=c99 -Wall
primPath = primitives/
compPath = compressor/
fPath = fileManager/

all: util.o vector.o bitBuffer.o bfInfoHeader.o bfHeader.o main.o
	gcc *.o $(CFLAGS) -o main

run: all
	./main


main.o: main.c
	gcc -c main.c

# Compilando o código de fileManager
bfHeader.o: $(fPath)bfHeader.c
	gcc -c $(fPath)bfHeader.c

bfInfoHeader.o: $(fPath)bfInfoHeader.c
	gcc -c $(fPath)bfInfoHeader.c

# Compilando o código em compressor


# Compilando o código de primitives
bitBuffer.o: $(primPath)bitBuffer.c
	gcc -c $(primPath)bitBuffer.c 

vector.o: $(primPath)vector.c
	gcc -c $(primPath)vector.c

util.o: $(primPath)util.c
	gcc -c $(primPath)util.c

removeAllO:
	rm *.o

clean: removeAllO
	rm main