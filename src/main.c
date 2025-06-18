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

    compress(bmp1, testFile);

    return 0;
}