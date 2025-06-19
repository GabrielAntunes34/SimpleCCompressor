#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Funções de teste
#include "fileManager/fileManager.h"
#include "fileManager/compressor.h"

int main(int argc, char *argv[]) {
    // Mostrando a utilização caso o usuário não forneça todos os argumentos
    if (argc != 4) {
        printf("Uso: %s [-c | -d] <arquivo_entrada> <arquivo_saida>\n", argv[0]);
        return 1;
    }

    // Obtendo os valores dos argumentos do terminal
    char* flag = argv[1];
    char* entrada = argv[2];
    char* saida = argv[3];

    // Usado para armazenar a taxa de compressão obtida
    int compressTax = 0;
    bool checkDecompress;

    // Tomando a ação de acordo com a flag
    if (strcmp(flag, "-c") == 0) {
        compressTax = compress(entrada, saida);

        if(compressTax == -1)
            printf("Compressão interrompida devido a algum erro\n");
        else {
            printf("Compressão concluida com sucesso!\n");
            printf("Taxa obtida: %d por cento\n", compressTax);
        }
    }
    else if (strcmp(flag, "-d") == 0) {
        decompress(entrada, saida);
    }
    else {
        printf("Flag inválida: %s\n", flag);
        printf("Use -c para comprimir ou -d para descomprimir.\n");
        return 1;
    }

    return 0;
}