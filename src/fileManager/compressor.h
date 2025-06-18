#ifndef COMPRESSOR_H
    #define COMPRESSOR_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #include "fileManager.h"
    #include "bfHeader.h"
    #include "bfInfoHeader.h"
    #include "cmpHeader.h"
    #include "bmp.h"
    
    #include "../primitives/util.h"
    #include "../primitives/pixel.h"
    #include "../primitives/bitBuffer.h"

    #include "../compressor/sampler.h"
    #include "../compressor/encoding.h"
    #include "../compressor/dct.h"

    //int calculateCompressTax();
    bool compress(char *entryBmp, char *exitBin);
    bool decompress(char *entryBin, char *exitBmp);

#endif