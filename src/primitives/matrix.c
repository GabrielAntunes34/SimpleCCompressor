#include "matrix.h"

//=================================================
// Funções para matrix double
//=================================================

// Cria uma matriz dinâmica de doubles
DBMATRIX dbMatrixCreate(int lines, int cols) {
    DBMATRIX mat;

    mat.lines = lines;
    mat.cols = cols;

    mat.matrix = (double **) malloc(lines * sizeof(double *));
    for(int i = 0; i < lines; i++) {
        mat.matrix[i] = (double *) malloc(cols * sizeof(double));
    }

    return mat;
}

// Limpa a memória de uma matriz dinâmica de doubles
void dbMatrixDestroy(DBMATRIX *mat) {
    if(mat == NULL)
        return;

    for(int i = 0; i < mat->lines; i++) {
        free(mat->matrix[i]);
    }
    free(mat->matrix);
    mat->matrix = NULL;
}

// Retorna a quantidade de blocos com o tamanho blkSize 
// existentes na matriz
int dbMatrixBlockQntd(DBMATRIX *mat, int blkSize) {
    if(mat == NULL || blkSize > mat->cols || blkSize > mat->lines)
        return -1;
    return (mat->cols * mat->lines) / (blkSize * blkSize);
}
void dbMatrixGetBlock(DBMATRIX *mat, int blkSize, int blkOffset, double blk[blkSize][blkSize]) {
    if(mat == NULL || blkSize > mat->cols || blkSize > mat->lines)
        return;

    // Auxiliares para os índices nos blocos
    int i = 0;
    int j;
    int bi = 0;     // linha do novo bloco
    int bj = 0;     // coluna do novo bloco
    int blkPerLine; // Quantidade de blocos por linha

    // Calculando a quantidade de blocos por linha
    blkPerLine = (mat->cols / blkSize);

    printf("%d blocks per line\n", blkPerLine);

    // Calculando os índices
    if(blkOffset > blkPerLine) {
        i = (blkOffset / (blkPerLine + 1)) * blkSize;
    }
    j = (blkOffset % blkPerLine) * blkSize;

    printf("i: %d\n", i);
    printf("j: %d\n", j);
    printf("i: %d\n", i + blkSize);
    printf("j: %d\n", j + blkSize);

    // Populando a matriz na stack com o novo bloco
    for(int k = 0; k < (blkSize * blkSize); k++) {
        if((k != 0) && (k % blkSize == 8)) {
            i++;
            bi++;
        } 

        blk[bi][bj] = mat->matrix[i][j];
        j++;
        bj++;
    }

    /*
    for(; i < (i + blkSize); i++) {
        for(; j < (j + blkSize); j++) {
            blk[bi][bj] = mat->matrix[i][j];
            bj++;
            printf("%lf, ", mat->matrix[i][j]);
        }
        printf("\n");
        bi++;
    }
    */
    return;
}

// Imprime uma matriz dinâmica de doubles
void dbMatrixPrint(DBMATRIX *mat) {
    for(int i = 0; i < mat->lines; i++) {
        for(int j = 0; j < mat->cols; j++) {
            printf("%.2lf, ", mat->matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}