#include "pixel.h"

// Converte um pixel rgb para yCbCr.
// CUIDADO: Isto leva a perda na informação do canal de cor g!
PIXELYCBCR pixelConvertRgbToYcbcr(PIXELRGB *pixel) {
    if(pixel == NULL) {
        PIXELYCBCR errorPixel = {-1, -1, -1};
        return errorPixel;
    }

    PIXELYCBCR newPixel;

    // Aplicando a fórmula de conversão para todos os campos (menos cg)
    newPixel.y = (0.299 * pixel->r) + (0.5876 * pixel->g) + (0.114 * pixel->b);
    newPixel.cb = 0.564 * (pixel->b - newPixel.y);
    newPixel.cr = 0.713 * (pixel->r - newPixel.y);

    return newPixel;
}

// Converte um pixel de yCbCr de volta para rgb.
PIXELRGB pixelConvertYcbcrToRgb(PIXELYCBCR *pixel) {
    if(pixel == NULL) {
        PIXELRGB errorPixel = {-1, -1, -1};
        return errorPixel;
    }

    // Aplicando a fórmula de conversão para todos os campos, e convertendo
    // para unsigned char.
    PIXELRGB newPixel;
    newPixel.r = (unsigned char) (pixel->y + (1.402 * pixel->cr));
    newPixel.g = (unsigned char) (pixel->y - (0.344 * pixel->cb) - (0.714 * pixel->cr));
    newPixel.b = (unsigned char) (pixel->y + (1.772 * pixel->cb)); 

    return newPixel;
}

// Imprime um pixel no formato rgb. Função compatível com a
// interface genérica.
void pixelRgbPrint(const void *val) {
    PIXELRGB *ptr = (PIXELRGB *) val;
    printf("(%d, %d, %d)", ptr->r, ptr->g, ptr->b);
}

// Imprime um pixel no formato yCbCr. Função compatível com a
// interface genérica.
void pixelYcbcrPrint(const void *val) {
    PIXELYCBCR *ptr = (PIXELYCBCR *)val;
    printf("(%lf, %lf, %lf)", ptr->y, ptr->cb, ptr->cr);
}