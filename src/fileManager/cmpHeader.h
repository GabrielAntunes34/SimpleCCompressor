#ifndef CMPHEADER_H
    #define CMPHEADER_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #define CMPHEADER_OFFSET 54

    // Cabeçalho extra para o arquivo comprimido
    typedef struct cmpHeader {
        int yBlocks;               // Número de blocos em y
        int cbBlocks;              // Número de blocos em cb
        int crBlocks;              // Número de blocos em cr
        int cromPdWidth;           // Padding na largura de cb e cr
        int cromPdHeigth;          // Padding na altura de cb e cr
        int yCmpBytes;             // Número de bytes comprimidos final de Y
        int cbCmpBytes;            // Número de bytes comprimidos final de cb
        int crCmpBytes;            // Número de bytes comprimidos final de cr
    } CMPHEADER;

    bool cmpHeaderRead(CMPHEADER *header, FILE *filePtr);
    bool cmpHeaderWrite(CMPHEADER *header, FILE *filePtr);
    bool cmpHeaderPrint(CMPHEADER *header);

#endif