#ifndef FILEMANAGER_H
    #define FILEMANAGER_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #include "bfHeader.h"
    #include "bfInfoHeader.h"
    #include "bmp.h"

    #include "../primitives/vector.h"
    #include "../primitives/pixel.h"
    #include "../primitives/util.h"
    
    // Funções de I/O para arquivos
    BMP *loadImage(char *bmpName);
    bool writeNewImage(char *bmpName, BMP *newImage);

    //bool writeJPEGImage()

    // Funções de compressão / descompressão
    //bool compress(BMP *bmp);
    //bool decompress();

#endif