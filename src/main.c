#include <stdio.h>

#include "../include/petal.h"

void brainfuck_example() {
    PetalParser *petal = init_with_string("+--++++<<>>..,[][],<>>");
    WITH_OPERATORS(petal,
        "+", "-", "<", ">", ".", ",", "[", "]"
    );

    parse(petal);
    petal_inspect(petal);

    petal_free(petal);
}

int main() {
    brainfuck_example();

    return 0;
}