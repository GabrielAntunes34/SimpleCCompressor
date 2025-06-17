#ifndef SAMPLER_H
    #define SAMPLER_H
    #include <stdio.h>
    #include <stdlib.h>

    #include "../primitives/matrix.h"
    #include "../primitives/pixel.h"
    #include "../primitives/util.h"

    // Funções para aplicar subamostragem na etapa de tratamento
    // das imagens
    int calculateSample420Padding(int dimension, int blkSize);
    DBMATRIX downSample420(DBMATRIX *channel, int blkSize);
    DBMATRIX upSample420(DBMATRIX *sampledData, int pdHeigth, int pdWidth);


#endif