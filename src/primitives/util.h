#ifndef UTIL_H
    #define UTIL_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #define min(A, B) ((A < B) ? A : B)
    #define max(A, B) ((A > B) ? A : B) 

    struct dbMatrix {
        double ** matrix;
        int lines;
        int cols;
    };
    typedef struct dbMatrix DBMATRIX;

    // Funções para gerenecimanto de memória de matrizes
    DBMATRIX dbMatrixCreate(int lines, int cols);
    void dbMatrixDestroy(DBMATRIX *mat);
    void dbMatrixPrint(DBMATRIX *mat);

    void displayError(char *errorStr);

    // Prints de tipos primitivos para ED'S genéricas
    void intPrint(const void *val);
    void charPrint(const void *val);
    void floatPrint(const void *val);
    void doublePrint(const void *val);
    void boolPrint(const void *val);

#endif