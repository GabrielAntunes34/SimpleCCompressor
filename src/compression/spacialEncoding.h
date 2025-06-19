#ifndef SPAENCODING_H
    #define SPAENCODING_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>

    #include "../fileManager/fileManager.h"

    #include "../image/pixel.h"
    #include "../image/bfHeader.h"
    #include "../image/bfInfoHeader.h"
    #include "../image/bmp.h"

    #include "../primitives/util.h"
    #include "../primitives/bitBuffer.h"
    #include "../primitives/vector.h"

    #include "../compression/sampler.h"
    #include "../compression/dct.h"

    #define BITBUFFER_ERROR -3200

    // Imlementa a porção espacial da codificação entrópica

    // Par de valores da codificação por carreira
    typedef struct rlepair_ {
        int run;
        int level;
    } rlepair;
    typedef rlepair *RLEPair;

    typedef struct rlepairs_ {
        RLEPair pairs;
        int size;
    } rlepairs;
    typedef rlepairs *RLEPairs;

    int* zigZagNxN(int blkSize, int matrix[blkSize][blkSize]);
    void unZigZagNxN(int blkSize, int blk[blkSize][blkSize], int *zigZag);

    int* zigZagDifference(int *zigZag);
    int* unZigZagDifference(int *zigZag);

    RLEPairs createRLEPairs(int size);
    void destroyRLEPairs(RLEPairs *pairs);
    RLEPairs runLengthEncoding(int *zigZag, int *size);
    int* runLengthDecoding(RLEPairs rle);

#endif