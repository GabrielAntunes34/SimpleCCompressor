#include <stdio.h>
#include <stdlib.h>

// Test functions
#include "test/bmpTests.h"
#include "test/compressTest.h"

#include "primitives/matrix.h"
#include "primitives/util.h"
#include "primitives/pixel.h"

#include "fileManager/bfHeader.h"
#include "fileManager/bfInfoHeader.h"
#include "fileManager/fileManager.h"
#include "fileManager/bmp.h"


int main() {
    char *bmp1 = "img/testImg.bmp";
    char *bmp2 = "img/testImg2.bmp";
    char *bmp3 = "img/colors.bmp";
    char *testFile = "test.bmp";

    //testSampler(bmp3, testFile);

    double blk[8][8] = {
        {255.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
    };

    dct(8, blk, true);
    inverseDct(8, blk, true);

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            printf("%.2lf, ", blk[i][j]);
        }
        printf("\n");
    }

    return 0;
}