#include "util.h"

//////////////////////////////////////
// PRINTS INTERNOS PARA TIPOS BÁSICOS
//////////////////////////////////////
void intPrint(const void *val) {
    int *ptr = (int *)val;
    printf("%d ", *ptr);
}

void charPrint(const void *val) {
    char *ptr = (char *)val;
    printf("%c ", *ptr);
}

void printFloat(const void *val) {
    float *ptr = (float *)val;
    printf("%f ", *ptr);
}

void printDouble(const void *val) {
    double *ptr = (double *)val;
    printf("%lf ", *ptr);
}

