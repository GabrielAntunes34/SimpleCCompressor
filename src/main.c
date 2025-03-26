#include <stdio.h>
#include <stdlib.h>
#include "bitBuffer.h"

int main() {
    FILE *pf;
    BITBUFFER *bf;
    unsigned char str[5] = "alfa";

    bf = bitBufferCreate(6);
    
    if(bitBufferIsEmpty(bf)) {
        bitBufferInsert(bf, str, 4, pf);
    }
    

    printf("oi 2\n");


    bitBufferPrint(bf);

    printf("oi 3\n");


    //bitBufferWrite(bf, pf);

    bitBufferDestroy(&bf);
    return 0;
}