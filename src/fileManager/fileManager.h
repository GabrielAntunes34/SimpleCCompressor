#ifndef FILEMANAGER_H
    #define FILEMANAGER_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>
    #include "bfHeader.h"
    #include "bfInfoHeader.h"

    #include "../primitives/vector.h"
    #include "../primitives/pixel.h"
    #include "../primitives/util.h"
    
    // Funções de I/O para arquivos
    VECTOR *loadBmpImage(char *bmpName, BFHEADER *fHeader, BIHEADER *iHeader, bool asYcbcr);
    //bool writeBmpImage(FILE *bmpPtr, BFHEADER *fHeader, BIHEADER *iHeader, VECTOR *imgData);
    //void printBmpData();
    //void printHeaders(BFHEADER *fHeader, BIHEADER *iHeader);
    //bool writeJPEGImage()

    // Funções de compressão / descompressão

    // Manipulações das imagens
    //bool convertRgbToYcbcr();

#endif