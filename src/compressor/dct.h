#ifndef DCT_H
    #define DCT_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include <math.h>
    
    #include "../primitives/vector.h"
    #include "../primitives/util.h"

    #define BLK_SIZE 8      // Tamanho padrão dos plocos da DCT

    #define CROM_QNT_TBL 0  // Tabela de quantização para Croominancia
    #define LUM_QNT_TBL 1   // Tabela de quantização para Luminancia

    // Funções DCT
    void dct(int size, double blk[size][size], bool levelShift);
    void inverseDct(int size, double blk[size][size], bool levelShift);

    // Funções da quantização
    void quantize(int size, double blk[size][size], int newBlk[size][size], int table);
    void dequantize(int size, int blk[size][size], double newBlk[size][size], int table);
#endif