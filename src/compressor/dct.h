#ifndef DCT_H
    #define DCT_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    
    #include "../primitives/vector.h"
    #include "../primitives/util.h"

    #define BLK_SIZE 8

    void prepareBlocks(PIXELYCBCR ***mat, int width, int heigth, VECTOR *yBlocks, VECTOR *cbBlocks, VECTOR *crBlocks, bool LevelShift);

    void dct(VECTOR *channel, int blockBg);

#endif