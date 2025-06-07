#include "bmpTests.h"

void testBfHeader(char *bmp) {
    BFHEADER header;
    FILE *pf;

    pf = fopen(bmp, "rb");
    if(pf == NULL) {
        printf("Erro ao abrir arquivo\n");
        return;
    }

    bfHeaderRead(&header, pf);
    fclose(pf);

    bfHeaderPrint(&header);
    printf("Bmp válido?? %d", bfHeaderIsValid(&header));
}

void testIHeader(char *bmp) {
    BIHEADER iHeader;
    FILE *pf;

    pf = fopen(bmp, "rb");
    if(pf == NULL) {
        printf("Erro ao abrir arquivo");
        return;
    }

    biHeaderRead(&iHeader, pf);
    fclose(pf);

    biHeaderPrint(&iHeader);
}

void testPixel() {
    PIXELRGB px = {22, 34, 217};
    PIXELYCBCR pxy;
    VECTOR *pixels = vectorCreateAs(pixelRgb, NULL);
    VECTOR *pixelsy = vectorCreateAs(pixelYcbcr, NULL);

    // Populando um vector com pixels rgb
    vectorPushBack(pixels, &px);
    px.g = 65;
    vectorPushBack(pixels, &px);
    px.r = 88;
    px.b = 70;
    vectorPushBack(pixels, &px);
    px.r = 132;
    px.g = 221;
    px.b = 142;
    vectorPushBack(pixels, &px);
    vectorPrintAs(pixels, pixelRgb);
    printf("\n");
    
    // Convertendo cada elemento para o respectívo yCbCr
    for(int i = 0; i < vectorGetSize(pixels); i++) {
        pxy = pixelConvertRgbToYcbcr(&vectorIndexAs(pixels, pixelRgb, i));
        vectorPushBack(pixelsy, &pxy);
    }
    vectorPrintAs(pixelsy, pixelYcbcr);
    printf("\n");

    // Convertendo de volta para comparação das perdas
    for(int i = 0; i < vectorGetSize(pixelsy); i++) {
        px = pixelConvertYcbcrToRgb(&vectorIndexAs(pixelsy, pixelYcbcr, i));
        vectorSetIndex(pixels, i, &px);
    }
    vectorPrintAs(pixels, pixelRgb);

    // Desalocando tudo e retornando
    vectorDestroy(&pixels);
    vectorDestroy(&pixelsy);
    return;
}

void testFileManager(char *bmpFile) {
    BMP *bmp;
    PIXELYCBCR **dataCompressed;

    bmp = loadImage(bmpFile);
    if(!bmp)
        return;
    
    bmpPrint(bmp, true, false);
    writeNewImage("test.bmp", bmp);
    bmpDestroy(&bmp);
    return;
} 


