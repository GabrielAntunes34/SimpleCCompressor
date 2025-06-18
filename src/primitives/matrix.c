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
int dbMatrixGetBlockQntd(DBMATRIX *mat, int blkSize) {
    if(mat == NULL || blkSize > mat->cols || blkSize > mat->lines)
        return -1;
    return (mat->cols * mat->lines) / (blkSize * blkSize);
}

// Auxiliar para calcular os índices de um bloco na matriz dado
// o seu offset
void calcBLockPosition(DBMATRIX *mat, int blkSize, int blkOffset, int *i, int *j) {
    int blkPerLine;

    // Obtendo a quantidade de blocos por linha
    blkPerLine = (mat->cols / blkSize);

    // Calculando os índices i e j do bloco
    if(blkOffset >= blkPerLine)
        *i = (blkOffset / (blkPerLine)) * blkSize;
    *j = (blkOffset % blkPerLine) * blkSize;
}

// Retorna em uma matriz na stack da função que a chamou, um bloco da
// DBMATRIX
void dbMatrixGetBlock(DBMATRIX *mat, int blkSize, int blkOffset, double blk[blkSize][blkSize]) {
    if(mat == NULL || blkSize > mat->cols || blkSize > mat->lines)
        return;

    // Auxiliares para os índices nos blocos
    int i = 0;
    int j = 0;
    int bi = 0;     // linha do novo bloco
    int bj = 0;     // coluna do novo bloco

    // Calculando os índices do bloco
    calcBLockPosition(mat, blkSize, blkOffset, &i, &j);

    // Populando a matriz na stack com o novo bloco
    for(int k = 0; k < (blkSize * blkSize); k++) {
        if((k != 0) && (k % blkSize == 0)) {
            // Atualizando a linha
            i++;
            bi++;

            // Resetando o íncide para a coluna
            j -= blkSize;
            bj -= blkSize;
        } 

        blk[bi][bj] = mat->matrix[i][j];
        j++;
        bj++;
    }

    return;
}

void dbMatrixSetBlock(DBMATRIX *mat, int blkSize, int blkOffset, double blk[blkSize][blkSize]) {
    if(mat == NULL || blkSize > mat->cols || blkSize > mat->lines)
        return;

    // Auxiliares para os índices nos blocos
    int i = 0;
    int j = 0;
    int bi = 0;     // linha do novo bloco
    int bj = 0;     // coluna do novo bloco

    // Calculando os índices do bloco
    calcBLockPosition(mat, blkSize, blkOffset, &i, &j);

    // Populando o bloco da DBMATRIX com o novo dado na stack
    for(int k = 0; k < (blkSize * blkSize); k++) {
        if((k != 0) && (k % blkSize == 0)) {
            // Atualizando a linha
            i++;
            bi++;

            // Resetando o índice para a coluna
            j -= blkSize;
            bj -= blkSize;
        } 

        mat->matrix[i][j] = blk[bi][bj];
        j++;
        bj++;
    }
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