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
    // Somamos 128 em cb e cr para garantir todas as variaveis no intervalo [0, 255]
    newPixel.y = (double) (0.299 * pixel->r) + (0.587 * pixel->g) + (0.114 * pixel->b);
    newPixel.cb = (double) 0.564 * (pixel->b - newPixel.y) + 128;
    newPixel.cr = (double) 0.713 * (pixel->r - newPixel.y) + 128;

    return newPixel;
}

// Converte um pixel de yCbCr de volta para rgb.
PIXELRGB pixelConvertYcbcrToRgb(PIXELYCBCR *pixel) {
    if(pixel == NULL) {
        PIXELRGB errorPixel = {-1, -1, -1};
        return errorPixel;
    }

    // Retirando o padding de 128
    pixel->cb -= 128;
    pixel->cr -= 128;

    // Aplicando a fórmula de conversão para todos os campos, e convertendo
    // para unsigned char.
    PIXELRGB newPixel;

    double r = (pixel->y + (1.402 * pixel->cr));
    double g = (pixel->y - (0.344 * pixel->cb) - (0.714 * pixel->cr));
    double b = (pixel->y + (1.772 * pixel->cb));

    // Adequando os erros de arredondamento para [0, 255]
    r = min(255.0, max(0.0, r));
    g = min(255.0, max(0.0, g));
    b = min(255.0, max(0.0, b));

    // Aplicando a conversão de tipo
    newPixel.r = (unsigned char) r;
    newPixel.g = (unsigned char) g;
    newPixel.b = (unsigned char) b;

    return newPixel;
}

// Lê um pixel em um arquivo já aberto no formato RGB
// Estamos considerando a formatação de arquivos BMP!
PIXELRGB pixelRgbRead(FILE *pf) {
    PIXELRGB pixel;

    fread(&pixel.b, sizeof(unsigned char), 1, pf);
    fread(&pixel.g, sizeof(unsigned char), 1, pf);
    fread(&pixel.r, sizeof(unsigned char), 1, pf);

    return pixel;
}

// Lê de um pixel em um arquivo já aberto no formatro YCBCR
PIXELYCBCR pixelYcbcrRead(FILE *pf) {
    PIXELRGB pixel;
    PIXELYCBCR pixelY;

    pixel = pixelRgbRead(pf);
    pixelY = pixelConvertRgbToYcbcr(&pixel);
    return pixelY;
}

// Escreve um pixel em um arquivo já aberto no formato RGB
void pixelRgbWrite(PIXELRGB *pixel, FILE *pf) {
    fwrite(&pixel->b, sizeof(unsigned char), 1, pf);
    fwrite(&pixel->g, sizeof(unsigned char), 1, pf);
    fwrite(&pixel->r, sizeof(unsigned char), 1, pf);
}

// Escreve um pixel em um arquivo já aberto no formato YCBCR
void pixelYcbcrWrite(PIXELYCBCR *pixel, FILE *pf) {
    fwrite(&pixel->y, sizeof(double), 1, pf);
    fwrite(&pixel->cb, sizeof(double), 1, pf);
    fwrite(&pixel->cr, sizeof(double), 1, pf);
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