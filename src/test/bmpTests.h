#ifndef BMPTESTS_H
    #define BMPTESTS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #include "../primitives/util.h"
    #include "../image/pixel.h"
    #include "../image/bfHeader.h"
    #include "../image/bfInfoHeader.h"
    #include "../fileManager/fileManager.h"
    #include "../image/bmp.h"

    // Testes para as funções de IO dos arquivos bmp e seus TAD's
    // Internos

    void testBfHeader(char *bmp);
    void testIHeader(char *bmp);
    void testPixel();
    void testFileManager(char *bmpFile);

#endif