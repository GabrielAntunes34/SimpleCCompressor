#include "jpegTests.h"

void testBitBuffer() {
    BITBUFFER *buffer;
    VECTOR *testData;
    unsigned int test[15] = {0, 1, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 1};
    
    // Populando os dados em um vector para a simulação
    testData = vectorCreateAs(unsigned char, NULL);
    for(int i = 0; i < 15; i++) {
        vectorPushBack(testData, &test[i]);
    }

    // Criando e populando o buffer
    buffer = bitBufferCreate(5);
    bitBufferInsert(buffer, testData);
    bitBufferPrint(buffer);

    FILE *pf = fopen("test2.bin", "wb");
    if(pf == NULL) {
        printf("Um erro ocorreu...\n");
        exit(1);
    }

    bitBufferWrite(buffer, pf);

    fclose(pf);
    bitBufferDestroy(&buffer);
    vectorDestroy(&testData);
    return;
}