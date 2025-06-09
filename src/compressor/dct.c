#include "dct.h"

//==========================
// INTERNAL TABLES
//==========================

// Matriz dos cossenos pré-calculada
const double C[8][8] = {
    {0.354, 0.354, 0.354, 0.354, 0.354, 0.354, 0.354, 0.354},
    {0.490, 0.416, 0.278, 0.098, -0.098, -0.278, -0.416, -0.490},
    {0.462, 0.191, -0.191, -0.462, -0.462, -0.191, 0.191, 0.462},
    {0.416, -0.098, -0.490, -0.278, 0.278, 0.490, 0.098, -0.416},
    {0.354, -0.354, -0.354, 0.354, 0.354, -0.354, -0.354, 0.354},
    {0.278, -0.490, 0.098, 0.416, -0.416, -0.098, 0.490, -0.278},
    {0.191, -0.462, 0.462, -0.191,-0.191, 0.462,   -0.462, 0.191},
    {0.098, -0.278, 0.416, -0.490, 0.490, -0.416,   0.278, -0.098}
};

// Matriz dos cossenos transposta pré-calculada
const double CT[8][8] = {
    {0.354, 0.490, 0.462, 0.416, 0.354, 0.278, 0.191, 0.098 }, 
    {0.354, 0.416, 0.191, -0.098, -0.354, -0.490, -0.462, -0.278}, 
    {0.354, 0.278, -0.191, -0.490, -0.354, 0.098, 0.462, 0.416}, 
    {0.354, 0.098, -0.462, -0.278, 0.354, 0.416, -0.191, -0.490}, 
    {0.354, -0.098, -0.462, 0.278, 0.354, -0.416, -0.191, 0.490}, 
    {0.354, -0.278, -0.191, 0.490, -0.354, -0.098, 0.462, -0.416}, 
    {0.354, -0.416, 0.191, 0.098, -0.354, 0.490, -0.462, 0.278}, 
    {0.354, -0.490, 0.462, -0.416, 0.354, -0.278, 0.191, -0.098} 
};

// Decompoes os pixeis de uma matriz ycbcr para vectors de cada canal
// A ordem dos dados em cada vector é a ordem dos blocos 8 por 8
void prepareBlocks(PIXELYCBCR ***mat, int width, int heigth, VECTOR *yBlocks, VECTOR *cbBlocks, VECTOR *crBlocks, bool levelShift) {
    if(mat == NULL)
        return;
   
    // Auxiliares para percorrer a matriz em blocos
    int colOffset = 0;
    int lineOffset = 0;
    double y, cb, cr;

    // Iterando pela matriz de pixeis
    while(lineOffset < heigth) {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {   
                // Obtendo os valores
                y = (*mat)[i + lineOffset][j + colOffset].y;
                cb = (*mat)[i + lineOffset][j + colOffset].cb;
                cr = (*mat)[i + lineOffset][j + colOffset].cr;

                // Executando o level shift para facilitar o cálculo da DCT
                if(levelShift) {
                    y -= 128;
                    cb -= 128;
                    cr -= 128;
                }

                // Populando os blocos
                vectorPushBack(yBlocks, &(*mat)[i + lineOffset][j + colOffset].y);
                if(cb != -1.0)
                    vectorPushBack(cbBlocks, &cb);
                if(cb != -1.0)
                    vectorPushBack(crBlocks, &cr);
            }
        }

        // Ajustando o offset para o próximo bloco
        colOffset += 8;
        if(colOffset >= width) {
            colOffset = 0;
            lineOffset += 8;
        }
    }
}

// Auxiliar que multiplica os blocos para realizar a DCT
void multiplyBlocks(double res[8][8], const double block[8][8], const double B[8][8]) {

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            res[i][j] = 0.0;
            for(int k = 0; k < 8; k++) {
                res[i][j] += block[i][k] * B[k][j];
            }
        }
    }
}

// Aplica a DCT em um bloco de valores dentro de um canal de luminancia ou
// Croominancia da imagem.
void dct(VECTOR *channel, int blockBg) {
    //DBMATRIX block;
    //block = dbMatrixCreate(8, 8);
    double block[8][8];
    double aux[8][8];
    double dct[8][8];

    // Passando os valores do bloco para a matriz
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            block[i][j] = vectorIndexAs(channel, double, blockBg + (j + i * 8));
        }
    }

    // Aplicando a fórmula DCT = C.B.CT
    multiplyBlocks(aux, C, block);
    multiplyBlocks(dct, aux, CT);

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            printf("%.2lf, ", dct[i][j]);
        }
        printf("\n");
    }
    printf("\n");

}

// Para reobter os blocos em ycbcr, fazemos a dct inversa aplicando: CT.B.C
// inverseDct(??)