#ifndef COMPRESSOR_H
    #define COMPRESSOR_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #include "fileManager.h"
    #include "cmpHeader.h"

    #include "../image/bfHeader.h"
    #include "../image/bmp.h"
    #include "../image/bfInfoHeader.h"
    #include "../image/pixel.h"
    
    #include "../primitives/util.h"
    #include "../primitives/bitBuffer.h"

    #include "../compression/sampler.h"
    #include "../compression/spacialEncoding.h"
    #include "../compression/huffman.h"
    #include "../compression/dct.h"

    //int calculateCompressTax();
    int compress(char *entryBmp, char *exitBin);
    bool decompress(char *entryBin, char *exitBmp);

#endif