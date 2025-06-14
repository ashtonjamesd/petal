#include <stdio.h>

#include "../include/petal.h"
#include "../include/petal_configure.h"

void brainfuck_example() {
    Petal *petal = init_with_string("--+[<>.,]");
    petal_configure_brainfuck(petal);

    parse(petal);
    petal_inspect(petal);

    petal_free(petal);
}

void math_example() {
    Petal *petal = init_with_string("2 + 2.2 * 4^2 / 1.5");
    petal_configure_math(petal);

    parse(petal);
    petal_inspect(petal);

    petal_free(petal);
}

void c_like_example() {
    Petal *petal = init_with_string("int main() { return 0; }");
    with_identifiers(petal);
    with_integers(petal);
    skip_whitespace(petal);

    with_keywords(petal, 
        "int", "main", "return"
    );

    with_symbols(petal, 
        "(", ")", "{", "}", ";"
    );

    parse(petal);
    petal_inspect(petal);
    petal_free(petal);
}

void json_example() {
    Petal *petal = init_with_string("{\"name\": \"John\", \"age\": 30, \"isStudent\": false}");
    petal_configure_json(petal);

    parse(petal);
    petal_inspect(petal);
    petal_free(petal);
}

int main() {
    // brainfuck_example();
    // math_example();
    c_like_example();
    // json_example();

    return 0;
}