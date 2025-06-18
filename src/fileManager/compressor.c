#include "compressor.h"

// Função auxiliar que recebe um canal da imagem que já sofreu
// o downsampling e realiza o loop de compressão em cada bloco
// Retornando um bitBuffer com os dados comprimidos
BITBUFFER *compressLoop(DBMATRIX *channel, int blkQntdy, int quantTable) {
    BITBUFFER *cmpChannel = bitBufferCreate(channel->lines * channel->cols);

    // Comprimindo cada bloco do canal
    for(int i = 0; i < blkQntdy; i++) {
        double block[8][8];
        int qntBlock[8][8];

        // codificação com perda
        dbMatrixGetBlock(channel, BLK_SIZE, i, block);
        dct(BLK_SIZE, block, true);
        quantize(BLK_SIZE, block, qntBlock, LUM_QNT_TBL);
        
        // Codificação entrópica
        int *zigZagVet = zigZagNxN(BLK_SIZE, qntBlock);
        int *zigZagDiff =  zigZagDifference(zigZagVet);
        RLEPairs rle = runLengthEncoding(zigZagDiff, NULL);
        huffman_encoding(cmpChannel, rle);

        // Desalocando vetores auxiliares
        free(zigZagVet);
        zigZagVet = NULL;
        free(zigZagDiff);
        zigZagDiff = NULL;
        free(rle);      // EU SEI... TA VAZANDO OS PARES INTERNOS AQUI
    }

    return cmpChannel;
}

bool compress(char *entryBmp, char *exitBin) {
    BMP *bmp;
    CMPHEADER cmpHeader;    // Cabeçalho customizado do arquivo comprimido

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
    cmpHeader.cromPdHeigth = getSample420Padding(channelCb.lines, BLK_SIZE);
    cmpHeader.cromPdHeigth = getSample420Padding(channelCb.lines, BLK_SIZE);

    // Aplicando o downsampling
    DBMATRIX spCb = downSample420(&channelCb, BLK_SIZE);
    DBMATRIX spCr = downSample420(&channelCr, BLK_SIZE);
    dbMatrixDestroy(&channelCb);
    dbMatrixDestroy(&channelCr);
    
    // Obtendo a quantidade de blocos de dados
    cmpHeader.yBlocks = dbMatrixGetBlockQntd(&channelY, BLK_SIZE);
    cmpHeader.cbBlocks = dbMatrixGetBlockQntd(&spCb, BLK_SIZE);
    cmpHeader.crBlocks = dbMatrixGetBlockQntd(&spCr, BLK_SIZE);

    // Comprimindo cada canal
    BITBUFFER *cmpY = compressLoop(&channelY, cmpHeader.yBlocks, LUM_QNT_TBL);
    BITBUFFER *cmpCb = compressLoop(&spCb, cmpHeader.cbBlocks, CROM_QNT_TBL);
    BITBUFFER *cmpCr = compressLoop(&spCr, cmpHeader.crBlocks, CROM_QNT_TBL);

    // Adcionando a quantidade de bytes de cada canal no cabeçalho
    cmpHeader.yCmpBytes = bitBufferGetOccupiedBits(cmpY);
    cmpHeader.cbCmpBytes = bitBufferGetOccupiedBits(cmpCb);
    cmpHeader.crCmpBytes = bitBufferGetOccupiedBits(cmpCr);

    // Escrevendo o binário final
    bool checkWrite;
    checkWrite = writeCmpFile(exitBin, bmpGetInfoHeader(bmp), bmpGetFileHeader(bmp), cmpHeader, cmpY, cmpCb, cmpCr);
    if(!checkWrite)
        return false;
    
    // Desalocando a memória restante
    bmpDestroy(&bmp);
    bitBufferDestroy(&cmpY);
    bitBufferDestroy(&cmpCb);
    bitBufferDestroy(&cmpCr);
    dbMatrixDestroy(&channelY);
    dbMatrixDestroy(&spCb);
    dbMatrixDestroy(&spCr);

    return true;
}

void decompressLoop(DBMATRIX *channel, BITBUFFER *cmpChannel, int blkQntd, int quantTable) {
    // Descomprimindo cada bloco do canal
    for(int i = 0; i < blkQntd; i++) {
        double blk[8][8];
        int quantBlk[8][8];

        // Decodificando a compressão entrópica
        RLEPairs rle = huffman_decoding(cmpChannel);
        int *differencedCode = runLengthDecoding(rle);
        int *zigZagVet = unZigZagDifference(differencedCode);
        unZigZagNxN(BLK_SIZE, quantBlk, zigZagVet);

        // Decodificando a compressão com perda
        dequantize(BLK_SIZE, quantBlk, blk, LUM_QNT_TBL);
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
    CMPHEADER cmpHeader;

    // Abrindo o arquivo aqui por enquanto
    FILE *binPtr = fopen(entryBin, "rb");
    if(binPtr == NULL) {
        printf("Error\n");
        return false;
    }

    // lendo os cabeçalhos doidos
    bfHeaderRead(&fHeader, binPtr);
    biHeaderRead(&iHeader, binPtr);
    cmpHeaderRead(&cmpHeader, binPtr);

    // Lendo cada canal comprimido
    unsigned char aux;

    printf("Aquiiii\n");

    BITBUFFER *cmpY = bitBufferCreate(cmpHeader.yCmpBytes);
    for(int i = 0; i < cmpHeader.yCmpBytes; i++) {
        fread(&aux, sizeof(unsigned char), 1, binPtr);
        bitBufferInsertChar(cmpY, aux);
    }

    BITBUFFER *cmpCb = bitBufferCreate(cmpHeader.cbCmpBytes);
    for(int i = 0; i < cmpHeader.cbCmpBytes; i++) {
        fread(&aux, sizeof(unsigned char), 1, binPtr);
        bitBufferInsertChar(cmpCb, aux);
    }
    BITBUFFER *cmpCr = bitBufferCreate(cmpHeader.crCmpBytes);
    for(int i = 0; i < cmpHeader.crCmpBytes; i++) {
        fread(&aux, sizeof(unsigned char), 1, binPtr);
        bitBufferInsertChar(cmpCr, aux);
    }

    // Fechando o arquivo
    fclose(binPtr);

    printf("Banana\n");

    DBMATRIX Y = dbMatrixCreate(iHeader.bmpHeight, iHeader.bmpWidth);
    DBMATRIX spCb = dbMatrixCreate(iHeader.bmpHeight/2 + cmpHeader.cromPdHeigth, iHeader.bmpWidth/2 + cmpHeader.cromPdWidth);
    DBMATRIX spCr = dbMatrixCreate(iHeader.bmpHeight/2 + cmpHeader.cromPdHeigth, iHeader.bmpWidth/2 + cmpHeader.cromPdWidth);

    // Descomprimindo cada canal
    decompressLoop(&Y, cmpY, cmpHeader.yBlocks, LUM_QNT_TBL);
    decompressLoop(&spCb, cmpCb, cmpHeader.cbBlocks, CROM_QNT_TBL);
    decompressLoop(&spCr, cmpCr, cmpHeader.crBlocks, CROM_QNT_TBL);

    printf("Test 2 \n");

    // Revertendo o downsampling 
    DBMATRIX Cb = upSample420(&spCb, cmpHeader.cromPdHeigth, cmpHeader.cromPdWidth);
    DBMATRIX Cr = upSample420(&spCr, cmpHeader.cromPdHeigth, cmpHeader.cromPdWidth);
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
            printf("%d %d\n", i, j);

            px.y = Y.matrix[i][j];
            printf("Teste");

            px.cb = Cb.matrix[i][j];
            printf("Teste");

            px.cr = Cr.matrix[i][j];
            printf("Teste");

            printf("Abra\n");

            rgbPx = pixelConvertYcbcrToRgb(&px);
            printf("Kadabra\n");
            bmpSetPixel(bmp, i, j, rgbPx);
        }
    }

    printf("Test5\n");

    // Reescrevendo em um novo bmp
    writeBmpImage(exitBmp, bmp);

    // Apagando a memória dinamicamente alocada
    bmpDestroy(&bmp);
    bitBufferDestroy(&cmpY);
    bitBufferDestroy(&cmpCb);
    bitBufferDestroy(&cmpCr);
    dbMatrixDestroy(&Y);
    dbMatrixDestroy(&Cb);
    dbMatrixDestroy(&Cr);

    return true;
}