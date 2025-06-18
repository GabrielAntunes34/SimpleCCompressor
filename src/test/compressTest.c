#include "compressTest.h"

void testYcbcrCompresion(char *bmpEntry, char *bmpExit) {
    PIXELYCBCR **ycbcrMat;
    BMP *bmp;
    PIXELRGB px;        // Auxiliar para a conversão

    // Obtendo os pixeis em ycbcr
    bmp = loadBmpImage(bmpEntry);
    ycbcrMat = bmpGetYcbcrData(bmp);

    // Reconvertendo para rgb e salvando no bmp
    for(int i = 0; i < bmpGetHeigth(bmp); i++) {
        for(int j = 0; j < bmpGetWidth(bmp); j++) {
            // Verificando se toda a conversão se manteve no range [0, 255]
            if(ycbcrMat[i][j].cb < 0 || ycbcrMat[i][j].cr < 0 || ycbcrMat[i][j].y < 0) {
                pixelYcbcrPrint(&ycbcrMat[i][j]);
                printf("\n");
            }
            if(ycbcrMat[i][j].cb > 255 || ycbcrMat[i][j].cr > 255 || ycbcrMat[i][j].y > 255) {
                pixelYcbcrPrint(&ycbcrMat[i][j]);
                printf("\n");
            }

            px = pixelConvertYcbcrToRgb(&ycbcrMat[i][j]);
            bmpSetPixel(bmp, i, j, px);
        }
    }

    // Reescrevendo em um novo bmp
    writeBmpImage(bmpExit, bmp);
    bmpDestroy(&bmp);

    // Liberando a matriz ycbcr
    for(int i = 0; i < bmpGetHeigth(bmp); i++) {
        free(ycbcrMat[i]);
        ycbcrMat[i] = NULL;
    }
    free(ycbcrMat);
    ycbcrMat = NULL;
}

void testSampler(char *bmpEntry, char *bmpExit) {
    BMP *bmp;

    // Lendo a imagem
    bmp = loadBmpImage(bmpEntry);
    int width = bmpGetWidth(bmp);
    int heigth = bmpGetHeigth(bmp);

    // Obtendo os dados ycbcr
    DBMATRIX channelY  = dbMatrixCreate(heigth, width);
    DBMATRIX channelCb = dbMatrixCreate(heigth, width);
    DBMATRIX channelCr = dbMatrixCreate(heigth, width);
    bmpGetYcbcrChannels(bmp, &channelY, &channelCb, &channelCr);

    // Calculando o padding dos canais
    int pdCbH = getSample420Padding(channelCb.lines, 8);
    int pdCbW = getSample420Padding(channelCb.cols, 8);
    int pdCrH = getSample420Padding(channelCr.lines, 8);
    int pdCrW = getSample420Padding(channelCr.cols, 8);

    // Aplicando e revertendo o downsampling
    DBMATRIX spCb = downSample420(&channelCb, 8);
    DBMATRIX spCr = downSample420(&channelCr, 8);
    dbMatrixDestroy(&channelCb);
    dbMatrixDestroy(&channelCr);
    
    DBMATRIX newCb = upSample420(&spCb, pdCbH, pdCbW);
    DBMATRIX newCr = upSample420(&spCr, pdCrH, pdCrW);
    dbMatrixDestroy(&spCb);
    dbMatrixDestroy(&spCr);

    // Reconvertendo para rgb e salvando no bmp
    PIXELYCBCR px;
    PIXELRGB socorro;
    for(int i = 0; i < heigth; i++) {
        for(int j = 0; j < width; j++) {
            px.y = channelY.matrix[i][j];
            px.cb = newCb.matrix[i][j];
            px.cr = newCr.matrix[i][j];

            socorro = pixelConvertYcbcrToRgb(&px);
            bmpSetPixel(bmp, i, j, socorro);
        }
    }

    // Reescrevendo em um novo bmp
    writeBmpImage(bmpExit, bmp);

    // Apagando a memória dinamicamente alocada
    bmpDestroy(&bmp);
    dbMatrixDestroy(&channelY);
    dbMatrixDestroy(&newCb);
    dbMatrixDestroy(&newCr);
}

// Aplica a DCT e faz o retorno em uma matriz hardcodada
void testDct() {
        double blk[8][8] = {
        {255.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
        {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,},
    };
    int qtBlk[8][8];

    // Aplicando e mostrando o resultado da DCT
    dct(8, blk, true);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            printf("%.2lf, ", blk[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    // Aplicando e removendo a quantização
    quantize(8, blk, qtBlk, LUM_QNT_TBL);
    dequantize(8, qtBlk, blk, LUM_QNT_TBL);

    // Invertendo a DCT
    inverseDct(8, blk, true);
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            printf("%.2lf, ", blk[i][j]);
        }
        printf("\n");
    }
    return;
}


// Executa toda a compressão com perda e faz a volta,
// recriando a imagem original
void testDctImage(char *bmpEntry, char *bmpExit) {
    BMP *bmp;

    // Lendo a imagem
    bmp = loadBmpImage(bmpEntry);
    int width = bmpGetWidth(bmp);
    int heigth = bmpGetHeigth(bmp);

    // Obtendo os dados ycbcr
    DBMATRIX channelY  = dbMatrixCreate(heigth, width);
    DBMATRIX channelCb = dbMatrixCreate(heigth, width);
    DBMATRIX channelCr = dbMatrixCreate(heigth, width);
    bmpGetYcbcrChannels(bmp, &channelY, &channelCb, &channelCr);

    // Calculando o padding dos canais
    int pdCbH = getSample420Padding(channelCb.lines, 8);
    int pdCbW = getSample420Padding(channelCb.cols, 8);
    int pdCrH = getSample420Padding(channelCr.lines, 8);
    int pdCrW = getSample420Padding(channelCr.cols, 8);

    // Aplicando o downsampling
    DBMATRIX spCb = downSample420(&channelCb, 8);
    DBMATRIX spCr = downSample420(&channelCr, 8);
    dbMatrixDestroy(&channelCb);
    dbMatrixDestroy(&channelCr);
    
    // Obtendo a quantidade de blocos de dados
    int yBlocks = dbMatrixGetBlockQntd(&channelY, BLK_SIZE);
    int cbBlocks = dbMatrixGetBlockQntd(&spCb, BLK_SIZE);
    int crBlocks = dbMatrixGetBlockQntd(&spCr, BLK_SIZE);

    // TUDO CERTO ATÉ AQUI

    // Loop de compressão dos blocos y
    for(int i = 0; i < yBlocks; i++) {
        double block[8][8];
        int auxBlock[8][8];

        dbMatrixGetBlock(&channelY, BLK_SIZE, i, block);
        dct(BLK_SIZE, block, true);
        quantize(BLK_SIZE, block, auxBlock, LUM_QNT_TBL);

        dequantize(BLK_SIZE, auxBlock, block, LUM_QNT_TBL);
        inverseDct(BLK_SIZE, block, true);
        dbMatrixSetBlock(&channelY, BLK_SIZE, i, block);
    }
    // Loop de compressão dos blocos cb
    for(int i = 0; i < cbBlocks; i++) {
        double block[8][8];
        int auxBlock[8][8];

        dbMatrixGetBlock(&spCb, BLK_SIZE, i, block);
        dct(BLK_SIZE, block, true);
        quantize(BLK_SIZE, block, auxBlock, CROM_QNT_TBL);

        dequantize(BLK_SIZE, auxBlock, block, CROM_QNT_TBL);
        inverseDct(BLK_SIZE, block, true);
        dbMatrixSetBlock(&spCb, BLK_SIZE, i, block);  
    }
    // Loop de compressão dos blocos cr
    for(int i = 0; i < crBlocks; i++) {
        double block[8][8];
        int auxBlock[8][8];

        dbMatrixGetBlock(&spCr, BLK_SIZE, i, block);
        dct(BLK_SIZE, block, true);
        quantize(BLK_SIZE, block, auxBlock, CROM_QNT_TBL);

        dequantize(BLK_SIZE, auxBlock, block, CROM_QNT_TBL);
        inverseDct(BLK_SIZE, block, true);
        dbMatrixSetBlock(&spCr, BLK_SIZE, i, block);
    }

    // Revertendo o downsampling 
    DBMATRIX newCb = upSample420(&spCb, pdCbH, pdCbW);
    DBMATRIX newCr = upSample420(&spCr, pdCrH, pdCrW);
    dbMatrixDestroy(&spCb);
    dbMatrixDestroy(&spCr);

    // Reconvertendo para rgb e salvando no bmp
    PIXELYCBCR px;
    PIXELRGB socorro;
    for(int i = 0; i < heigth; i++) {
        for(int j = 0; j < width; j++) {
            px.y = channelY.matrix[i][j];
            px.cb = newCb.matrix[i][j];
            px.cr = newCr.matrix[i][j];

            socorro = pixelConvertYcbcrToRgb(&px);
            bmpSetPixel(bmp, i, j, socorro);
        }
    }

    // Reescrevendo em um novo bmp
    writeBmpImage(bmpExit, bmp);

    // Apagando a memória dinamicamente alocada
    bmpDestroy(&bmp);
    dbMatrixDestroy(&channelY);
    dbMatrixDestroy(&newCb);
    dbMatrixDestroy(&newCr);
}