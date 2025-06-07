#include "bmp.h"

struct bmp {
    BIHEADER iHeader;
    BFHEADER fHeader;
    PIXELRGB **data;
};

// Auxiliar to load all the data needed
pixelRgb **loadBmpData(int width, int heigth, FILE *bmpPtr) {
    PIXELRGB **imgData;

    // Alocando a matriz de pixeis RGB
    imgData = (PIXELRGB **) malloc(heigth * sizeof(PIXELRGB *));
    if(imgData == NULL)
        return NULL;

    // Alocando e populando cada vetor interno
    for(int i = 0; i < heigth; i++) {
        imgData[i] = (PIXELRGB *) malloc(width * sizeof(PIXELRGB));

        for(int j = 0; j < width; j++) {
            imgData[i][j] = pixelRgbRead(bmpPtr);
        }
    }

    return imgData;
}

// Lê os cabeçalhos e a o bitmap de um bmp para uma matriz local
BMP *bmpRead(FILE *bmpPtr) {
    BMP *bmp;

    bmp = (BMP *) malloc(sizeof(BMP));
    
    // Lendo os cabeçalhos
    bfHeaderRead(&bmp->fHeader, bmpPtr);
    if(!bfHeaderIsValid(&bmp->fHeader)) {
        displayError("Arquivo não é uma imagem BMP!");
        free(bmp);
        bmp = NULL;
        return NULL;
    }

    biHeaderRead(&bmp->iHeader, bmpPtr);
    
    // Lendo os dados da imagem em uma matriz de pixeis
    bmp->data = loadBmpData(bmp->iHeader.bmpWidth, bmp->iHeader.bmpHeight, bmpPtr);
    return bmp;
}

bool bmpWrite(BMP *image, FILE *bmpPtr) {
    bool checkAux;

    // Escrevendo os cabeçalhos
    checkAux = bfHeaderWrite(&image->fHeader, bmpPtr);
    if(!checkAux) {
        displayError("Erro ao escrever o file header bmp");
        return false;
    }
    checkAux = biHeaderWrite(&image->iHeader, bmpPtr);
    if(!checkAux) {
        displayError("Erro ao escrever o file header bmp");
        return false;
    }

    // Escrevendo os pixeis contidos na matriz
    for(int i = 0; i < image->iHeader.bmpHeight; i++) {
        for(int j = 0; j < image->iHeader.bmpWidth; j++) {
            pixelRgbWrite(&image->data[i][j], bmpPtr);
        }
    }

    return true;
}

// Converte o campo data de pixeis rgb para um campo de pixeis ycbcr e retorna
// a nova matriz agora desacoplada
PIXELYCBCR **bmpGetYcbcrData(BMP *image) {
    int width = image->iHeader.bmpWidth;
    int heigth = image->iHeader.bmpHeight;

    PIXELYCBCR **imgData;

    // Alocando a matriz de pixeis RGB
    imgData = (PIXELYCBCR **) malloc(heigth * sizeof(PIXELYCBCR *));
    if(imgData == NULL)
        return NULL;

    // Alocando e populando cada vetor interno
    for(int i = 0; i < heigth; i++) {
        imgData[i] = (PIXELYCBCR *) malloc(width * sizeof(PIXELYCBCR));

        for(int j = 0; j < width; j++) {
            imgData[i][j] = pixelConvertRgbToYcbcr(&image->data[i][j]);
        }
    }

    return imgData;
}

// Retorna a matriz interna de pixeis rgb
PIXELRGB **bmpGetRgbData(BMP *image) {
    if(image == NULL)
        return NULL;
    return image->data;
}

// Retorna o infoHeader interno
BIHEADER bmpGetInfoHeader(BMP *image) {
    return image->iHeader;   
}
BFHEADER bmpGetFileHeader(BMP *image) {
    return image->fHeader;
}

void bmpPrint(BMP *image, bool printHeaders, bool printData) {
    if(image == NULL)
        return;
    
    if(printHeaders) {
        bfHeaderPrint(&image->fHeader);
        biHeaderPrint(&image->iHeader);
    }

    if(printData) {
        for(int i = 0; i < image->iHeader.bmpHeight; i++) {
            for(int j = 0; j < image->iHeader.bmpWidth; j++) {
                pixelRgbPrint(&image->data[i][j]);
            }
        }
    }
    
    return;
}

// Libera a memória alocada para o BMP
void bmpDestroy(BMP **image) {
    if((*image) == NULL)
        return;

    // Liberando a matriz interna
    for(int i = 0; i < (*image)->iHeader.bmpHeight; i++) {
        free((*image)->data[i]);
        (*image)->data[i] = NULL;
    }
    free((*image)->data);
    (*image)->data = NULL;

    // Liberando o bmp
    free(*image);
    return;
}