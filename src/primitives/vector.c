#include <string.h>
#include "vector.h"

#define VECTOR_INITAL_CAPACITY 4


struct vector {
    void *data;         // Ponteiro para o array de dados genéricos
    size_t elemSize;    // Tamanho em bytes do tipo de dado usado
    size_t size;        // Quantidade de elementos ocupando vector
    size_t capacity;    // Quantidade de espaço alocado (medido em quantidade de elementos)

    void (*elemDestroy)(void **);    // Função opcional para desalocar a memória do elemento interno
};


//////////////////////////////////////
// Funções do TAD vector
//////////////////////////////////////

// Cria um vector a partir do tamanho em bytes do tipo de dado que ele armazenará.
VECTOR *vector_create(size_t elemSize, void (*elemDestroy)(void **)) {
    VECTOR *vec;

    vec = (VECTOR *) malloc(sizeof(VECTOR));
    if(vec != NULL) {
        // Alocando o array interno
        vec->data = malloc(VECTOR_INITAL_CAPACITY * elemSize);
        if(vec->data == NULL) {
            free(vec);
            vec = NULL;
            return NULL;
        }

        // Inicializando as porpiedades do vector
        vec->elemSize = elemSize;
        vec->size = 0;
        vec->capacity = VECTOR_INITAL_CAPACITY;
        vec->elemDestroy = elemDestroy;
    }

    return vec;
}

// Função que retorna um ponteiro genérico para o index expresso.
// Precisa de um casting!
void* vector_index(VECTOR *vec, size_t index) {
    if(vec == NULL || index >= vec->size)
        return NULL;

    // Usa-se (char *) para o compilador entender que acessarmos ao menos um
    // byte, dado que ele não faz aritimética de ponteiro com void *
    return (char *) vec->data + (index * vec->elemSize);
}

// Retorna a quantidade de posições preenchidas no vector
size_t vectorGetSize(VECTOR *vec) {
    return ((vec == NULL) ? VECTOR_ERROR : vec->size);
}

// Retorna a quantidade de elementos 
size_t vectorGetCapacity(VECTOR *vec) {
    return ((vec == NULL) ? VECTOR_ERROR : vec->capacity);
}

// Insere um novo elemento ao fim do vetor
bool vectorPushBack(VECTOR *vec, const void *elem) {
    if(vec == NULL)
        return NULL;

    // Verificando se é necessário realocar o espaço de vector
    if(vec->size == vec->capacity) {
        vec->capacity *= 2;
        vec->data = realloc(vec->data, vec->elemSize * vec->capacity);
    }

    // Copiando para a posição certa no vector o valor do endereço de elem
    memcpy((char *) vec->data + (vec->size * vec->elemSize), elem, vec->elemSize);
    vec->size++;
    return true;
}

// Altera o valor de um index já previamente preenchido no vetor
bool vectorSetIndex(VECTOR *vec, size_t index, const void *elem) {
    if(vec == NULL || index >= vec->size)
        return false;
    
    // Verificando se é necessário desalocar o valor anterior de index
    if(vec->elemDestroy != NULL) {
        void *elem = (char *) (vec->data + (index * vec->elemSize));
        vec->elemDestroy(elem);
    }

    // Copiando o valor armazenado no ponteiro passado para a nova posição
    memcpy((char *) vec->data + (index * vec->elemSize), elem, vec->elemSize);
    return true;
}

// Função que imprime o vetor, recebendo a função que imprime 
// o tipo do elemento que ele armazena
void vector_print(VECTOR *vec, void (printElem)(const void*)) {
    if(vec == NULL)
        return;

    for(size_t i = 0; i < vec->size; i++) {
        void *elem = (char *) vec->data + (i * vec->elemSize);

        // Verificando se é um TAD ou uma primitiva para imprimir adequadamente
        if(vec->elemDestroy == NULL) {
            printElem(elem);
            printf(" ");
        }
        else {
            printElem(*(void**)elem);
            printf(" ");
        }
    }
    printf("\n");
}

// Libera todo o espaço alocado para o vetor e os elementos internos caso
// elemDestroy seja diferente de NULL
void vectorDestroy(VECTOR **vec) {
    if(*vec == NULL)
        return;

    // Desalocando as memórias dos elementos internos, caso sejam TAD's
    if((*vec)->elemDestroy != NULL) {
        for(size_t i = 0; i < (*vec)->size; i++) {
            void *elem = (char *) (*vec)->data + ((i * (*vec)->elemSize));
            (*vec)->elemDestroy(elem);
        }
    }

    // Librando a memória array interno
    free((*vec)->data);
    (*vec)->data = NULL;

    // Liberando a memória do vector em si
    free(*vec);
    *vec = NULL;
}


/*
--- REGRAS DO CÓDIGO GENÉRICO ---
- A struct precisará armazenar o tamanho em bytes do tipo guardado

- Qualquer operação arítimética ou de acesso ao buffer genérico precisa de um casting
qualquer, pois do contrário o compilador não entenderá como operar com void*

- Isso inclui inserção, que fazemos com a função memcpy(), pois do contrário:
    - Teríamos que ter uma inserção com casting para cada tipo (não é genérico)
    - Teríamos que armazenar outros ponteiros para void, mas isso leva a mais
      gerenciamento de memória
      
- Operações de I/O precisão receber funções de alta ordem que definem o tratamento
para o tipo utilizado internamente
*/