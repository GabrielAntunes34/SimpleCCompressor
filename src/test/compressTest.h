#ifndef COMPRESSTESTS_H
    #define COMPRESSTESTS_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    #include "../primitives/util.h"
    #include "../primitives/pixel.h"
    #include "../fileManager/bfHeader.h"
    #include "../fileManager/bfInfoHeader.h"
    #include "../fileManager/fileManager.h"
    #include "../fileManager/bmp.h"

    // Testes para as funções de IO dos arquivos bmp e seus TAD's
    // Internos

    void testYcbcrCompresion(char *bmpEntry, char *bmpExit);

#endif