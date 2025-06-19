#include "compressor.h"

void printDblMat(int size, double mat[size][size]) {
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            printf("%.2lf, ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printIntMat(int size, int mat[size][size]) {
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            printf("%d, ", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void printIntVet(int size, int *vet) {
    for(int i = 0; i < size; i++) {
        printf("%d, ", vet[i]);
    }
    printf("\n");
}

// Função auxiliar que recebe um canal da imagem que já sofreu
// o downsampling e realiza o loop de compressão em cada bloco
// Retornando um bitBuffer com os dados comprimidos
void compressLoop(BITBUFFER *cmpData, DBMATRIX *channel, int blkQntdy, int quantTable) {
    for(int i = 0; i < blkQntdy; i++) {
        double block[8][8];
        int qntBlock[8][8];

        // codificação com perda
        dbMatrixGetBlock(channel, BLK_SIZE, i, block);
        //printDblMat(8, block);

        dct(BLK_SIZE, block, true);
        //printDblMat(8, block);

        quantize(BLK_SIZE, block, qntBlock, quantTable);
        //printIntMat(8, qntBlock);
        
        // Codificação entrópica
        int *zigZagVet = zigZagNxN(BLK_SIZE, qntBlock);
        //printIntVet(64, zigZagVet);
        //int *zigZagDiff =  zigZagDifference(zigZagVet);
        RLEPairs rle = runLengthEncoding(zigZagVet, NULL);
        //printf("Huffman:\n");
        huffman_encoding(cmpData, rle);


        // Desalocando vetores auxiliares
        free(zigZagVet);
        zigZagVet = NULL;
        //free(zigZagDiff);
        //zigZagDiff = NULL;
        free(rle);      // EU SEI... TA VAZANDO OS PARES INTERNOS AQUI
    }
}

bool compress(char *entryBmp, char *exitBin) {
    BMP *bmp;
    CMPHEADER cHeader;    // Cabeçalho customizado do arquivo comprimido

    // Obtendo os dados da imagem
    bmp = loadBmpImage(entryBmp);
    if(bmp == NULL)
        return false;
    int width = bmpGetWidth(bmp);
    int heigth = bmpGetHeigth(bmp);

    // Obtendo os dados ycbcr
    DBMATRIX channelY  = dbMatrixCreate(heigth, width);
    DBMATRIX channelCb = dbMatrixCreate(heigth, width);
    DBMATRIX channelCr = dbMatrixCreate(heigth, width);
    bmpGetYcbcrChannels(bmp, &channelY, &channelCb, &channelCr);

    // Calculando o padding dos canais de croôminancia
    cHeader.cromPdHeigth = getSample420Padding(channelCb.lines, BLK_SIZE);
    cHeader.cromPdWidth = getSample420Padding(channelCb.cols, BLK_SIZE);

    // Aplicando o downsampling
    DBMATRIX spCb = downSample420(&channelCb, BLK_SIZE);
    DBMATRIX spCr = downSample420(&channelCr, BLK_SIZE);
    dbMatrixDestroy(&channelCb);
    dbMatrixDestroy(&channelCr);
    
    // Obtendo a quantidade de blocos de dados
    cHeader.yBlocks = dbMatrixGetBlockQntd(&channelY, BLK_SIZE);
    cHeader.cbBlocks = dbMatrixGetBlockQntd(&spCb, BLK_SIZE);
    cHeader.crBlocks = dbMatrixGetBlockQntd(&spCr, BLK_SIZE);

    // Comprimindo cada canal
    BITBUFFER *cmpData = bitBufferCreate(channelY.lines * channelY.cols);
    compressLoop(cmpData, &channelY, cHeader.yBlocks, LUM_QNT_TBL);
    compressLoop(cmpData, &spCb, cHeader.cbBlocks, CROM_QNT_TBL);
    compressLoop(cmpData, &spCr, cHeader.crBlocks, CROM_QNT_TBL);

    printf("Occupied: %d\n", bitBufferGetOccupiedBits(cmpData));
    int bits = bitBufferGetOccupiedBits(cmpData);
    if(bits % 8 != 0)
        bits = bits / 8 + 1;
    else
        bits = bits / 8;
    cHeader.cmpBytes = bits;

    // Escrevendo o binário final
    bool checkWrite;
    checkWrite = writeCmpFile(exitBin, bmpGetInfoHeader(bmp), bmpGetFileHeader(bmp), cHeader, cmpData);
    if(!checkWrite)
        return false;
    
    // Desalocando a memória restante
    bmpDestroy(&bmp);
    bitBufferDestroy(&cmpData);
    dbMatrixDestroy(&channelY);
    dbMatrixDestroy(&spCb);
    dbMatrixDestroy(&spCr);

    return true;
}

void decompressLoop(DBMATRIX *channel, BITBUFFER *cmpChannel, int *bitPos, int blkQntd, int quantTable) {
    // Descomprimindo cada bloco do canal
    
    for(int i = 0; i < blkQntd; i++) {
        double blk[8][8];
        int quantBlk[8][8];
        
        printf("pos: %d\n", *bitPos);
        printf("%d: ", i);

        // Decodificando a compressão entrópica
        RLEPairs rle = huffman_decoding(cmpChannel, bitPos);
        int *differencedCode = runLengthDecoding(rle);
        //int *zigZagVet = unZigZagDifference(rle);
        unZigZagNxN(BLK_SIZE, quantBlk, differencedCode);

        /*
        for(int j = 0; j < 8; j++) {
            for(int k = 0; k < 8; j++) {
                printf("%d, ", quantBlk[i][j]);
            }
            printf("\n");
        }
        */

        printf("\n");

        // Decodificando a compressão com perda
        dequantize(BLK_SIZE, quantBlk, blk, quantTable);
        inverseDct(BLK_SIZE, blk, true);
        dbMatrixSetBlock(channel, BLK_SIZE, i, blk);
    }
}

bool decompress(char *entryBin, char *exitBmp) {
    //====================
    // Leitura do Bin
    //====================
    BIHEADER iHeader;
    BFHEADER fHeader;
    CMPHEADER cHeader;

    // Abrindo o arquivo aqui por enquanto
    FILE *binPtr = fopen(entryBin, "rb");
    if(binPtr == NULL) {
        printf("Error\n");
        return false;
    }

    // lendo os cabeçalhos doidos
    bfHeaderRead(&fHeader, binPtr);
    biHeaderRead(&iHeader, binPtr);
    cmpHeaderRead(&cHeader, binPtr);

    // Lendo cada canal comprimido
    char byte = 0;

    BITBUFFER *cmpData = bitBufferCreate(cHeader.yBlocks * cHeader.yBlocks);
    for(int i = 0; i < cHeader.cmpBytes; i++) {
        fread(&byte, sizeof(unsigned char), 1, binPtr);
        bitBufferInsertChar(cmpData, byte);
    }

    printf("Aquiiii4\n");

    // Fechando o arquivo
    fclose(binPtr);

    printf("Banana\n");

    DBMATRIX Y = dbMatrixCreate(iHeader.bmpHeight, iHeader.bmpWidth);
    DBMATRIX spCb = dbMatrixCreate(iHeader.bmpHeight/2 + cHeader.cromPdHeigth, iHeader.bmpWidth/2 + cHeader.cromPdWidth);
    DBMATRIX spCr = dbMatrixCreate(iHeader.bmpHeight/2 + cHeader.cromPdHeigth, iHeader.bmpWidth/2 + cHeader.cromPdWidth);

    // Descomprimindo cada canal
    int bitPosition = 0;
    decompressLoop(&Y, cmpData, &bitPosition, cHeader.yBlocks, LUM_QNT_TBL);
    decompressLoop(&spCb, cmpData, &bitPosition, cHeader.cbBlocks, CROM_QNT_TBL);
    decompressLoop(&spCr, cmpData, &bitPosition, cHeader.crBlocks, CROM_QNT_TBL);

    printf("Test 2 \n");

    // Revertendo o downsampling 
    DBMATRIX Cb = upSample420(&spCb, cHeader.cromPdHeigth, cHeader.cromPdWidth);
    DBMATRIX Cr = upSample420(&spCr, cHeader.cromPdHeigth, cHeader.cromPdWidth);
    dbMatrixDestroy(&spCb);
    dbMatrixDestroy(&spCr);

    printf("Test 3\n");

    // Reconvertendo para rgb e salvando no bmp
    BMP *bmp = bmpCreate(iHeader, fHeader, NULL);

    printf("Test4\n");

    int heigth = iHeader.bmpHeight;
    int width = iHeader.bmpWidth;

    printf("Og: %d\nNew: %d\n", heigth, Cr.lines);

    PIXELYCBCR px;
    PIXELRGB rgbPx;
    for(int i = 0; i < heigth; i++) {
        for(int j = 0; j < width; j++) {
            px.y = Y.matrix[i][j];
            px.cb = Cb.matrix[i][j];
            px.cr = Cr.matrix[i][j];

            rgbPx = pixelConvertYcbcrToRgb(&px);
            bmpSetPixel(bmp, i, j, rgbPx);
        }
    }

    printf("Test5\n");

    // Reescrevendo em um novo bmp
    writeBmpImage(exitBmp, bmp);

    // Apagando a memória dinamicamente alocada
    bmpDestroy(&bmp);
    bitBufferDestroy(&cmpData);
    dbMatrixDestroy(&Y);
    dbMatrixDestroy(&Cb);
    dbMatrixDestroy(&Cr);

    return true;
}

bool testCompressDecompress() {
    return false;
}