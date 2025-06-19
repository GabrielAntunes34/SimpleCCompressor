#include "bmp.h"

struct bmp {
    BIHEADER iHeader;
    BFHEADER fHeader;
    PIXELRGB **data;
};

// Cria um bmp com parametros passados na memória pelo usuário
// Devido ao processo de descompressão, data pode ser nulo
BMP *bmpCreate(BIHEADER iHeader, BFHEADER fHeader, PIXELRGB** data) {
    BMP *image;

    // Alocando dinâmicamente o BMP
    image = (BMP *) malloc(sizeof(BMP));
    if(image == NULL) 
        return NULL;

    // Setando os cabeçalhos
    image->iHeader = iHeader;
    image->fHeader = fHeader;

    // Verificando se é preciso alocar uma matrix para os pixeis
    if(data == NULL) {
        image->data = (PIXELRGB **) malloc(iHeader.bmpHeight * sizeof(PIXELRGB *));
        for(int i = 0; i < iHeader.bmpHeight; i++) {
            image->data[i] = (PIXELRGB *) malloc(iHeader.bmpWidth * sizeof(PIXELRGB));
        }
    }
    else
        image->data = data;

    return image;
}

// Auxiliar para carregar todo o bitmap da imagem
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

// Converte cada pixel de data para ycbcr e grava em matrizes passadas por
// referencia
void bmpGetYcbcrChannels(BMP *image, DBMATRIX *y, DBMATRIX *cb, DBMATRIX *cr) {
    if(image == NULL)
        return;

    // Variáveis auxiliares para percorrer a matriz de pixeis
    int width = image->iHeader.bmpWidth;
    int heigth = image->iHeader.bmpHeight;
    PIXELYCBCR px;

    // Iterando pela matriz de pixeis para realizar a conversão
    for(int i = 0; i < heigth; i++) {
        for(int j = 0; j < width; j++) {
            px = pixelConvertRgbToYcbcr(&image->data[i][j]);
            y->matrix[i][j] = px.y;
            cb->matrix[i][j] = px.cb;
            cr->matrix[i][j] = px.cr;
        }
    }

    return;
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

// Retorna a largura da imagem
int bmpGetWidth(BMP *image) {
    if(image == NULL)
        return -1;

    return image->iHeader.bmpWidth;
}

// Retorna a altura da imagem
int bmpGetHeigth(BMP *image) {
    if(image == NULL)
        return -1;

    return image->iHeader.bmpHeight;
}

// Retorna o infoHeader interno
BIHEADER bmpGetInfoHeader(BMP *image) {
    return image->iHeader;   
}

// Retorna o file header interno
BFHEADER bmpGetFileHeader(BMP *image) {
    return image->fHeader;
}

bool bmpSetPixel(BMP *image, int i, int j, pixelRgb newPx) {
    if(image == NULL)
        return false;

    if(i > image->iHeader.bmpHeight || j > image->iHeader.bmpWidth)
        return false;
        
    image->data[i][j] = newPx;
    return true;
}

// Permite o usuário imprimir o bitmap e os cabeçalhos do arquivo
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