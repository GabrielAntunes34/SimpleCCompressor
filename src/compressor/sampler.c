#include "sampler.h"

// Calcula o padding necessário para uma dimensão do canal com downsample aplicado
int calculateSample420Padding(int dimension, int blkSize) {
    int sampledDim = dimension / 2;

    if(sampledDim % 8 != 0)
        return 8 - (sampledDim % 8);
    return 0;
}
/*
int calculateSamplePadding(DBMATRIX *channel, int *pdHeigth, int *pdWidth) {
    int newHeigth = channel->lines / 2;
    int newWidth = channel->cols / 2;

    printf("nH: %d\n", newHeigth);

    // Padding para a altura alcançar o próximo múltiplo de 8
    if(newHeigth % 8 != 0)
        *pdHeigth = 8 - (newHeigth % 8);
    else
        *pdHeigth = 0;

    // Padding para a largura alcançar o próximo múltiplo de 8
    if(newWidth % 8 != 0)
        *pdWidth = 8 - (newWidth % 8);
    else
        *pdWidth = 0;
}
*/

// Aplica a subamostragem 4:2:0 na matrix yCbCr por meio
// Da média dos valores em um bloco 2 x 2
DBMATRIX downSample420(DBMATRIX *channel, int blkSize) {
    DBMATRIX compressed;
    double average;
    int heigth = channel->lines;
    int width = channel->cols;

    // Calculando o padding, quando necessário
    int pdH = calculateSample420Padding(channel->lines, blkSize);
    int pdW = calculateSample420Padding(channel->cols, blkSize);

    // Instanciando a nova matriz dos dados subamostrados
    compressed = dbMatrixCreate((heigth / 2) + pdH, (width / 2) + pdW);

    // Andando pelos blocos 2x2 e substituindo pela média da croominância
    for(int i = 0; i < heigth; i += 2) {
        for(int j = 0; j < width; j += 2) {
            average = (channel->matrix[i][j] + channel->matrix[i][j+1] + 
                       channel->matrix[i+1][j] + channel->matrix[i+1][j+1]) / 4;

            compressed.matrix[i / 2][j / 2] = average;
        }
    }

    return compressed;
}

// Recria a matriz original a partir de um canal subamostrado
// Em 4:2:0
DBMATRIX upSample420(DBMATRIX *sampledData, int pdHeigth, int pdWidth) {
    DBMATRIX decData;

    // Instanciando a matriz do tamanho da matrix original
    int width = (sampledData->cols - pdWidth) * 2;
    int heigth = (sampledData->lines - pdHeigth) * 2;
    decData = dbMatrixCreate(heigth, width);

    // Iterando pela nova matriz e copiando os valores perdidos
    for(int i = 0; i < heigth; i++) {
        for(int j = 0; j < width; j++) {
            decData.matrix[i][j] = sampledData->matrix[i / 2][j / 2];
        }
    }

    return decData;
}