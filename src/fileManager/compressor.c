#include "compressor.h"

// Função auxiliar que recebe um canal da imagem que já sofreu
// o downsampling e realiza o loop de compressão em cada bloco
// Retornando um bitBuffer com os dados comprimidos
void compressLoop(BITBUFFER *cmpData, DBMATRIX *channel, int blkQntdy, int quantTable) {
    for(int i = 0; i < blkQntdy; i++) {
        double block[8][8];
        int qntBlock[8][8];

        // codificação com perda
        dbMatrixGetBlock(channel, BLK_SIZE, i, block);
        dct(BLK_SIZE, block, true);
        quantize(BLK_SIZE, block, qntBlock, quantTable);
        
        // Codificação entrópica
        int *zigZagVet = zigZagNxN(BLK_SIZE, qntBlock);
        RLEPairs rle = runLengthEncoding(zigZagVet, NULL);
        huffman_encoding(cmpData, rle);

        // Desalocando vetores auxiliares
        free(zigZagVet);
        zigZagVet = NULL;
        destroyRLEPairs(&rle);
    }
}

// Realiza a compressão do algorítimo JPEG
int compress(char *entryBmp, char *exitBin) {
    BMP *bmp;
    CMPHEADER cHeader;    // Cabeçalho customizado do arquivo comprimido

    // Obtendo os dados da imagem
    bmp = loadBmpImage(entryBmp);
    if(bmp == NULL)
        return -1.0;
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

    // Calculando o número de bytes do arquivo comprimido
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
        return -1.0;
    
    // Calculando a taxa de compressão
    printf("original: %d", 3 * (width * heigth));
    printf("Novo: %d", cHeader.cmpBytes);

    int compressTax = (3 * width * heigth) / cHeader.cmpBytes;

    // Desalocando a memória restante
    bmpDestroy(&bmp);
    bitBufferDestroy(&cmpData);
    dbMatrixDestroy(&channelY);
    dbMatrixDestroy(&spCb);
    dbMatrixDestroy(&spCr);

    // Retornando a taxa de compressão obtida
    return compressTax;
}

// Função auxiliar que descomprime os bits do binário comprimido
// Nos canais YCbCr da imagem antes de se executar o downsampling
void decompressLoop(DBMATRIX *channel, BITBUFFER *cmpChannel, int *bitPos, int blkQntd, int quantTable) {
    for(int i = 0; i < blkQntd; i++) {
        double blk[8][8];
        int quantBlk[8][8];

        // Decodificando a compressão entrópica
        RLEPairs rle = huffman_decoding(cmpChannel, bitPos);
        int *differencedCode = runLengthDecoding(rle);
        unZigZagNxN(BLK_SIZE, quantBlk, differencedCode);

        // Decodificando a compressão com perda
        dequantize(BLK_SIZE, quantBlk, blk, quantTable);
        inverseDct(BLK_SIZE, blk, true);
        dbMatrixSetBlock(channel, BLK_SIZE, i, blk);

        // Desalocando vetores dinâmicos
        free(differencedCode);
        differencedCode = NULL;
        destroyRLEPairs(&rle);
    }
}

// Realiza a descompressão do algorítimo JPEG
bool decompress(char *entryBin, char *exitBmp) {
    BIHEADER iHeader;
    BFHEADER fHeader;
    CMPHEADER cHeader;
    BITBUFFER *cmpData;

    // Lendo o arquivo
    cmpData = loadCmpFile(entryBin, &iHeader, &fHeader, &cHeader);

    // Recriando as matrizes dos canais originais
    DBMATRIX Y = dbMatrixCreate(iHeader.bmpHeight, iHeader.bmpWidth);
    DBMATRIX spCb = dbMatrixCreate(iHeader.bmpHeight/2 + cHeader.cromPdHeigth, iHeader.bmpWidth/2 + cHeader.cromPdWidth);
    DBMATRIX spCr = dbMatrixCreate(iHeader.bmpHeight/2 + cHeader.cromPdHeigth, iHeader.bmpWidth/2 + cHeader.cromPdWidth);

    // Descomprimindo cada canal
    int bitPosition = 0;
    decompressLoop(&Y, cmpData, &bitPosition, cHeader.yBlocks, LUM_QNT_TBL);
    decompressLoop(&spCb, cmpData, &bitPosition, cHeader.cbBlocks, CROM_QNT_TBL);
    decompressLoop(&spCr, cmpData, &bitPosition, cHeader.crBlocks, CROM_QNT_TBL);

    // Revertendo o downsampling 
    DBMATRIX Cb = upSample420(&spCb, cHeader.cromPdHeigth, cHeader.cromPdWidth);
    DBMATRIX Cr = upSample420(&spCr, cHeader.cromPdHeigth, cHeader.cromPdWidth);
    dbMatrixDestroy(&spCb);
    dbMatrixDestroy(&spCr);

    // Reconvertendo para rgb e salvando no bmp
    BMP *bmp = bmpCreate(iHeader, fHeader, NULL);
    int heigth = iHeader.bmpHeight;
    int width = iHeader.bmpWidth;

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