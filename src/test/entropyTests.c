#include "entropyTests.h"

// Cria uma matriz NxN aleatória para testes
int** generateRandomMatrix() {
    number **matrix = (number **)malloc(BLK_SIZE * sizeof(number *));
    for (int i = 0; i < BLK_SIZE; i++) {
        matrix[i] = (number *)malloc(BLK_SIZE * sizeof(number));
        for (int j = 0; j < BLK_SIZE; j++) {
            matrix[i][j] = rand() % 256 - 128; // Valores entre -128 e 127
        }
    }
    return matrix;
}

// Função de teste para codificação e decodificação
// Função de teste para codificação e decodificação
void testEncodingDecoding() {
    //number **matrix = generateRandomMatrix();
    number *zigZag;
    number *zigZag2;
    RLEPairs rlePairs;
    bitBuffer *buffer = bitBufferCreate(2048);

    int matrixDec[0][0];
    int matrix[8][8] = {
        {1, 2, 3, 4, 0, 0, 0, 0},
        {2, 2, 3, 0, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0, 0, 0},
        {3, 2, 0, 0, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 0},
        {1, 2, 0, 0, 0, 0, 0, 0},
    };

    printf("Matriz NxN:\n");
    for (int i = 0; i < BLK_SIZE; i++) {
        for (int j = 0; j < BLK_SIZE; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    zigZag = zigZagNxN(BLK_SIZE, matrix);

    printf("\nVetor ZigZag:\n");
    for (int i = 0; i < 64; i++) {
        printf("%3d ", zigZag[i]);
    }
    printf("\n");

    zigZag2 = zigZagDifference(zigZag);

    // Free no vetor zigZag
    free(zigZag);

    printf("\nVetor ZigZag:\n");
    for (int i = 0; i < 64; i++) {
        printf("%3d ", zigZag2[i]);
    }
    printf("\n");

    rlePairs = runLengthEncoding(zigZag2, NULL);

    // Free no vetor zigZag2
    free(zigZag2);

    printf("\nPares RLE:\n");
    for (int i = 0; i < rlePairs->size; i++) {
        printf("Run: %d, Level: %d\n", rlePairs->pairs[i].run, rlePairs->pairs[i].level);
    }

    printf("\nCodificação Huffman:\n");
    if (huffman_encoding(buffer, rlePairs)) {
        printf("Codificação Huffman bem-sucedida.\n");
    } else {
        printf("Falha na codificação Huffman.\n");
    }

    bitBufferPrint(buffer);

    // Free no rlePairs
    free(rlePairs->pairs);
    free(rlePairs);

    printf("\nDecodificação Huffman:\n");
    int pos = 0;
    RLEPairs decodedRLE = huffman_decoding(buffer, &pos);
    if (decodedRLE == NULL) {
        printf("Falha na decodificação Huffman.\n");
        bitBufferDestroy(&buffer); // Libera buffer antes de retornar
        return;
    }
    printf("Decodificação RLE:\n");
    for (int i = 0; i < decodedRLE->size; i++) {
        printf("Run: %d, Level: %d\n", decodedRLE->pairs[i].run, decodedRLE->pairs[i].level);
    }
    // Libera o buffer de bits
    bitBufferDestroy(&buffer);

    // Libera a memória alocada para o vetor RLE
    free(decodedRLE->pairs);

    // Decodifica o vetor zigzag
    number *zigZagDec = runLengthDecoding(decodedRLE);

    // Libera decodedRLE após uso
    free(decodedRLE);

    printf("\nVetor ZigZag Decodificado:\n");
    for (int i = 0; i < 64; i++) {
        printf("%3d ", zigZagDec[i]);
    }
    printf("\n");

    number *zigZagDec2 = unZigZagDifference(zigZagDec);

    // Libera zigZagDec após uso
    free(zigZagDec);

    printf("\nVetor ZigZag Decodificado:\n");
    for (int i = 0; i < 64; i++) {
        printf("%3d ", zigZagDec2[i]);
    }
    printf("\n");

    unZigZagNxN(8, matrixDec, zigZagDec);

    printf("\nMatriz Decodificada:\n");
    for (int i = 0; i < BLK_SIZE; i++) {
        for (int j = 0; j < BLK_SIZE; j++) {
            printf("%3d ", matrixDec[i][j]);
        }
        printf("\n");
    }
}
