#ifndef BITBUFFER_H
    #define BITBUFFER_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    typedef struct bitBuffer BITBUFFER;

    BITBUFFER *bitBufferCreate(int size);
    bool bitBufferIsEmpity(BITBUFFER *bitBuffer);

    bool bitBufferInsert(BITBUFFER *bitBuffer, unsigned char *code);
    bool bitBufferWrite(BITBUFFER *bitBuffer, FILE *pf);

    void bitBufferPrint(BITBUFFER *bitBuffer);
    bool bitBufferDestroy(BITBUFFER **bitBuffer);

#endif



/*
DEPENDENCIAS DESTE TAD:
--> bitBufferInsert:
    code deve ser uma string que recebeu o valos (int) 0 ou 1 (não (char) '0' ou '1')
    Decidimos que, caso o vetor acabe, vamos chamar o write e depois clean
    Depende de receber um ponteiro para arquivo aberto

--> bitBufferWrite:
    depende de receber um ponteiro para arquivo aberto

*/