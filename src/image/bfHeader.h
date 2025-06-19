#ifndef BFHEADER_H
    #define BFHEADER_H
    #include<stdio.h>
    #include<stdlib.h>
    #include<stdbool.h>

    #define BF_MAGIC 0x4D42

    // Cabeçalho do arquivo BMP
    typedef struct bfHeader {
        unsigned short magic;      // Magic number
        unsigned int fileSize;        // tamanho do arquivo
        unsigned short reserved1; 
        unsigned short reserved2;
        unsigned int bmpOffset;     // Offset do bitmap
    } BFHEADER;

    bool bfHeaderIsValid(BFHEADER *header);
    bool bfHeaderRead(BFHEADER *header, FILE *bfPtr);
    bool bfHeaderWrite(BFHEADER *header, FILE *bfPtr);

    void bfHeaderPrint(BFHEADER *header);

#endif