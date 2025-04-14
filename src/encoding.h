#ifndef ENCODING_H
    #define ENCODING_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>


    #define BITBUFFER_ERROR -3200
    #define N 8 // Tamanho da matriz NxN
    #define EOB (-1)

    typedef int number;

    number* zigZagNxN(number *matrix[N]);

#endif