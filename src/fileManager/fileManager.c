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

bool compress2(BMP *bmp) {
    int width = bmpGetWidth(bmp);
    int heigth = bmpGetHeigth(bmp);

    DBMATRIX channelY  = dbMatrixCreate(width, heigth);
    DBMATRIX channelCb = dbMatrixCreate(width, heigth);
    DBMATRIX channelCr = dbMatrixCreate(width, heigth);

    //bmpGetYcbCrData(bmp, &channelY, &channelCb, &channelCr);

    // Downsampling each channel
    //DBMATRIX spChannelCb = downSample4202(&channelCb);
    //DBMATRIX spChannelCr = downSample4202(&channelCr);
    dbMatrixDestroy(&channelCb);
    dbMatrixDestroy(&channelCr);

    // Obtendo a quantidade de blocos de dados
    int yBlocks = dbMatrixBlockQntd(&channelY, BLK_SIZE);
    //int cbBlocks = dbMatrixBlockQntd(&spChannelCb, BLK_SIZE);
    //int crBlocks = dbMatrixBlockQntd(&spChannelCr, BLK_SIZE);

    // Loop de compressão dos blocos y
    for(int i = 0; i < yBlocks; i++) {
        double block[8][8];
        
        dbMatrixGetBlock(&channelY, BLK_SIZE, i, block);
        //dct2(block);
        //printBlock();
    }
    return false;
}

/*
bool compress(BMP *bmp) {
    PIXELYCBCR **ycbcrMat;

    // Alocando os vetores
    VECTOR *yBlocks = vectorCreateAs(double, NULL);
    VECTOR *cbBlocks = vectorCreateAs(double, NULL);
    VECTOR *crBlocks = vectorCreateAs(double, NULL);

    // Preparando a imagem para a dct
    ycbcrMat = bmpGetYcbcrData(bmp);
    downSample420(&ycbcrMat, bmpGetWidth(bmp), bmpGetHeigth(bmp));
    prepareBlocks(&ycbcrMat, bmpGetWidth(bmp), bmpGetHeigth(bmp), yBlocks, cbBlocks, crBlocks, true);

    // Liberando a matriz ycbcr
    for(int i = 0; i < bmpGetHeigth(bmp); i++) {
        free(ycbcrMat[i]);
        ycbcrMat[i] = NULL;
    }
    free(ycbcrMat);
    ycbcrMat = NULL;

    // Libreando os vectors
    vectorDestroy(&yBlocks);
    vectorDestroy(&crBlocks);
    vectorDestroy(&cbBlocks);
    return false;
}
*/

// Descomprime uma imagem JPEG para um bmp
bool decompress() {
    return false;
}
