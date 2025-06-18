#ifndef ENTROPY_TESTS_H
    #define ENTROPY_TESTS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #include "../primitives/util.h"
    #include "../primitives/pixel.h"
    #include "../fileManager/bfHeader.h"
    #include "../fileManager/bfInfoHeader.h"
    #include "../fileManager/fileManager.h"
    #include "../fileManager/bmp.h"
    #include "../compressor/encoding.h"
    #include "../compressor/sampler.h"
    #include "../compressor/dct.h"

    int** generateRandomMatrix();
    void testEncodingDecoding();

#endif