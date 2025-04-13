#include <stdio.h>
#include <stdlib.h>
#include "bitBuffer.h"

int main() {
    FILE *pf;
    BITBUFFER *bf;
    unsigned int str[24] = {1, 1, 0, 1, 1, 1, 0, 0,
        1, 1, 0, 1, 1, 0, 1, 0,
        1, 1, 1, 0, 1, 0, 1, 1};

    bf = bitBufferCreate(2);

    pf = fopen("test.bin", "wb");
    if(pf == NULL)
        return -1;

    if(bitBufferIsEmpty(bf)) {
        bitBufferInsert(bf, str, 24, pf);
    }

    bitBufferPrint(bf);
    bitBufferWrite(bf, pf);

    fclose(pf);
    bitBufferDestroy(&bf);
    return 0;
}