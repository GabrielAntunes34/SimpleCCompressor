#include <stdio.h>
#include <stdlib.h>

// Funções de teste
#include "test/bmpTests.h"
#include "test/compressTest.h"
#include "test/jpegTests.h"
#include "test/entropyTests.h"

#include "primitives/matrix.h"
#include "primitives/util.h"
#include "primitives/pixel.h"

#include "fileManager/bfHeader.h"
#include "fileManager/bfInfoHeader.h"
#include "fileManager/fileManager.h"
#include "fileManager/bmp.h"
#include "fileManager/compressor.h"


int main() {
    char *bmp1 = "img/testImg.bmp";
    char *bmp2 = "img/testImg2.bmp";
    char *bmp3 = "img/testImg3.bmp";
    char *bmp4 = "img/colors.bmp";
    char *testFile = "test.bmp";
    char *testFile2 = "decTest.bmp";

    //compress(bmp1, testFile);
    //decompress(testFile, testFile2);
    testEncodingDecoding();

    /*
    BITBUFFER *bf = bitBufferCreate(3);
    int vals[15] = {1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0};
    
    VECTOR *vec = vectorCreateAs(int, NULL);
    for(int i = 0; i < 8; i++) {
        vectorPushBack(vec, &vals[i]);
    }
    vectorPrintAs(vec, int);
    bitBufferInsert(bf, vec);
    bitBufferPrint(bf);

    VECTOR *vecTwo = vectorCreateAs(int, NULL);
    for(int i = 8; i < 15; i++) {
        vectorPushBack(vecTwo, &vals[i]);
        //printf("vals: %d, ", vals[i]);
    }
    //printf("\n");
    vectorPrintAs(vecTwo, int);
    bitBufferInsert(bf, vecTwo);
    bitBufferPrint(bf);

    bitBufferDestroy(&bf);
    */
    return 0;
}