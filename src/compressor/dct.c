#include "dct.h"

// Decompoes os pixeis de uma matriz ycbcr para vectors de cada canal
// A ordem dos dados em cada vector é a ordem dos blocos 8 por 8
void prepareBlocks(PIXELYCBCR ***mat, int width, int heigth, VECTOR *yBlocks, VECTOR *cbBlocks, VECTOR *crBlocks) {
    if(mat == NULL)
        return;

    // Alocando os vetores
    yBlocks = vectorCreateAs(dctBlock, NULL);
    cbBlocks = vectorCreateAs(dctBlock, NULL);
    crBlocks = vectorCreateAs(dctBlock, NULL);

    // Variáveis auxiliares para os blocos
    dctBlock yBlock;
    dctBlock cbBlock;
    dctBlock crBlock;

    int colOffset = 0;
    int lineOffset = 0;

    printf("Lines: %d, cols: %d\n\n", heigth, width);

    // Iterando pela matriz de pixeis
    while(lineOffset < heigth) {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                
                // Popoulando os blocos y, cb e cr
                yBlock.mat[i][j] = (*mat)[i + lineOffset][j + colOffset].y;
                //cbBlock.mat[i][j] = (*mat)[i + lineOffset][j + colOffset].cb;
                //crBlock.mat[i][j] = (*mat)[i + lineOffset][j + colOffset].cr;
            }
        }

        // Inserindo o suposto bloco no vector
        printf("Aquiiiii\n");
        printf("colOffset: %d, lineOffset: %d\n", colOffset, lineOffset);
        dctBlockPrint(yBlock);
        printf("\n");

        vectorPushBack(yBlocks, &yBlock);
        //vectorPushBack(cbBlocks, &cbBlock);
        //vectorPushBack(crBlocks, &crBlock);

        // Ajustando o offset para o próximo bloco
        colOffset += 8;
        if(colOffset >= width) {
            colOffset = 0;
            lineOffset += 8;
        }
    }

    /*
    // Iterando pela matriz de pixeis
    for(int k = 1; k <= blkQntdy; k ++) {
        for(int i = 0; i < 8; i++) {
            for(int j = 0; j < 8; j++) {
                count++;
                
                // Popoulando os blocos y, cb e cr
                yBlock.mat[i][j] = (*mat)[i*blkQntdy][j*blkQntdy].y;
                cbBlock.mat[i][j] = (*mat)[i*blkQntdy][j*blkQntdy].cb;
                crBlock.mat[i][j] = (*mat)[i*blkQntdy][j*blkQntdy].cr;

                // Inserindo nos vectors, se estiverem prontos
                printf("i: %d, j: %d\n", i, j);
                if(count == 63) {
                    printf("Aquiiiii\n");
                    dctBlockPrint(yBlock);
                    printf("\n");

                    vectorPushBack(yBlocks, &yBlock);
                    vectorPushBack(cbBlocks, &cbBlock);
                    vectorPushBack(crBlocks, &crBlock);

                    count = 0;
                    
                }
            }
        }
    }*/
}

void dctBlockPrint(dctBlock block) {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            printf("%.2lf, ", block.mat[i][j]);
        }
        printf("\n\n");
    }
}