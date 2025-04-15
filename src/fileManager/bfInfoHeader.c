#include "bfInfoHeader.h"

// Aloca memória para um novo info header
BINFOHEADER *bInfoHeaderCreate() {
    BINFOHEADER *header;

    header = (BINFOHEADER *) malloc(sizeof(BINFOHEADER));
    if(header != NULL) {
        header->size = 0;
        header->bmpWidth = 0;
        header->bmpHeight = 0;
        header->planes = 0;
        header->bitCount = 0;
        header->compression = 0;
        header->imageSize = 0;
        header->XPixelsPerMeter = 0;
        header->YPixelsPerMeter = 0;
        header->clrUsed = 0;
        header->clrImportant = 0;
    }

    return header;
}
    
// Lê para um TAD info header os campos de um arquivo já aberto
bool bInfoHeaderRead(BINFOHEADER *header, FILE *bfPtr) {
    if(header == NULL || bfPtr == NULL)
        return false;
    
    // Verificando se o bfPtr está no início do arquivo
    if(ftell(bfPtr) != SEEK_SET)
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
bool bInfoHeaderWrite(BINFOHEADER *header, FILE *bfPtr) {
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

// Desaloca a memória utilizada no info header
void bInfoHeaderDestroy(BINFOHEADER **header) {
    if(*header == NULL)
        return;

    free(*header);
    *header = NULL;
}