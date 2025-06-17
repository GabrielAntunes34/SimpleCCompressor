#ifndef BITBUFFER_H
    #define BITBUFFER_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #include "vector.h"

    #define BITBUFFER_ERROR -3200

    // Buffer de unsigned chars para guardar os bits do código comprimido
    typedef struct bitBuffer BITBUFFER;
    typedef struct bitBuffer bitBuffer;

    BITBUFFER *bitBufferCreate(int size);

    bool bitBufferIsEmpty(BITBUFFER *bitBuffer);

    int bitBufferGetByteSize(BITBUFFER *bitBuffer);
    bool bitBufferInsert(BITBUFFER *bitBuffer, VECTOR *code);
    bool bitBufferClean(BITBUFFER *bitBuffer);
    bool bitBufferWrite(BITBUFFER *bitBuffer, FILE *pf);
    bool bitBufferReadBit(BITBUFFER *bitBuffer, int position);
    int bitBufferGetOccupiedBits(BITBUFFER *bitBuffer);


    void bitBufferPrint(const void *bitBuffer);
    bool bitBufferDestroy(BITBUFFER **bitBuffer);

#endif



/*
DEPENDENCIAS DESTE TAD:
--> bitBufferInsert:
    code deve ser uma string que recebeu o valor (int) 0 ou 1 (não (char) '0' ou '1')

--> bitBufferWrite:
    depende de receber um ponteiro para arquivo ABERTO
*/