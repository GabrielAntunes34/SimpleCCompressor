#include "util.h"

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

