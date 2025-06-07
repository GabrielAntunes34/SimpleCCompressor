#ifndef BMP_H
    #define BMP_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "bfHeader.h"
    #include "bfInfoHeader.h"

    #include "../primitives/vector.h"
    #include "../primitives/pixel.h"
    #include "../primitives/util.h"

    typedef struct bmp BMP;
    
    BMP *bmpRead(FILE *bmpPtr);
    bool bmpWrite(BMP *image, FILE *bmpPtr);

    PIXELYCBCR **bmpGetYcbcrData(BMP *image);
    PIXELRGB **bmpGetRgbData(BMP *image);

    BIHEADER bmpGetInfoHeader(BMP *image);
    BFHEADER bmpGetFileHeader(BMP *image);

    void bmpPrint(BMP *image, bool printHeaders, bool printData);
    void bmpDestroy(BMP **image);

#endif