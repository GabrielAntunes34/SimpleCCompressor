#include <stdio.h>
#include <stdlib.h>
#include "primitives/bitBuffer.h"
#include "primitives/vector.h"
#include "primitives/util.h"

#include "fileManager/bfHeader.h"
#include "fileManager/bfInfoHeader.h"

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

int main() {
    char *bmp1 = "test/testImg.bmp";
    char *bmp2 = "test/testImg2.bmp";


    testIHeader(bmp2);

    return 0;
}