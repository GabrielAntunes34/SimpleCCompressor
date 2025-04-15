#include "bfHeader.h"

// Cria na heap um header para arquivo bmp com os campos vazios
BFHEADER *bfHeaderCreate() {
    BFHEADER *header;

    // Alocando memória e setando todos os campos para um valor nulo (0)
    header = (BFHEADER *) malloc(sizeof(BFHEADER));
    if(header != NULL) {
        header->magic = 0;
        header->fileSize = 0;
        header->reserved1 = 0;
        header->reserved2 = 0;
        header->bmpOffset = 0;
    }

    return header;
}

// Verifica se o magic number do cabeçalho corresponde ao de um bmp
bool bfHeaderIsValid(BFHEADER *header) {
    return true;
}

// Lê os dados do header em um arquivo já aberto
bool bfHeaderRead(BFHEADER *header, FILE *bfPtr) {
    if(header == NULL || bfPtr == NULL)
        return false;

    // Verificando se o bfPtr está no início do arquivo
    if(ftell(bfPtr) != SEEK_SET)
        fseek(bfPtr, 0, SEEK_SET);

    // Lendo campo a campo no arquivo
    fread(&header->magic, sizeof(unsigned short), 1, bfPtr);
    fread(&header->fileSize, sizeof(unsigned int), 1, bfPtr);
    fread(&header->reserved1, sizeof(unsigned short), 1, bfPtr);
    fread(&header->reserved2, sizeof(unsigned short), 1, bfPtr);
    fread(&header->bmpOffset, sizeof(unsigned int), 1, bfPtr);

    return true;
}

// Escreve os dados do info header em um arquivo já valido
bool bfHeaderWrite(BFHEADER *header, FILE *bfPtr) {
    if(bfPtr == NULL || (!bfHeaderIsValid(header)))
        return false;
    
    // Verificando se o bfPtr está no início do arquivo
    if(ftell(bfPtr) != SEEK_SET)
        fseek(bfPtr, 0, SEEK_SET);

    // Escrevendo campo a campo no arquivo
    fwrite(&header->magic, sizeof(unsigned short), 1, bfPtr);
    fwrite(&header->fileSize, sizeof(unsigned int), 1, bfPtr);
    fwrite(&header->reserved1, sizeof(unsigned short), 1, bfPtr);
    fwrite(&header->reserved2, sizeof(unsigned short), 1, bfPtr);
    fwrite(&header->bmpOffset, sizeof(unsigned int), 1, bfPtr);

    return true;
}

// Desaloca a memória para o header
void bfHeaderDestroy(BFHEADER **header) {
    if(*header == NULL)
        return;

    free(*header);
    *header = NULL;
}