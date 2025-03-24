#include "bitBuffer.h"

// Struct que define o byte
struct bitBuffer {
    int size;       // Tamanho do vetor de chars (ou seja, do buffer)
    int occupied;   // bits ocupados pelos códigos em todo o buffer
    unsigned char *buffer;
};

// Função auxliar que eseta todos os bytes no bitBuffer para 
bool bitBufferClean(BITBUFFER *bitBuffer) {
    if(bitBuffer == NULL)
        return false;

    for(int i = 0; i < bitBuffer->size; i++){
        bitBuffer->buffer = 0;
    }
    bitBuffer->occupied = 0;

    return true;
}

BITBUFFER *bitBufferCreate(int size) {
    BITBUFFER *bitBuffer;

    // Alocando o buffer de bytes
    bitBuffer = (BITBUFFER *) malloc(sizeof(BITBUFFER));
    if(bitBuffer == NULL)
        return NULL;
    
    // Inicializando os valores e alocando o vetor interno
    bitBuffer->size = size;
    bitBuffer->occupied = 0;
    bitBuffer->buffer = (char *) malloc(size * sizeof(char));
    if(bitBuffer->buffer == NULL) {
        free(bitBuffer);
        bitBuffer = NULL;
        return NULL;
    }
    bitBufferClean(bitBuffer);

    return bitBuffer;
}

bool bitBufferIsEmpity(BITBUFFER *bitBuffer) {
    if(bitBuffer == NULL)
        return false;
    
    if(bitBuffer->occupied == 0)
        return true;
    return false;
}

// Dado um código especificado em uma string, esta função o insere no buffer
// utilizando operações bitwise para a correta escrita no arquivo
bool bitBufferInsert(BITBUFFER *bitBuffer, unsigned char *code, int size, FILE *pf) {
    if(bitBuffer == NULL)
        return false;

    int currIndex = bitBufferGetSize(bitBuffer);

    for(int i; i < size; i++) {
        bitBuffer->buffer[currIndex] = bitBuffer->buffer[currIndex] << 1;
        bitBuffer->buffer[currIndex] = bitBuffer->buffer[currIndex] | code[i];

        // Atualizando o número de bits ocupados e verificando se é necessário mudar o byte
        bitBuffer->occupied++;
        if(bitBuffer->occupied % 8 == 0) {
            currIndex++;

            // Caso em que o próximo valor ultrapassa o limite do buffer
            if(currIndex > bitBuffer->size) {
                bitBufferWrite(bitBuffer, pf);
                currIndex = 0;
            }
        }

        return true;
    }
}

int bitBufferGetByteSize(BITBUFFER *bitBuffer) {
    if(bitBuffer == NULL)
        return NULL;

    if(bitBuffer->occupied % 8 != 0)
        return (bitBuffer->occupied / 8) + 1; //Ocorre apenas ao fim do arquivo
    else
        return (bitBuffer->occupied / 8);
}

bool bitBufferWrite(BITBUFFER *bitBuffer, FILE *pf) {
    if(bitBuffer == NULL || pf == NULL)
        return false;
    
    // Calculando o número de indíces ocupados
    int indexes;
    indexes = bitBufferGetByteSize(bitBuffer);
    
    // Escrevendo cada byte ocupado no arquivo.
    for(int i = 0; i < indexes; i++) {
        fwrite(&bitBuffer->buffer[i], sizeof(char), 1, pf);
    }

    bitBufferClean(bitBuffer);
    return true;
}


// Função auxiliar para printar os bytes do bit buffer em binário little endian
void printCharBinaryLE(unsigned char c) {
    for (int i = 0; i < 8; i++) {
        // Print the least significant bit first
        printf("%d", (c >> i) & 1);
    }
    printf("\n");
}

void bitBufferPrint(BITBUFFER *bitBuffer) {
    if(bitBuffer == NULL)
        return;

    for(int i = 0; i < bitBuffer->size; i++) {
        printCharBinaryLE(bitBuffer->buffer[i]);
    }

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