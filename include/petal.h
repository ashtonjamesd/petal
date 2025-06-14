#ifndef PETAL_H
#define PETAL_H

#include <stdbool.h>

#define WITH_KEYWORDS(petal, ...) \
    with_keywords(petal, sizeof((char*[]){__VA_ARGS__}) / sizeof(char*), __VA_ARGS__)

#define WITH_OPERATORS(petal, ...) \
    with_symbols(petal, sizeof((char*[]){__VA_ARGS__}) / sizeof(char*), __VA_ARGS__)

typedef enum {
    PETAL_KEYWORD,
    PETAL_IDENTIFIER,
    PETAL_WHITESPACE,
    PETAL_INTEGER,
    PETAL_FLOAT,
    PETAL_SYMBOL,
    PETAL_UNRECOGNIZED,
} PetalTokenType;

typedef struct {
    char           *lexeme;
    PetalTokenType  type;
} PetalToken;

typedef struct {
    int         node_count;
    int         node_capacity;
    PetalToken *nodes;
} PetalAst;

typedef struct {
    char       *source;

    bool        has_identifiers;
    bool        has_floats;
    bool        has_integers;
    bool        has_whitespace;
    int         current;

    int         token_count;
    int         token_capacity;
    PetalToken *tokens;

    int         recognized_token_count;
    int         recognized_token_capacity;
    PetalToken *recognized_tokens;
} PetalParser;

PetalParser *init_with_file(char *path);
PetalParser *init_with_string(char *source);

void         petal_free(PetalParser *parser);

void         with_keywords(PetalParser *petal, int count, ...);
void         with_symbols(PetalParser *petal, int count, ...);
void         with_identifiers(PetalParser *petal);
void         with_whitespace(PetalParser *petal);
void         with_integers(PetalParser *petal);
void         with_floats(PetalParser *petal);

void         parse(PetalParser *parser);

void         petal_inspect(PetalParser *parser);

#endif