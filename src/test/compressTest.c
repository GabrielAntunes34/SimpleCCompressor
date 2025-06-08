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
    PIXELYCBCR **ycbcrMat;
    PIXELRGB px;
    BMP *bmp;

    // Obtendo os pixeis em ycbcr
    bmp = loadBmpImage(bmpEntry);
    ycbcrMat = bmpGetYcbcrData(bmp);

    // Aplicando o downsampling
    downSample420(&ycbcrMat, bmpGetWidth(bmp), bmpGetHeigth(bmp));
    upSample420(&ycbcrMat, bmpGetWidth(bmp), bmpGetHeigth(bmp));

    // Reconvertendo para rgb e salvando no bmp
    for(int i = 0; i < bmpGetHeigth(bmp); i++) {
        for(int j = 0; j < bmpGetWidth(bmp); j++) {
            // Verificando se toda a conversão se manteve no range [0, 255]
            //if(ycbcrMat[i][j].cb < 0 || ycbcrMat[i][j].cr < 0 || ycbcrMat[i][j].y < 0) {
            //    pixelYcbcrPrint(&ycbcrMat[i][j]);
            //}

            px = pixelConvertYcbcrToRgb(&ycbcrMat[i][j]);
            bmpSetPixel(bmp, i, j, px);
        }
    }

    // Reescrevendo em um novo bmp
    writeBmpImage(bmpExit, bmp);

    // Apagando a memória dinamicamente alocada
    bmpDestroy(&bmp);
    // Liberando a matriz ycbcr
    for(int i = 0; i < bmpGetHeigth(bmp); i++) {
        free(ycbcrMat[i]);
        ycbcrMat[i] = NULL;
    }
    free(ycbcrMat);
    ycbcrMat = NULL;
}

void testBlocks(char *bmpEntry) {
    PIXELYCBCR **ycbcrMat;
    VECTOR *yBlocks;
    VECTOR *crBlocks;
    VECTOR *cbBlocks;

    // Obtendo os pixeis em ycbcr
    BMP *bmp = loadBmpImage(bmpEntry);
    ycbcrMat = bmpGetYcbcrData(bmp);

    // Obtendo os blocos da imagem
    ycbcrMat = bmpGetYcbcrData(bmp);
    prepareBlocks(&ycbcrMat, 16, 16, yBlocks, cbBlocks, crBlocks);

    printf("Size: %ld\n", vectorGetSize(yBlocks));

    // Imprimindo o resultado
    printf("Blocos y:\n");
    for(int i = 0; i < vectorGetSize(yBlocks); i++) {
        printf("Aquiiii\n");
        //dctBlock *blk = (dctBlock*) vector_index(yBlocks, i);
        printf("Aquiiiii2\n");
        //dctBlockPrint(*blk);
    }

    // Liberando a matriz ycbcr
    for(int i = 0; i < bmpGetHeigth(bmp); i++) {
        free(ycbcrMat[i]);
        ycbcrMat[i] = NULL;
    }
    free(ycbcrMat);
    ycbcrMat = NULL;

    bmpDestroy(&bmp);
    vectorDestroy(&yBlocks);
    vectorDestroy(&cbBlocks);
    vectorDestroy(&crBlocks);
}