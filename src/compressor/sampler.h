#ifndef SAMPLER_H
    #define SAMPLER_H
    #include <stdio.h>
    #include <stdlib.h>

    #include "../primitives/pixel.h"
    #include "../primitives/util.h"

    void downSample420(PIXELYCBCR ***mat, int width, int heigth);
    void upSample420(PIXELYCBCR ***mat, int width, int heigth);

#endif