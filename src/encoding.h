#ifndef ENCODING_H
    #define ENCODING_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #define BITBUFFER_ERROR -3200
    #define N 8 // Tamanho da matriz NxN

    typedef int number;

    void zigZagNxN(number *matrix[N], number zigZag[N * N]);

#endif