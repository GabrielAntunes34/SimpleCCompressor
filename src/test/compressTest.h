#ifndef COMPRESSTESTS_H
    #define COMPRESSTESTS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #include "../primitives/util.h"
    #include "../image/pixel.h"
    #include "../image/bfHeader.h"
    #include "../image/bfInfoHeader.h"
    #include "../fileManager/fileManager.h"
    #include "../image/bmp.h"

    #include "../compression/sampler.h"
    #include "../compression/dct.h"

    // Testes para as funções de IO dos arquivos bmp e seus TAD's
    // Internos

    void testYcbcrCompresion(char *bmpEntry, char *bmpExit);

    void testSampler(char *bmpEntry, char *bmpExit);
    
    void testDct();
    void testDctImage(char *bmpEntry, char *bmpExit);
#endif