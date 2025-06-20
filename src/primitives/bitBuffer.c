#include "bitBuffer.h"

#define MAX_CHAR 255

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
    bitBuffer->occupied = 0;
    //bitBufferClean(bitBuffer);

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
    return (bitBuffer->occupied / 8);
}

// Dado um vector com o código de huffman em unsigned char, esta função 
// o insere no buffer utilizando operações bitwise para a
// correta escrita no arquivo
bool bitBufferInsert(BITBUFFER *bitBuffer, VECTOR *code) {
    if(bitBuffer == NULL)
        return false;

    int currIndex = bitBufferGetByteSize(bitBuffer);

    for(int i = 0; i < vectorGetSize(code); i++) {
        // Desloca todos os bits para a esquerda e imprime o valor no menos significativo
        bitBuffer->buffer[currIndex] = (bitBuffer->buffer[currIndex] << 1);
        bitBuffer->buffer[currIndex] = (bitBuffer->buffer[currIndex] | vectorIndexAs(code, unsigned char, i));

        // Atualizando o número de bits ocupados e verificando se é necessário mudar o byte
        bitBuffer->occupied++;
        if(bitBuffer->occupied % 8 == 0) {
            currIndex++;

            // Caso o próximo valor ultrapassa o limite do buffer
            if(currIndex >= bitBuffer->size) {
                return false;
            }
        }
    }
    return true;
}

// Função que reseta todos os bytes no bitBuffer para 0 
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
    // Para o último byte verificamos se ele está completamente cheio. Se nao
    // O byte formado será escrito de trás para frente, com os zeros que sobram
    // Sendo considerados descartáveis no arquivo
    for(int i = 0; i < indexes; i++) {
        if((i == (indexes - 1)) && (bitBuffer->occupied % 8 != 0))
            bitBuffer->buffer[i] = bitBuffer->buffer[i] << (8 - (bitBuffer->occupied % 8)); 
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

    for(int i = 0; i < (bf->occupied / 8) + 1; i++) {
        printf("[%d]: ", i);
        printCharBinaryBE(bf->buffer[i]);
        printf("\n");
    }
    printf("Occupation: %d of %d\n", bf->occupied, (bf->size * 8));

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

// Função que lê o bit em uma posição específica do buffer
bool bitBufferReadBit(BITBUFFER *bitBuffer, int position) {
    if(bitBuffer == NULL || position < 0 || position >= bitBuffer->occupied) {
        return false; // Posição inválida
    }

    // Calcula o índice do byte e o deslocamento do bit
    int byteIndex = position / 8;
    int bitOffset = position % 8;

    // Retorna o valor do bit na posição especificada
    return (bitBuffer->buffer[byteIndex] >> (7 - bitOffset)) & 1;
}

// Função que retorna o número de bits ocupados no bitbuffer
int bitBufferGetOccupiedBits(BITBUFFER *bitBuffer) {
    if(bitBuffer == NULL) {
        return BITBUFFER_ERROR; // Erro ao acessar o bitBuffer
    }
    return bitBuffer->occupied;
}

// Função que retorna o tamanho em bytes alocados para o bitBuffer
int bitBufferGetSize(BITBUFFER *bitBuffer) {
    if(bitBuffer == NULL)
        return BITBUFFER_ERROR;
    return bitBuffer->size;
}

// Insere um char dentro do bitBuffer (usada na descompressão)
bool bitBufferInsertChar(BITBUFFER *bitBuffer, unsigned char val) {
    if(bitBuffer == NULL)
        return false;

    // Verificando se o buffer já não atingiu seu limite
    if((bitBuffer->occupied / 8) >= bitBuffer->size)
        return false;
    bitBuffer->buffer[bitBuffer->occupied / 8] = val;
    bitBuffer->occupied += 8;
    return true;
}  