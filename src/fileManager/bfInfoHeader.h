#ifndef BINFOHEADER_H
    #define BINFOHEADER_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    // CONSTANTES PARA CARACTERÍSTICAS DO ARQUIVO
    // COMPRESSÃO
    #define BI_RGB 0  // Sem codificação
    #define BI_RLE8 1 // Codificação RLE de 8 bits
    #define BI_RLE4 2 // Codificação RLE de 4 bits 

    // Esquema de cores do bitCount
    #define RGB_24_bit 24
    #define MONOCHROME 1
    #define RGB_16_BIT 16

    #define INFO_HEADER_OFFSET 14

    typedef struct bInfoHeader {
        unsigned int size;         // Tamanho do infoHeader
        int bmpWidth;              // Largura da imagem (em pixels)
        int bmpHeight;             // Altura da imagem (em pixels)
        unsigned short planes;     // Quantidade de color planes
        unsigned short bitCount;   // Tamanho em bits de cada pixel
        unsigned int compression;  // Tipo de compressão a ser utilizada
        unsigned int imageSize;    // Quantidade de dados da imagem
        int XPixelsPerMeter;       // Pixels por metro em X
        int YPixelsPerMeter;       // Pixels por metro em Y
        unsigned int clrUsed;      // Quantidade de cores utilizada
        unsigned int clrImportant; // Quantidade de cores importantes
    }BINFOHEADER;

    BINFOHEADER *bInfoHeaderCreate();
    
    bool bInfoHeaderRead(BINFOHEADER *header, FILE *bfPtr);
    bool bInfoHeaderWrite(BINFOHEADER *header, FILE *bfPtr);

    void bInfoHeaderDestroy(BINFOHEADER **header);
#endif