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

    // Verificando se o arquivo não tem nenhuma compressão
    if(h->compression != BI_RGB)
        return false;

    return true;
}

// Auxiliar que se dedica a carregar e conferir os cabeçalhos de um BMP
bool loadBmpHeaders(FILE *bmpPtr, BFHEADER *fHeader, BIHEADER * iHeader) {
    bfHeaderRead(fHeader, bmpPtr);
    if(!bfHeaderIsValid(fHeader)) {
        displayError("Arquivo não é uma imagem BMP!");
        fHeader = NULL;
        return false;
    }

    biHeaderRead(iHeader, bmpPtr);
    if(!isBmpValidForCompression(iHeader)) {
        displayError("Dimensões da imagem BMP fogem ao escopo deste compressor.");
        fHeader = NULL;
        iHeader = NULL;
        return false;
    }

    return true;
}

VECTOR *loadBmpData(FILE *bmpPtr, int pixelQntd, bool asYcbcr) {
    VECTOR *imgData;

    if(asYcbcr) {
        PIXELYCBCR px;  // Auxiliar para ler os pixels
        imgData = vectorCreateAs(pixelRgb, NULL);
        if(imgData == NULL)
            return NULL;

        for(int i = 0; i < pixelQntd; i++) {
            px = pixelYcbcrRead(bmpPtr);
            vectorPushBack(imgData, &px);
        }
    }
    else {
        PIXELRGB px; // auxiliar para ler e inserir os pixels
        imgData = vectorCreateAs(pixelYcbcr, NULL);
        if(imgData == NULL)
            return NULL;

        for(int i = 0; i < pixelQntd; i++) {
            px = pixelRgbRead(bmpPtr);
            vectorPushBack(imgData, &px);
        }
    }

    return imgData;
}

// Carrega os arquivos de imagens bmp em um vector de pixels,
// além de setar os headers na memóris
VECTOR *loadBmpImage(char *bmpName, BFHEADER *fHeader, BIHEADER *iHeader, bool asYcbcr) {
    if(bmpName == NULL || fHeader == NULL || iHeader == NULL)
        return NULL;
    
    FILE *bmpPtr;
    VECTOR *imgData;
    int pixelQntd;
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

    return imgData;
}

//bool writeBMPImage(char *bmpName) {

//}


//==========================
// Interface do compressor
//==========================
