#ifndef FILEMANAGER_H
    #define FILEMANAGER_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #include "bfHeader.h"
    #include "bfInfoHeader.h"
    #include "cmpHeader.h"
    #include "bmp.h"

    #include "../primitives/bitBuffer.h"
    #include "../primitives/vector.h"
    #include "../primitives/matrix.h"
    #include "../primitives/pixel.h"
    #include "../primitives/util.h"

    #include "../compressor/sampler.h"
    #include "../compressor/dct.h"
    
    // Funções de I/O para arquivos
    BMP *loadBmpImage(char *bmpName);
    bool writeBmpImage(char *bmpName, BMP *newImage);

    bool writeCmpFile(char *binName, BIHEADER iHeader, BFHEADER fHeader,
        CMPHEADER cmpHeader, BITBUFFER *cmpY, BITBUFFER *cmpCb, BITBUFFER *cmpCr);

    // Funções de compressão / descompressão
    //bool compress(BMP *bmp);
    //bool decompress();

#endif