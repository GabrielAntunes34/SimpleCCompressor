#include "bitBuffer.h"

struct bitBuffer {
    int size;       // Tamanho do vetor de chars (ou seja, do buffer)
    int occupied;   // bits ocupados pelos códigos em todo o buffer
    unsigned char *buffer;
};

// Cria um buffer de n bytes
BITBUFFER *bitBufferCreate(int size) {
    BITBUFFER *bitBuffer;

    // Alocando o buffer de bytes
    bitBuffer = (BITBUFFER *) malloc(sizeof(BITBUFFER));
    if(bitBuffer == NULL)
        return NULL;
    
    // Inicializando os valores e alocando o vetor interno
    bitBuffer->size = size;
    bitBuffer->buffer = (unsigned char *) malloc(size * sizeof(unsigned char));
    if(bitBuffer->buffer == NULL) {
        free(bitBuffer);
        bitBuffer = NULL;
        return NULL;
    }
    bitBufferClean(bitBuffer);

    return bitBuffer;
}

// Função que verifica se o bitBuffer está vazio
bool bitBufferIsEmpty(BITBUFFER *bitBuffer) {
    if(bitBuffer == NULL)
        return false;
    
    if(bitBuffer->occupied == 0)
        return true;
    return false;
}

// Função que retorna no número de bytes preenchidos no buffer
int bitBufferGetByteSize(BITBUFFER *bitBuffer) {
    if(bitBuffer == NULL)
        return BITBUFFER_ERROR;

    if(bitBuffer->occupied % 8 != 0)
        return (bitBuffer->occupied / 8) + 1; //Ocorre apenas ao fim do arquivo
    else
        return (bitBuffer->occupied / 8);
}

// Dado um código especificado em uma string, esta função o insere no buffer
// utilizando operações bitwise para a correta escrita no arquivo
bool bitBufferInsert(BITBUFFER *bitBuffer, unsigned int *code, int size, FILE *pf) {
    if(bitBuffer == NULL || pf == NULL)
        return false;

    int currIndex = bitBufferGetByteSize(bitBuffer);

    for(int i = 0; i < size; i++) {
        // Desloca todos os bits para a esquerda e imprime o valor no menos significativo
        bitBuffer->buffer[currIndex] = bitBuffer->buffer[currIndex] << 1;
        bitBuffer->buffer[currIndex] = bitBuffer->buffer[currIndex] | code[i];

        // Atualizando o número de bits ocupados e verificando se é necessário mudar o byte
        bitBuffer->occupied++;
        if(bitBuffer->occupied % 8 == 0) {
            currIndex++;

            // Caso em que o próximo valor ultrapassa o limite do buffer
            if(currIndex >= bitBuffer->size) {
                bitBufferWrite(bitBuffer, pf);
                currIndex = 0;
            }
        }
    }
    return true;
}

// Função que eseta todos os bytes no bitBuffer para 
bool bitBufferClean(BITBUFFER *bitBuffer) {
    if(bitBuffer == NULL)
        return false;

    for(int i = 0; i < bitBuffer->size; i++){
        bitBuffer->buffer[i] = 0;
    }
    bitBuffer->occupied = 0;

    return true;
}

// Função que escreve os conteúdos do bitBuffer em um arquivo já aberto
bool bitBufferWrite(BITBUFFER *bitBuffer, FILE *pf) {
    if(bitBuffer == NULL || pf == NULL)
        return false;
    
    // Calculando o número de indíces ocupados
    int indexes;
    indexes = bitBufferGetByteSize(bitBuffer);
    
    // Escrevendo cada byte ocupado no arquivo.
    // Para o último byte verificamos se ele está completamente cheio. Se sim
    // O byte formado será escrito de trás para frente, com os zeros que sobram
    // Sendo considerados descartáveis no arquivo
    for(int i = 0; i < indexes; i++) {
        if((i == (indexes - 1)) && (bitBuffer->occupied % 8 != 0)) {
            printf("\nFinal shift: %d\n", (8 - (bitBuffer->occupied % 8)));
            bitBuffer->buffer[i] = bitBuffer->buffer[i] << (8 - (bitBuffer->occupied % 8)); 
        }
        fwrite(&bitBuffer->buffer[i], sizeof(char), 1, pf);

    }

    bitBufferClean(bitBuffer);
    return true;
}


// Função auxiliar para printar os bytes do bit buffer em binário little endian
void printCharBinaryBE(unsigned char c) {
    // Desloca cada bit para a posição menos significativa
    // e executa & para obter seu valor na impressão
    for (int i = 7; i >= 0; i--) {
        printf("%d", (c >> i) & 1);
    }
    return;
}

// Imprime em binário o valor de cada byte, além das propriedades do buffer
void bitBufferPrint(const void *bitBuffer) {
    if(bitBuffer == NULL)
        return;

    BITBUFFER *bf = (BITBUFFER *) bitBuffer;

    for(int i = 0; i < bf->size; i++) {
        printf("[%d]: ", i);
        printCharBinaryBE(bf->buffer[i]);
        printf("\n");
    }
    printf("Occupation: %d of %d\n", bf->occupied + 1, (bf->size * 8));

    return;
}

bool bitBufferDestroy(BITBUFFER **bitBuffer) {
    if(*bitBuffer == NULL)
        return false;
    
    // Apagando o buffer Interno
    if((*bitBuffer)->buffer != NULL) {
        free((*bitBuffer)->buffer);
        (*bitBuffer)->buffer = NULL;
    }
    
    // Apagando o ponteiro para o TAD bitBuffer
    free(*bitBuffer);
    *bitBuffer = NULL;

    return true;
}