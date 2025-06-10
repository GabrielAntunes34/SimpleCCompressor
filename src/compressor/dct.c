#include "dct.h"

//============================
// TABELAS INTERNAS DA DCT
//============================

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

// Matriz de quantização para o componente da luminância
const int QY[8][8] = {
    {16, 11, 10, 16, 24, 40, 51, 61},
    {12, 12, 14, 19, 26, 58, 60, 55},
    {14, 13, 16, 24, 40, 57, 69, 56},
    {14, 17, 22, 29, 51, 87, 80, 62},
    {18, 22, 37, 56, 68, 109, 103, 77},
    {24, 35, 55, 64, 81, 104, 113, 92},
    {79, 64, 78, 87, 103, 121, 120, 101},
    {72, 92, 95, 98, 112, 100, 103, 99}

};


// Matriz de quantização para os componentes de croôminancia
const int QC[8][8] = {
    {17, 18, 24, 47, 99, 99, 99, 99},
    {18, 21, 26, 66, 99, 99, 99, 99},
    {24, 26, 56, 99, 99, 99, 99, 99},
    {47, 66, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99},
    {99, 99, 99, 99, 99, 99, 99, 99}
};

//============================
// FUNÇÕES PARA APLICAR A DCT
//============================

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

// Aplica a DCT em um bloco de tamanho blkSize x blocSize
// Com os valores de um canal y, cb ou cr da imagem
void dct(int blkSize, double blk[blkSize][blkSize], bool levelShift) {
    double aux[8][8];

    // Aplicando o levelShift para otimizar a quantização, caso necessário
    if(levelShift) {
        for(int i = 0; i < blkSize; i++) {
            for(int j = 0; j < blkSize; j++) {
                blk[i][j] -= 128;
            }
        }
    }

    // Aplicando a fórmula DCT = C.B.CT
    multiplyBlocks(aux, C, blk);
    multiplyBlocks(blk, aux, CT);
}

// Para reobter os blocos em ycbcr, fazemos a dct inversa aplicando: CT.B.C
void inverseDct(int blkSize, double blk[blkSize][blkSize], bool levelShift) {
    double aux[8][8];

    // Aplicando a formula DCT = CT.B.C
    multiplyBlocks(aux, CT, blk);
    multiplyBlocks(blk, aux, C);

    // Revertendo o level shift, se necessário
    if(levelShift) {
        for(int i = 0; i < blkSize; i++) {
            for(int j = 0; j < blkSize; j++) {
                blk[i][j] += 128;
            }
        }
    }
}