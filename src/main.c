#include <stdio.h>
#include <stdlib.h>
#include "primitives/bitBuffer.h"
#include "primitives/vector.h"
#include "primitives/util.h"

int main() {
    VECTOR* vec = vectorCreateAs(int, NULL);
    int i;

    i = 2;
    vectorPushBack(vec, &i);
    i = 5;
    vectorPushBack(vec, &i);
    i = 1;
    vectorPushBack(vec, &i);
    i = 7;
    vectorPushBack(vec, &i);
    i = 9;
    vectorPushBack(vec, &i);

    vectorPrintAs(vec, int);

    vectorDestroy(&vec);
    return 0;
}