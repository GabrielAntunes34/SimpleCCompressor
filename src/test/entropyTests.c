#include "entropyTests.h"

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

void testEncodingDecoding() {
    number **matrix = generateRandomMatrix();
    number *zigZag;
    RLEPairs rlePairs;
    bitBuffer *buffer = bitBufferCreate(2048);


    printf("Matriz NxN:\n");
    for (int i = 0; i < BLK_SIZE; i++) {
        for (int j = 0; j < BLK_SIZE; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }

    //zigZag = zigZagNxN(BLK_SIZE, matrix);

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

    printf("\nCodificação Huffman:\n");
    if (huffman_encoding(buffer, rlePairs)) {
        printf("Codificação Huffman bem-sucedida.\n");
    } else {
        printf("Falha na codificação Huffman.\n");
    }
    printf("Tamanho do buffer: %d bytes\n", bitBufferGetByteSize(buffer));
    printf("Bits ocupados: %d\n", bitBufferGetOccupiedBits(buffer));
    printf("Bytes ocupados: %d\n", bitBufferGetByteSize(buffer));
    printf("Buffer em binário:\n");
    for (int i = 0; i < bitBufferGetByteSize(buffer); i++) {
        printf("Byte %d: ", i);
        printf("\n");
    }
    printf("\nDecodificação Huffman:\n");
    RLEPairs decodedRLE = huffman_decoding(buffer);
    if (decodedRLE == NULL) {
        printf("Falha na decodificação Huffman.\n");
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
    free(decodedRLE);

    number *zigZagDec = runLengthDecoding(rlePairs);

    printf("\nVetor ZigZag Decodificado:\n");
    for (int i = 0; i < 64; i++) {
        printf("%3d ", zigZagDec[i]);
    }
    printf("\n");

    zigZagDec = unZigZagDifference(zigZagDec);

    printf("\nVetor ZigZag Decodificado:\n");
    for (int i = 0; i < 64; i++) {
        printf("%3d ", zigZagDec[i]);
    }
    printf("\n");

    number **matrixDec = unZigZagNxN(zigZagDec);

    printf("\nMatriz Decodificada:\n");
    for (int i = 0; i < BLK_SIZE; i++) {
        for (int j = 0; j < BLK_SIZE; j++) {
            printf("%3d ", matrixDec[i][j]);
        }
        printf("\n");
    }

    // Libera a memória alocada
    for (int i = 0; i < 8; i++) {
        free(matrix[i]);
    }
    free(matrix);
}