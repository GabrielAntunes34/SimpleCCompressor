#ifndef DCT_H
    #define DCT_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    
    #include "../primitives/vector.h"
    #include "../primitives/util.h"

    #define BLK_SIZE 8

    void dct(int blkSize, double blk[blkSize][blkSize], bool levelShift);
    void inverseDct(int blkSize, double blk[blkSize][blkSize], bool levelShift);

#endif