#ifndef HUFFMAN_H
    #define HUFFMAN_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #include "../compression/spacialEncoding.h"
    #include "../primitives/vector.h"
    #include "../primitives/bitBuffer.h"

    // Implementa a codificação de huffman final da compressão

    // Tipo para a tabela de huffman
    typedef struct {
        int run;
        int category;
        char *prefix;
        int total_length;
    } AC_Huffman_Code;

    bool huffman_encoding(bitBuffer* buffer, RLEPairs rle);
    RLEPairs huffman_decoding(bitBuffer* buffer, int *pos);

#endif