#ifndef UTIL_H
    #define UTIL_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    // Prints de tipos primitivos para ED'S genéricas
    void intPrint(const void *val);
    void charPrint(const void *val);
    void floatPrint(const void *val);
    void doublePrint(const void *val);
    void boolPrint(const void *val);

#endif