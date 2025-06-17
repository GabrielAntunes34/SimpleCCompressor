#ifndef ENCODING_H
    #define ENCODING_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>


    #define BITBUFFER_ERROR -3200
    #define N 8 // Tamanho da matriz NxN
    #define EOB (-1)

    typedef int number; // Pode ser alterado para outro tipo de dado, se necessário

    typedef struct {
        int run;
        int category;
        const char *prefix;
        int total_length;
    } AC_Huffman_Code;

    number* zigZagNxN(number *matrix[N]);

    number** unZigZagNxN(number *zigZag);

    number* zigZagDifference(number *zigZag);

    number* unZigZagDifference(number *zigZag);

    RLEPairs createRLEPairs(int size);

    RLEPairs runLengthEncoding(number *zigZag, int *size);

    number* runLengthDecoding(RLEPairs rle);

    

#endif