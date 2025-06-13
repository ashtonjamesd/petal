#include <stdio.h>

#include "../include/petal.h"

int main() {
    PetalParser *petal = init_with_file("example/source.x");


    parse(petal);
    petal_inspect(petal);

    petal_free(petal);

    return 0;
}