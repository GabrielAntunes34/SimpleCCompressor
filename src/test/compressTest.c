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
    bmpDestroy(&bmp);
    

}