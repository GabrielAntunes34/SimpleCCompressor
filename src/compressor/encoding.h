#ifndef ENCODING_H
    #define ENCODING_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <string.h>

    #include "../primitives/util.h"
    #include "../primitives/pixel.h"
    #include "../fileManager/bfHeader.h"
    #include "../fileManager/bfInfoHeader.h"
    #include "../fileManager/fileManager.h"
    #include "../fileManager/bmp.h"
    #include "../primitives/bitBuffer.h"
    #include "../primitives/vector.h"
    #include "../compressor/sampler.h"
    #include "../compressor/dct.h"
    #define BITBUFFER_ERROR -3200

    typedef int number; // Pode ser alterado para outro tipo de dado, se necessário

    typedef struct {
        int run;
        int category;
        char *prefix;
        int total_length;
    } AC_Huffman_Code;

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

    number* zigZagNxN(int blkSize, int matrix[blkSize][blkSize]);

    void unZigZagNxN(int blkSize, int blk[blkSize][blkSize], number *zigZag);

    number* zigZagDifference(number *zigZag);

    number* unZigZagDifference(number *zigZag);

    RLEPairs createRLEPairs(int size);

    RLEPairs runLengthEncoding(number *zigZag, int *size);

    number* runLengthDecoding(RLEPairs rle);

    bool huffman_encoding(bitBuffer* buffer, RLEPairs rle);

    RLEPairs huffman_decoding(bitBuffer* buffer);

#endif