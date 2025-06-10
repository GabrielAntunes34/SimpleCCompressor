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


    // Aplicando e revertendo o downsampling
    DBMATRIX spCb = downSample420(&channelCb);
    DBMATRIX spCr = downSample420(&channelCr);
    dbMatrixDestroy(&channelCb);
    dbMatrixDestroy(&channelCr);
    
    DBMATRIX newCb = upSample420(&spCb);
    DBMATRIX newCr = upSample420(&spCr);
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