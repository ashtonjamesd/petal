#ifndef PETAL_H
#define PETAL_H

#include <stdbool.h>

#define with_keywords(petal, ...) \
    with_petal_keywords(petal, sizeof((char*[]){__VA_ARGS__}) / sizeof(char*), __VA_ARGS__)

#define with_symbols(petal, ...) \
    with_petal_symbols(petal, sizeof((char*[]){__VA_ARGS__}) / sizeof(char*), __VA_ARGS__)

typedef enum {
    PETAL_KEYWORD,
    PETAL_IDENTIFIER,
    PETAL_WHITESPACE,
    PETAL_INTEGER,
    PETAL_FLOAT,
    PETAL_SYMBOL,
    PETAL_STRING,
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
    bool        has_strings;

    bool        skip_whitespace;
    bool        skip_unrecognized;
    
    int         current;

    int         token_count;
    int         token_capacity;
    PetalToken *tokens;

    int         recognized_token_count;
    int         recognized_token_capacity;
    PetalToken *recognized_tokens;
} Petal;

Petal *init_with_file(char *path);
Petal *init_with_string(char *source);

void   petal_free(Petal *parser);

void   with_petal_keywords(Petal *petal, int count, ...);
void   with_petal_symbols(Petal *petal, int count, ...);
void   with_identifiers(Petal *petal);
void   with_whitespace(Petal *petal);
void   with_integers(Petal *petal);
void   with_floats(Petal *petal);
void   with_strings(Petal *petal);

void   skip_whitespace(Petal *petal);
void   skip_unrecognized(Petal *petal);

void   parse(Petal *parser);

void   petal_inspect(Petal *parser);

#endif