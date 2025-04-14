#include "encoding.h"

struct rlepair {
    int run;
    int level;
};

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
            matrix[i][j] = rand() % 256; // Valores aleatórios entre 0 e 255
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

// Função principal para testar a conversão zigzag
int main() {
    testZigZag();
    return 0;
}