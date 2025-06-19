#include "fileManager.h"

//=========================
// I/O DE IMAGENS
//=========================

// Função auxiliar que verifica se as dimensões do bmp carregado são válidas para
// este compressor
bool isBmpValidForCompression(BIHEADER *h) {
    // Verificando se as dimensões são múltiplas de 8
    if((h->bmpHeight % 8 != 0) || (h->bmpWidth % 8 != 0))
        return false;

    // Verificando se as dimensões estão no intervalo esperado
    if(h->bmpHeight < 8 || h->bmpWidth > 1280 || h->bmpHeight < 8 || h->bmpHeight > 800)
        return false;

    // Verificando se o arquivo já não tem nenhuma compressão
    if(h->compression != BI_RGB)
        return false;

    return true;
}

// Carrega os dadaos e cabeçalhos de um arquivo bmp na memória
BMP *loadBmpImage(char *bmpName) {
    BMP *image;
    FILE *bmpPtr;
    BIHEADER iHeader;

    // Abrindo o ponteiro para a image
    bmpPtr = fopen(bmpName, "rb");
    if(bmpPtr == NULL) {
        displayError("Não foi possível abrir o arquivo bmp");
        return NULL;
    }

    // Realizando a leitura do BMP
    image = bmpRead(bmpPtr);
    if(image == NULL) {
        displayError("Não foi possível ler todo o bmp");
        return NULL;
    }
    fclose(bmpPtr);

    // Verificando se este bmp já não foi comprimido ou está nas dimensões
    // Permitidas para o trabalho
    iHeader = bmpGetInfoHeader(image);
    if(!isBmpValidForCompression(&iHeader)) {
        displayError("Este bmp não pode ser comprimido");
        bmpDestroy(&image);
        return NULL;
    }

    return image;
}

// Recebe um novo bmp descomprimido para ser escrito em um arquivo
// *Obs: Ela não apaga o BMP!
bool writeBmpImage(char *bmpName, BMP *newImage) {
    FILE *bmpPtr;
    BFHEADER fHeader;
    bool checkWrite;

    // Verificando o bmp recebido
    fHeader = bmpGetFileHeader(newImage);
    if(!bfHeaderIsValid(&fHeader)) {
        displayError("BMP inválido na memória");
        return false;
    }

    // Abrindo o arquivo
    bmpPtr = fopen(bmpName, "wb");
    if(bmpPtr == NULL) {
        displayError("Não foi possível abrir o arquivo");
        return false;
    }
    
    checkWrite = bmpWrite(newImage, bmpPtr);
    if(!checkWrite) {
        displayError("Não foi possível escrever todo o bmp");
        return false;
    }

    fclose(bmpPtr);
    return true;
}

bool writeCmpFile(char *binName, BIHEADER iHeader, BFHEADER fHeader, CMPHEADER cHeader, BITBUFFER *cmpData) {
    if(binName == NULL || cmpData == NULL)
        return false;
    
    FILE *binPtr;
    bool checkAux;

    // Abrindo o novo arquivo comprimido para a escrita
    binPtr = fopen(binName, "wb");
    if(!binPtr) {
        displayError("Erro ao abrir o novo arquivo");
        return false;
    }

    cmpHeaderPrint(&cHeader);

    // Escrevendo os cabeçalhos
    bfHeaderWrite(&fHeader, binPtr);
    biHeaderWrite(&iHeader, binPtr);
    cmpHeaderWrite(&cHeader, binPtr);

    // escrevendo os dados
    bitBufferWrite(cmpData, binPtr);

    fclose(binPtr);
    return true;
}
