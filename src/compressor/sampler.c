#include "sampler.h"

// Aplica a subamostragem 4:2:0 na matrix yCbCr por meio
// Da média dos valores em um bloco 2 x 2
void downSample420(PIXELYCBCR ***mat, int width, int heigth) {
    if(mat == NULL)
        return;

    double average;

    // Andando pelos blocos 2x2 e substituindo pela média da
    // croominância
    for(int i = 0; i < heigth; i += 2) {
        for(int j = 0; j < width; j += 2) {
            average = ((*mat)[i][j].cb + (*mat)[i][j+1].cb + (*mat)[i+1][j].cb + (*mat)[i+1][j+1].cb) / 4;
            (*mat)[i][j].cb = average;
            (*mat)[i][j+1].cb = -1.0;
            (*mat)[i+1][j].cb = -1.0;
            (*mat)[i+1][j+1].cb = -1.0;

            average = ((*mat)[i][j].cr + (*mat)[i][j+1].cr + (*mat)[i+1][j].cr + (*mat)[i+1][j+1].cr) / 4;
            (*mat)[i][j].cr = average;
            (*mat)[i][j+1].cr = -1.0;
            (*mat)[i+1][j].cr = -1.0;
            (*mat)[i+1][j+1].cr = -1.0;
        }
    }
}


// Realiza o upsampling pela média dos vizinhos não descartados em cada ploco 2 por 2
void upSample420(PIXELYCBCR ***mat, int width, int heigth) {
    if(mat == NULL)
        return;
    
    // Copiando os pixeis cb e cr para as regioes de informação perdida
    for(int i = 0; i < heigth - 1; i += 2) {
        for(int j = 0; j < width - 1; j += 2 ) {
            (*mat)[i+1][j].cb = (*mat)[i][j].cb;
            (*mat)[i][j+1].cb = (*mat)[i][j].cb;
            (*mat)[i+1][j+1].cb = (*mat)[i][j].cb;

            (*mat)[i+1][j].cr = (*mat)[i][j].cr;
            (*mat)[i][j+1].cr = (*mat)[i][j].cr;
            (*mat)[i+1][j+1].cr = (*mat)[i][j].cr;
        }
    }
}