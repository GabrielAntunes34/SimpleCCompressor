#ifndef ENTROPY_TESTS_H
    #define ENTROPY_TESTS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #include "../primitives/util.h"
    #include "../image/pixel.h"
    #include "../image/bfHeader.h"
    #include "../image/bfInfoHeader.h"
    #include "../fileManager/fileManager.h"
    #include "../image/bmp.h"
    #include "../compression/spacialEncoding.h"
    #include "../compression/huffman.h"
    #include "../compression/sampler.h"
    #include "../compression/dct.h"

    int** generateRandomMatrix();
    void testEncodingDecoding();

#endif