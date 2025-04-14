#include "encoding.h"

typedef struct rlepair_ {
    int run;
    int level;
} rlepair;
typedef rlepair *RLEPair;

typedef struct rlepairs_ {
    RLEPair pairs;
    int size;
} rlepairs;
typedef rlepairs *RLEPairs;

//Função que converte uma matriz 8x8 em um vetor 1D fazendo zigzag com o bloco da DCT para compressão
number* zigZagNxN(number *matrix[N]) {
    int i = 0, j = 0, index = 0;
    number *zigZag = (number *)malloc(N * N * sizeof(number));
    if (zigZag == NULL) {
        printf("Erro ao alocar memória para o vetor zigZag.\n");
        return NULL;
    }
    // Inicializa o vetor zigZag com zeros
    for (int k = 0; k < N * N; k++) {
        zigZag[k] = 0;
    }
    // Preenche o vetor zigZag com os valores da matriz
    while(index < N * N){
        // Verifica se a posição atual está dentro dos limites da matriz
        if(i >= 0 && i < N && j >= 0 && j < N) {
            zigZag[index++] = matrix[i][j];
        }

        // Move na diagonal
        if((i + j) % 2 == 0) {
            if(j == N - 1) {
                i++;
            } else if(i == 0) {
                j++;
            } else {
                i--;
                j++;
            }
        } else {
            if(i == N - 1) {
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

//codificação por diferença do vetor zigZag
number* zigZagDifference(number *zigZag) {
    number *zigZagDiff = (number *)malloc(N * N * sizeof(number));
    if (zigZagDiff == NULL) {
        printf("Erro ao alocar memória para o vetor zigZagDiff.\n");
        return NULL;
    }
    // Inicializa o vetor zigZagDiff com zeros
    for (int k = 0; k < N * N; k++) {
        zigZagDiff[k] = 0;
    }
    // Preenche o vetor zigZagDiff com as diferenças
    zigZagDiff[0] = zigZag[0];
    for (int i = 1; i < N * N; i++) {
        zigZagDiff[i] = zigZag[i] - zigZag[i - 1];
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


//Run Length Encoding (RLE) para compressão de dados
RLEPairs runLengthEncoding(number *zigZag, int *size) {
    RLEPairs rle = createRLEPairs(N * N);
    int index = 0;
    int count = 0;
    for (int i = 0; i < N * N; i++) {
        if (zigZag[i] == 0) {
            count++;
        } else {
            rle->pairs[index].run = count;
            rle->pairs[index].level = zigZag[i];
            index++;
            count = 0;
        }
    }
    rle->pairs[index].run = EOB; // Último run
    rle->pairs[index].level = 0; // Indica o fim do bloco
    index++;

    rle->size = index; // Tamanho do vetor RLE
    return rle;
}

/*
==========================
FUNÇÕES DE TESTE
==========================
*/

//Gera uma matriz NxN de valores aleatórios
number **generateRandomMatrix() {
    number **matrix = (number **)malloc(N * sizeof(number *));
    for (int i = 0; i < N; i++) {
        matrix[i] = (number *)malloc(N * sizeof(number));
        for (int j = 0; j < N; j++) {
            if (rand() % 2 == 0) {
                matrix[i][j] = 0; // Zeros para simular a compressão
            } else {
                matrix[i][j] = rand() % 256; // Valores aleatórios entre 0 e 255
            }
        }
    }
    return matrix;
}

//Função de teste que gera uma matriz aleatória, imprime-a, converte para zigzag e imprime o resultado
void testZigZag() {
    number **matrix = generateRandomMatrix();
    number *zigZag;

    printf("Matriz NxN:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    zigZag = zigZagNxN(matrix);

    printf("\nVetor ZigZag:\n");
    for (int i = 0; i < 64; i++) {
        printf("%3d ", zigZag[i]);
    }
    printf("\n");

    // Libera a memória alocada
    for (int i = 0; i < 8; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

//Função de teste que gera uma matriz aleatória, imprime-a, aplica RLE e imprime o resultado
void testRLE() {
    number **matrix = generateRandomMatrix();
    number *zigZag;
    RLEPairs rlePairs;

    printf("Matriz NxN:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    zigZag = zigZagNxN(matrix);

    printf("\nVetor ZigZag:\n");
    for (int i = 0; i < 64; i++) {
        printf("%3d ", zigZag[i]);
    }
    printf("\n");

    zigZag = zigZagDifference(zigZag);

    printf("\nVetor ZigZag:\n");
    for (int i = 0; i < 64; i++) {
        printf("%3d ", zigZag[i]);
    }
    printf("\n");

    rlePairs = runLengthEncoding(zigZag, NULL);

    printf("\nPares RLE:\n");
    for (int i = 0; i < rlePairs->size; i++) {
        printf("Run: %d, Level: %d\n", rlePairs->pairs[i].run, rlePairs->pairs[i].level);
    }

    // Libera a memória alocada
    for (int i = 0; i < 8; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Função principal para testar a conversão zigzag
int main() {
    testRLE();
    return 0;
}