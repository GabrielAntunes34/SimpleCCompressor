#pragma once
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "util.h"
    #include "pixel.h"

    // Macros para manter o uso "typesafe"
    // E abstrair o uso dos ponteiros genéricos
    #define VECTOR_ERROR -3201
    #define vectorCreateAs(type, elemDestroy) vector_create(sizeof(type), elemDestroy)
    #define vectorIndexAs(vec, type, index) (*(type *)vector_index(vec, index))
    #define vectorPrintAs(vec, type) vector_print(vec, type##Print)


    // Vector é um array genérico e memory safe.
    typedef struct vector VECTOR;
    typedef struct vector vector;


    VECTOR *vector_create(size_t elemSize, void (*elemDestroy)(void **));

    void *vector_index(VECTOR *vec, size_t index);
    size_t vectorGetSize(VECTOR *vec);
    size_t vectorGetCapacity(VECTOR *vec);

    bool vectorPushBack(VECTOR *vec, const void *elem);
    bool vectorSetIndex(VECTOR *vec, size_t index, const void *elem);

    void vector_print(VECTOR *vec, void (printElem)(const void*));
    void vectorDestroy(VECTOR **vec);



/*
--- UTILIZAÇÃO DO VECTOR GENÉRICO ---
--> Funciona tanto com tipos primitivos quanto TAD'S encapsulados com ponteiro opaco
--> algumas macros foram definidas para facilitar a utilização de funções em que
o tipo interno do vector precisa ser expresso
--> Utiliza memcpy internamente para copiar os valores a serem
atribuídos nas posições de memória do array
--> Para TAD's, armazena um ponteiro para a função que os desaloca

--> Uso com tipos primitivos:
- Basta ter uma função: void tipoPrint(const void *val);
- internamente, este argumento deve receber um casting para
um ponterio do tipo
- Ao instanciá-lo, deve-se passar NULL para o argumento da função
"destroy"

--> Interface para usá-lo com um TAD:
- Deve ser encapsulado com ponterio opaco
- Assim, vector guardará o endereço do ponteiro para o TAD
- deve conter no .h as sequintes definições:
    - typedef struct nomeDoTad nomeDoTad;
    - void nomeDoTadPrint(const void *tad);
    - void nomeDoTadDestroy(void ** tad);
- Em ambas funções, damos um casting para (TAD *) ao receber
o argumento void *
- Ao instanciá-lo, deve-se passar a função tadDestroy como argumento.

*/