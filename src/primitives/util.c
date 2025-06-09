#include "util.h"

//=================================================
// Structs para facilitar a manipulação de matrizes
//=================================================

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

void dbMatrixDestroy(DBMATRIX *mat) {
    if(mat == NULL)
        return;

    for(int i = 0; i < mat->lines; i++) {
        free(mat->matrix[i]);
    }
    free(mat->matrix);
    mat->matrix = NULL;
}

void dbMatrixPrint(DBMATRIX *mat) {
    for(int i = 0; i < mat->lines; i++) {
        for(int j = 0; j < mat->cols; j++) {
            printf("%.2lf, ", mat->matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

//=====================================
// Funções para tratamento de erros
//=====================================

// Mostra qualquer mensagem de erro em uma interfáce pré-definida
void displayError(char *errorStr) {
    printf("!=== ERROR ===!\n");
    printf("%s\n\n", errorStr);
}
  
//=====================================
// PRINTS INTERNOS PARA TIPOS BÁSICOS
//=====================================
void intPrint(const void *val) {
    int *ptr = (int *)val;
    printf("%d", *ptr);
}

void charPrint(const void *val) {
    char *ptr = (char *)val;
    printf("%c", *ptr);
}

void floatPrint(const void *val) {
    float *ptr = (float *)val;
    printf("%f", *ptr);
}

void doublePrint(const void *val) {
    double *ptr = (double *)val;
    printf("%lf", *ptr);
}

void boolPrint(const void *val) {
    bool *ptr = (bool *)val;
    if(*ptr)
        printf("True");
    printf("False");
}

