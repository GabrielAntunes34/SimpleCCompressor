#include "cmpHeader.h"

// Lê o cmpHeader de um arquivo comprimido já aberto
bool cmpHeaderRead(CMPHEADER *header, FILE *filePtr) {
    if(header == NULL)
        return false;

    // Verificando se o ponteiro para o arquivo está na posição correta
    if(ftell(filePtr) != CMPHEADER_OFFSET)
        fseek(filePtr, CMPHEADER_OFFSET, SEEK_SET);
    
    // Lendo cada campo do arquivo
    fread(&header->yBlocks, sizeof(int), 1, filePtr);
    fread(&header->cbBlocks, sizeof(int), 1, filePtr);
    fread(&header->crBlocks, sizeof(int), 1, filePtr);
    fread(&header->cromPdHeigth, sizeof(int), 1, filePtr);
    fread(&header->cromPdWidth, sizeof(int), 1, filePtr);
    fread(&header->yCmpBytes, sizeof(int), 1, filePtr);
    fread(&header->cbCmpBytes, sizeof(int), 1, filePtr);
    fread(&header->crCmpBytes, sizeof(int), 1, filePtr);

    return true;
}

// Escreve os dados de um arquivo comprimido em 
bool cmpHeaderWrite(CMPHEADER *header, FILE *filePtr) {
    if(header == NULL)
        return false;

        // Verificando se o ponteiro para o arquivo está na posição correta
    if(ftell(filePtr) != CMPHEADER_OFFSET)
        fseek(filePtr, CMPHEADER_OFFSET, SEEK_SET);
    
    // Escrevendo campo a cmpo no arquivoi
    fwrite(&header->yBlocks, sizeof(int), 1, filePtr);
    fwrite(&header->cbBlocks, sizeof(int), 1, filePtr);
    fwrite(&header->crBlocks, sizeof(int), 1, filePtr);
    fwrite(&header->cromPdHeigth, sizeof(int), 1, filePtr);
    fwrite(&header->cromPdWidth, sizeof(int), 1, filePtr);
    fread(&header->yCmpBytes, sizeof(int), 1, filePtr);
    fread(&header->cbCmpBytes, sizeof(int), 1, filePtr);
    fread(&header->crCmpBytes, sizeof(int), 1, filePtr);

    return true;
}

bool cmpHeaderPrint(CMPHEADER *header) {
    if(header == NULL)
        return false;

    printf("===== HEADER DO ARQUIVO COMPRIMIDO =====\n\n");

    printf("Número de blocos no canal Y: %d\n", header->yBlocks);
    printf("Número de blocos no canal Cb: %d\n", header->cbBlocks);
    printf("Número de blocos no canal Cr: %d\n", header->crBlocks);
    printf("Padding adcionado na altura das croôminancias: %d\n", header->cromPdHeigth);
    printf("Padding adcionado na largura das croôminancias: %d\n", header->cromPdWidth);
    printf("Número de bytes finais do canal y: %d\n", header->yCmpBytes);
    printf("Número de bytes finais do canal Cb: %d\n", header->cbCmpBytes);
    printf("Número de bytes finais do canal Cr: %d\n", header->crCmpBytes);
    return true;
}
