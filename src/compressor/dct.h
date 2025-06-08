#ifndef DCT_H
    #define DCT_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    
    #include "../primitives/vector.h"
    #include "../primitives/util.h"

    //typedef double dctBlock[8][8];
    typedef struct dctBlock {
        double mat[8][8];
    }dctBlock;
    
    void prepareBlocks(PIXELYCBCR ***mat, int width, int heigth, VECTOR *yBlocks, VECTOR *cbBlocks, VECTOR *crBlocks, bool LevelShift);
    void dctBlockPrint(dctBlock);
#endif