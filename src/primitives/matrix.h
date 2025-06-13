#ifndef MATRIX_H
    #define MATRIX_H
    #include <stdio.h>
    #include <stdbool.h>
    #include <stdlib.h>

    // Matriz dinâmica de doubles
    struct dbMatrix {
        double ** matrix;
        int lines;
        int cols;
    };
    typedef struct dbMatrix DBMATRIX;
    typedef struct dbMatrix dbMatrix;

    // Funções para gerenecimanto de memória de matrizes
    DBMATRIX dbMatrixCreate(int lines, int cols);
    void dbMatrixDestroy(DBMATRIX *mat);

    // Funções para a obtenção de blocos de matrizes
    int dbMatrixGetBlockQntd(DBMATRIX *mat, int blkSize);
    void dbMatrixGetBlock(DBMATRIX *mat, int blkSize, int blkOffset, double blk[blkSize][blkSize]);
    void dbMatrixSetBlock(DBMATRIX *mat, int blkSize, int blkOffset, double blk[blkSize][blkSize]);

    // Funções de IO para matrizes
    void dbMatrixPrint(DBMATRIX *mat);

#endif