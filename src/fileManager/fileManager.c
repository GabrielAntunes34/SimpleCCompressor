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

/*
// Carrega os arquivos de imagens bmp em um vector de pixels,
// além de setar os headers na memóris
VECTOR *loadBmpImage(char *bmpName, BFHEADER *fHeader, BIHEADER *iHeader, bool asYcbcr) {
    if(bmpName == NULL || fHeader == NULL || iHeader == NULL)
        return NULL;
    
    FILE *bmpPtr;
    VECTOR *imgData;
    int pixelQntd;      // Quantidade de pixels na imagem
    bool checkAux;      // Recebe os retornos de funções bool para verificações

    // Abrindo o arquivo bmp
    bmpPtr = fopen(bmpName, "rb");
    if(bmpPtr == NULL) {
        displayError("Nao foi possível abrir o arquivo.");
        return NULL;
    }

    // Carregando os cabeçalhos
    checkAux = loadBmpHeaders(bmpPtr, fHeader, iHeader);
    if(!checkAux)
        return NULL;

    // Populando o vector de acordo com a forma que os bits serão carregados
    fseek(bmpPtr, fHeader->bmpOffset, SEEK_SET);
    pixelQntd = iHeader->bmpHeight * iHeader->bmpWidth;
    imgData = loadBmpData(bmpPtr, pixelQntd, asYcbcr);

    fclose(bmpPtr);
    return imgData;
}

bool writeBmpImage(char *bmpName, BFHEADER *fHeader, BIHEADER *iHeader, VECTOR *imgData) {
    if(bmpName == NULL || fHeader == NULL || iHeader == NULL || imgData == NULL)
        return NULL;

    FILE *bmpPtr;
    bool checkAux;
    pixelRgb px;        // Auxiliar para obter os pixeis do vector
    long pixelQntd;

    bmpPtr = fopen(bmpName, "wb");
    if(bmpPtr == NULL) {
        displayError("Não foi possível abrir o arquivo");
        return false;
    }

    // Escrevendo os cabeçalhos
    checkAux = bfHeaderWrite(fHeader, bmpPtr);
    if(!checkAux) {
        displayError("Erro ao escrever o file header bmp");
        return false;
    }
    checkAux = biHeaderWrite(iHeader, bmpPtr);
    if(!checkAux) {
        displayError("Erro ao escrever o file header bmp");
        return false;
    }

    // Escrevendo os pixeis contidos no vetor
    for(int i = 0; i < vectorGetSize(imgData); i++) {
        px = vectorIndexAs(imgData, pixelRgb, i);
        pixelRgbWrite(&px, bmpPtr);
    }

    // Fechando o arquivo e retornando
    fclose(bmpPtr);
    return true;
}
*/

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

//==========================
// Interface do compressor
//==========================


// Comprime uma imagem bmp já carregada com o algorítimo JPEG
bool compress(BMP *bmp) {
    PIXELYCBCR **ycbcrMat;
    VECTOR *yBlocks;
    VECTOR *crBlcocks;
    VECTOR *cbBlocks;

    // Preparando a imagem para a dct
    ycbcrMat = bmpGetYcbcrData(bmp);
    //downSample420(&ycbcrMat, bmpGetWidth(bmp), bmpGetHeigth(bmp));
    prepareBlocks(&ycbcrMat, bmpGetWidth(bmp), bmpGetHeigth(bmp), yBlocks, cbBlocks, crBlcocks);


    // Liberando a matriz ycbcr
    for(int i = 0; i < bmpGetHeigth(bmp); i++) {
        free(ycbcrMat[i]);
        ycbcrMat[i] = NULL;
    }
    free(ycbcrMat);
    ycbcrMat = NULL;

    // Libreando os vectors
    vectorDestroy(&yBlocks);
    vectorDestroy(&crBlcocks);
    vectorDestroy(&cbBlocks);
    return false;
}

// Descomprime uma imagem JPEG para um bmp
bool decompress() {
    return false;
}
