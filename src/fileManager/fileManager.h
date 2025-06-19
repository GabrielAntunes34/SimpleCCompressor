#ifndef FILEMANAGER_H
    #define FILEMANAGER_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #include "cmpHeader.h"
    
    #include "../primitives/bitBuffer.h"
    #include "../primitives/vector.h"
    #include "../primitives/matrix.h"
    #include "../primitives/util.h"
   
    #include "../image/bfHeader.h"
    #include "../image/bfInfoHeader.h"
    #include "../image/pixel.h"
    #include "../image/bmp.h"
    
    // Funções de I/O para arquivos
    BMP *loadBmpImage(char *bmpName);
    bool writeBmpImage(char *bmpName, BMP *newImage);

    bitBuffer *loadCmpFile(char *binName, BIHEADER *iHeader, BFHEADER *fHeader, CMPHEADER *cHeader);
    bool writeCmpFile(char *binName, BIHEADER iHeader, BFHEADER fHeader, CMPHEADER cHeader, BITBUFFER *cmpData);
#endif