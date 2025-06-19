#include "spacialEncoding.h"

//Função que converte uma matriz 8x8 em um vetor 1D fazendo zigzag com o bloco da DCT para compressão
int* zigZagNxN(int blkSize, int matrix[blkSize][blkSize]) {
    int i = 0, j = 0, index = 0;

    // Alocando o vetor retulstante
    int *zigZag = (int *)malloc(blkSize * blkSize * sizeof(int));
    if (zigZag == NULL) {
        printf("Erro ao alocar memória para o vetor zigZag.\n");
        return NULL;
    }

    // Inicializando o vetor zigZag com zeros
    for (int k = 0; k < blkSize * blkSize; k++) {
        zigZag[k] = 0;
    }

    // Preenchendo o vetor zigZag com os valores da matriz
    while(index < blkSize * blkSize){
        // Verifica se a posição atual está dentro dos limites da matriz
        if(i >= 0 && i < blkSize && j >= 0 && j < blkSize) {
            zigZag[index++] = matrix[i][j];
        }

        // Move na diagonal
        if((i + j) % 2 == 0) {
            if(j == blkSize - 1) {
                i++;
            } else if(i == 0) {
                j++;
            } else {
                i--;
                j++;
            }
        } else {
            if(i == blkSize - 1) {
                j++;
            } else if(j == 0) {
                i++;
            } else {
                i++;
                j--;
            }
        }
    }
    return zigZag;
}

void unZigZagNxN(int blkSize, int blk[blkSize][blkSize], int *zigZag) {
    int i = 0, j = 0, index = 0;

    // Preenche a matriz com os valores do vetor zigZag
    while(index < BLK_SIZE * BLK_SIZE){
        // Verifica se a posição atual está dentro dos limites da matriz
        if(i >= 0 && i < BLK_SIZE && j >= 0 && j < BLK_SIZE) {
            blk[i][j] = zigZag[index++];
        }

        // Move na diagonal
        if((i + j) % 2 == 0) {
            if(j == BLK_SIZE - 1) {
                i++;
            } else if(i == 0) {
                j++;
            } else {
                i--;
                j++;
            }
        } else {
            if(i == BLK_SIZE - 1) {
                j++;
            } else if(j == 0) {
                i++;
            } else {
                i++;
                j--;
            }
        }
    }
}

//codificação por diferença do vetor zigZag
int* zigZagDifference(int *zigZag) {
    int *zigZagDiff = (int *)malloc(BLK_SIZE * BLK_SIZE * sizeof(int));
    if (zigZagDiff == NULL) {
        printf("Erro ao alocar memória para o vetor zigZagDiff.\n");
        return NULL;
    }
    // Inicializa o vetor zigZagDiff com zeros
    for (int k = 0; k < BLK_SIZE * BLK_SIZE; k++) {
        zigZagDiff[k] = 0;
    }
    // Preenche o vetor zigZagDiff com as diferenças
    zigZagDiff[0] = zigZag[0];
    for (int i = 1; i < BLK_SIZE * BLK_SIZE; i++) {
        zigZagDiff[i] = zigZag[i] - zigZag[i - 1];
    }
    return zigZagDiff;
}

int* unZigZagDifference(int *zigZag) {
    int *zigZagDiff = (int *)malloc(BLK_SIZE * BLK_SIZE * sizeof(int));
    if (zigZagDiff == NULL) {
        printf("Erro ao alocar memória para o vetor zigZagDiff.\n");
        return NULL;
    }
    // Inicializa o vetor zigZagDiff com zeros
    for (int k = 0; k < BLK_SIZE * BLK_SIZE; k++) {
        zigZagDiff[k] = 0;
    }
    // Preenche o vetor zigZagDiff com as diferenças
    zigZagDiff[0] = zigZag[0];
    for (int i = 1; i < BLK_SIZE * BLK_SIZE; i++) {
        zigZagDiff[i] = zigZag[i] + zigZagDiff[i - 1];
    }
    return zigZagDiff;
}

//Cria um RLEPairs
RLEPairs createRLEPairs(int size) {
    RLEPairs rlePairs = (RLEPairs)malloc(sizeof(rlepairs));
    if (rlePairs == NULL) {
        printf("Erro ao alocar memória para RLEPairs.\n");
        return NULL;
    }
    rlePairs->pairs = (RLEPair)malloc(size * sizeof(rlepair));
    if (rlePairs->pairs == NULL) {
        printf("Erro ao alocar memória para os pares RLE.\n");
        free(rlePairs);
        return NULL;
    }
    rlePairs->size = size;
    return rlePairs;
}

// Desaloca a struct RLEPairs
void destroyRLEPairs(RLEPairs *pairs) {
    if(*pairs == NULL)
        return;
    
    // desalocando os pares internos
   for(int i = 0; i < (*pairs)->size; i++) {
        free((*pairs)->pairs);
        (*pairs)->pairs = NULL;
   } 

   // Desalocando o vetor
   free(*pairs);
   *pairs = NULL;
}


//Run Length Encoding (RLE) para compressão de dados
RLEPairs runLengthEncoding(int *zigZag, int *size) {
    RLEPairs rle = createRLEPairs(BLK_SIZE * BLK_SIZE);
    int index = 0;
    int count = 0;

    for (int i = 0; i < BLK_SIZE * BLK_SIZE; i++) {
        if (zigZag[i] == 0) {
            count++;
            if(count == 16) {
                rle->pairs[index].run = 15;
                rle->pairs[index].level = 0;
                index++;
                if(i != 63) count = 0; 
            }
        } else {
            rle->pairs[index].run = count;
            rle->pairs[index].level = zigZag[i];
            index++;
            count = 0;
        }
    }
    rle->pairs[index].run = 0; // Último run
    rle->pairs[index].level = 0; // Indica o fim do bloco
    index++;

    rle->size = index; // Tamanho do vetor RLE
    return rle;
}

//RunLength Decoding (RLD) para descompressão de dados
int* runLengthDecoding(RLEPairs rle) {
    int *zigZag = (int *)malloc(BLK_SIZE * BLK_SIZE * sizeof(int));
    if (zigZag == NULL) {
        printf("Erro ao alocar memória para o vetor zigZag.\n");
        return NULL;
    }
    // Inicializa o vetor zigZag com zeros
    for (int k = 0; k < BLK_SIZE * BLK_SIZE; k++) {
        zigZag[k] = 0;
    }
    int index = 0;
    for (int i = 0; i < rle->size; i++) {
        for (int j = 0; j < rle->pairs[i].run; j++) {
            zigZag[index++] = 0;
        }
        if (rle->pairs[i].level != 0) {
            zigZag[index++] = rle->pairs[i].level;
        } else {
            break;
        }
    }
    return zigZag;
}


