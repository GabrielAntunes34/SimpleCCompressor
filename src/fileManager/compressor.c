#include "compressor.h"

// Função auxiliar que recebe um canal da imagem que já sofreu
// o downsampling e realiza o loop de compressão em cada bloco
// Retornando um bitBuffer com os dados comprimidos
void compressLoop(DBMATRIX *channel, BITBUFFER *cmpData, int blkQntdy, int quantTable) {
    // Comprimindo cada bloco do canal
    for(int i = 0; i < blkQntdy; i++) {
        double block[8][8];
        int auxBlock[8][8];

        // codificação com perda
        dbMatrixGetBlock(&channel, BLK_SIZE, i, block);
        dct(BLK_SIZE, block, true);
        quantize(BLK_SIZE, block, auxBlock, LUM_QNT_TBL);

        /*
        COISAS PARA COLOCAR NO DECOMPRESS LOOP
        dequantize(BLK_SIZE, auxBlock, block, LUM_QNT_TBL);
        inverseDct(BLK_SIZE, block, true);
        dbMatrixSetBlock(&channel, BLK_SIZE, i, block);
        */
    }
}

DBMATRIX *decompressLoop() {

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
    cmpHeader.cromPaddingHeigth = getSample420Padding(channelCb.lines, BLK_SIZE);
    cmpHeader.cromPaddingHeigth = getSample420Padding(channelCb.lines, BLK_SIZE);

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
    BITBUFFER *cmpData = bitBufferCreate(2 * channelY.cols * channelY.lines);
    compressLoop(&channelY, cmpData, cmpHeader.yBlocks, LUM_QNT_TBL);
    compressLoop(&spCb, cmpData, cmpHeader.cbBlocks, CROM_QNT_TBL);
    compressLoop(&spCr, cmpData, cmpHeader.crBlocks, CROM_QNT_TBL);

    // Escrevendo o binário final
    bool checkWrite;
    checkWrite = writeCmpFile(exitBin, bmpGetInfoHeader(bmp), bmpGetFileHeader(bmp), cmpHeader, cmpData);
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

bool decompress(char *entryBin, char *exitBmp) {
    //====================
    // Leitura do Bin
    //====================

    /*
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
    */
}