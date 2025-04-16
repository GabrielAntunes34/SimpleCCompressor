#ifndef PIXEL_H
    #define PIXEL_H
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdbool.h>

    // Define um tipo pixel para o formato rgb (usual das imagens
    // bmp)
    typedef struct pixelRgb {
        unsigned char r;
        unsigned char g;
        unsigned char b;
    }PIXELRGB;
    typedef struct pixelRgb pixelRgb;

    // Define um pixel para o formato YCbCr (utilizado na compressão
    // e na aplicação da TCT)
    typedef struct pixelYcbcr {
        double y;
        double cb;
        double cr;
    }PIXELYCBCR;
    typedef struct pixelYcbcr pixelYcbcr;

    PIXELYCBCR pixelConvertRgbToYcbcr(PIXELRGB *pixel);
    PIXELRGB pixelConvertYcbcrToRgb(PIXELYCBCR *pixel);

    void pixelRgbPrint(const void *val);
    void pixelYcbcrPrint(const void *val);

#endif