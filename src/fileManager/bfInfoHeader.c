#include "bfInfoHeader.h"
    
// Lê para um TAD info header os campos de um arquivo já aberto
bool biHeaderRead(BIHEADER *header, FILE *bfPtr) {
    if(header == NULL || bfPtr == NULL)
        return false;
    
    // Verificando se o bfPtr está no início do arquivo
    if(ftell(bfPtr) != INFO_HEADER_OFFSET)
        fseek(bfPtr, INFO_HEADER_OFFSET, SEEK_SET);

    // Lendo campo a campo do arquivo
    fread(&header->size, sizeof(unsigned int), 1, bfPtr);\
    fread(&header->bmpWidth, sizeof(int), 1, bfPtr);
    fread(&header->bmpHeight, sizeof(int), 1, bfPtr);
    fread(&header->planes, sizeof(unsigned short), 1, bfPtr);
    fread(&header->bitCount, sizeof(unsigned short), 1, bfPtr);
    fread(&header->compression, sizeof(unsigned int), 1, bfPtr);
    fread(&header->imageSize, sizeof(unsigned int), 1, bfPtr);
    fread(&header->XPixelsPerMeter, sizeof(int), 1, bfPtr);
    fread(&header->YPixelsPerMeter, sizeof(int), 1, bfPtr);
    fread(&header->clrUsed, sizeof(unsigned int), 1, bfPtr);
    fread(&header->clrImportant, sizeof(unsigned int), 1, bfPtr);

    return true;
}

// Escreve os campos do info header em um arquivo já aberto
bool biHeaderWrite(BIHEADER *header, FILE *bfPtr) {
    if(header == NULL || bfPtr == NULL)
        return false;
    
    // Verificando se o bfPtr está no início do arquivo
    if(ftell(bfPtr) != SEEK_SET)
        fseek(bfPtr, INFO_HEADER_OFFSET, SEEK_SET);

    // Lendo campo a campo do arquivo
    fwrite(&header->size, sizeof(unsigned int), 1, bfPtr);\
    fwrite(&header->bmpWidth, sizeof(int), 1, bfPtr);
    fwrite(&header->bmpHeight, sizeof(int), 1, bfPtr);
    fwrite(&header->planes, sizeof(unsigned short), 1, bfPtr);
    fwrite(&header->bitCount, sizeof(unsigned short), 1, bfPtr);
    fwrite(&header->compression, sizeof(unsigned int), 1, bfPtr);
    fwrite(&header->imageSize, sizeof(unsigned int), 1, bfPtr);
    fwrite(&header->XPixelsPerMeter, sizeof(int), 1, bfPtr);
    fwrite(&header->YPixelsPerMeter, sizeof(int), 1, bfPtr);
    fwrite(&header->clrUsed, sizeof(unsigned int), 1, bfPtr);
    fwrite(&header->clrImportant, sizeof(unsigned int), 1, bfPtr);

    return true;
}

void biHeaderPrint(BIHEADER *header) {
    if(header == NULL)
        return;

    printf("===== INFO HEADER DO BMP =====\n\n");

    printf("dimensões da imagem: (%d, %d)\n", header->bmpWidth, header->bmpWidth);
    printf("Quantidade de color planes: %d\n", header->planes);
    printf("canais de cores (em bits): %d\n", header->bitCount);
    printf("Compressão adotada: %d\n", header->compression);
    printf("Tamanho do bitMap (em bytes): %d\n", header->imageSize);
    printf("resolucao (em px/m): %d x %d\n", header->XPixelsPerMeter, header->YPixelsPerMeter);
    printf("Quantidade de cores utilizadas: %d\n", header->clrUsed);
    printf("Cores importantes: %d\n\n", header->clrImportant);

    return;
}