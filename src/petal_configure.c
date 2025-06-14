#include "../include/petal_configure.h"

void petal_configure_json(Petal *petal) {
    with_identifiers(petal);
    with_strings(petal);
    with_floats(petal);
    with_whitespace(petal);

    with_symbols(petal, 
        "{", "}", ",", "[", "]", ":"
    );

    with_keywords(petal,
        "true", "false", "null"
    );
}

void petal_configure_brainfuck(Petal *petal) {
    skip_whitespace(petal);
    skip_unrecognized(petal);

    with_symbols(petal,
        "+", "-", "<", ">", ".", ",", "[", "]"
    );
}

void petal_configure_math(Petal *petal) {
    with_floats(petal);
    skip_whitespace(petal);

    with_symbols(petal,
        "+", "-", "*", "/", "%", "^", "(", ")"
    );
}