#ifndef BMP_H
    #define BMP_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    
    #include "bfHeader.h"
    #include "bfInfoHeader.h"
    #include "pixel.h"

    #include "../primitives/matrix.h"
    #include "../primitives/vector.h"
    #include "../primitives/util.h"

    typedef struct bmp BMP;
    
    BMP *bmpCreate(BIHEADER iHeader, BFHEADER fHeader, PIXELRGB** data);
    BMP *bmpRead(FILE *bmpPtr);
    bool bmpWrite(BMP *image, FILE *bmpPtr);

    void bmpGetYcbcrChannels(BMP *image, DBMATRIX *y, DBMATRIX *cb, DBMATRIX *cr);
    PIXELYCBCR **bmpGetYcbcrData(BMP *image);
    PIXELRGB **bmpGetRgbData(BMP *image);
    int bmpGetWidth(BMP *image);
    int bmpGetHeigth(BMP *image);
    BIHEADER bmpGetInfoHeader(BMP *image);
    BFHEADER bmpGetFileHeader(BMP *image);

    bool bmpSetPixel(BMP *image, int i, int j, pixelRgb newPx);

    void bmpPrint(BMP *image, bool printHeaders, bool printData);
    void bmpDestroy(BMP **image);

#endif