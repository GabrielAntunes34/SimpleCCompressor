#include "sampler.h"

// Aplica a subamostragem 4:2:0 na matrix yCbCr por meio
// Da média dos valores em um bloco 2 x 2
DBMATRIX downSample420(DBMATRIX *channel) {
    DBMATRIX compressed;
    double average;
    int heigth = channel->lines;
    int width = channel->cols;

    // Instanciando a nova matriz dos dados subamostrados
    compressed = dbMatrixCreate((int) (heigth / 2), (int) (width / 2));

    // Andando pelos blocos 2x2 e substituindo pela média da
    // croominância
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
DBMATRIX upSample420(DBMATRIX *sampledData) {
    DBMATRIX decData;

    // Instanciando a matriz do tamanho da matrix original
    int width = sampledData->cols * 2;
    int heigth = sampledData->lines * 2;
    decData = dbMatrixCreate(heigth, width);

    // Iterando pela nova matriz e copiando os valores perdidos
    for(int i = 0; i < heigth; i++) {
        for(int j = 0; j < width; j++) {
            decData.matrix[i][j] = sampledData->matrix[i / 2][j / 2];
        }
    }

    return decData;
}