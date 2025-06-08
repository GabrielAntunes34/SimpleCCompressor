#include <stdio.h>
#include <stdlib.h>

// Test functions
#include "test/bmpTests.h"
#include "test/compressTest.h"

#include "primitives/util.h"
#include "primitives/pixel.h"

#include "fileManager/bfHeader.h"
#include "fileManager/bfInfoHeader.h"
#include "fileManager/fileManager.h"
#include "fileManager/bmp.h"

#define TESTFILE "test.bmp"

int main() {
    char *bmp1 = "img/testImg.bmp";
    char *bmp2 = "img/testImg2.bmp";
    char *bmp3 = "img/colors.bmp";
    char *testFile = "test.bmp";


    testBlocks(bmp3);
    return 0;
}