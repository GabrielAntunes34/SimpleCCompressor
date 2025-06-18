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

bool writeCmpFile(char *binName, BIHEADER iHeader, BFHEADER fHeader, CMPHEADER cmpHeader, BITBUFFER *cmpData) {
    if(binName == NULL || cmpData == NULL)
        return false;
    
    FILE *binPtr;
    bool checkAux;

    // Abrindo o novo arquivo comprimido para a escrita
    binPtr = fopen(binName, "wb");
    if(binPtr) {
        displayError("BMP inválido na memória");
        return false;
    }

    // Escrevendo os cabeçalhos
    bfHeaderWrite(&fHeader, binPtr);
    biHeaderWrite(&iHeader, binPtr);
    cmpHeaderWrite(&cmpHeader, binPtr);

    // escrevendo os dados
    checkAux = bitBufferWrite(cmpData, binPtr);
    if(!checkAux) {
        displayError("Algo de errado ocorreu ao descarregar os dados comprimidos");
        return false;
    }

    fclose(binPtr);
    return true;
}

//==========================
// Interface do compressor
//==========================


// Comprime uma imagem bmp já carregada com o algorítimo JPEG

bool compress(BMP *bmp) {
    int width = bmpGetWidth(bmp);
    int heigth = bmpGetHeigth(bmp);

    DBMATRIX channelY  = dbMatrixCreate(heigth, width);
    DBMATRIX channelCb = dbMatrixCreate(heigth, width);
    DBMATRIX channelCr = dbMatrixCreate(heigth, width);

    bmpGetYcbcrChannels(bmp, &channelY, &channelCb, &channelCr);

    // Calculando o padding dos canais
    int pdCbH = calculateSample420Padding(channelCb.lines, BLK_SIZE);
    int pdCbW = calculateSample420Padding(channelCb.cols, BLK_SIZE);
    int pdCrH = calculateSample420Padding(channelCr.lines, BLK_SIZE);
    int pdCrW = calculateSample420Padding(channelCr.cols, BLK_SIZE);

    // Downsampling de cada canal
    DBMATRIX spChannelCb = downSample420(&channelCb, BLK_SIZE);
    DBMATRIX spChannelCr = downSample420(&channelCr, BLK_SIZE);
    dbMatrixDestroy(&channelCb);
    dbMatrixDestroy(&channelCr);

    // Obtendo a quantidade de blocos de dados
    int yBlocks = dbMatrixGetBlockQntd(&channelY, BLK_SIZE);
    int cbBlocks = dbMatrixGetBlockQntd(&spChannelCb, BLK_SIZE);
    int crBlocks = dbMatrixGetBlockQntd(&spChannelCr, BLK_SIZE);

    // Loop de compressão dos blocos y
    for(int i = 0; i < yBlocks; i++) {
        double block[8][8];
        
        dbMatrixGetBlock(&channelY, BLK_SIZE, i, block);
        dct(BLK_SIZE, block, true);
    }

    return false;
}

// Descomprime uma imagem JPEG para um bmp
bool decompress() {
    return false;
}
