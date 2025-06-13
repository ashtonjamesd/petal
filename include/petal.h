#ifndef PETAL_H
#define PETAL_H

typedef enum {
    PETAL_KEYWORD,
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
    int         current;

    int         token_count;
    int         token_capacity;
    PetalToken *tokens;

    int         keyword_count;
    int         keyword_capacity;
    char       *keywords;

    int         symbol_count;
    int         symbol_capacity;
    char       *symbols;
} PetalParser;

PetalParser *init_with_file(char *path);
PetalParser *init_with_string(char *source);

void         petal_free(PetalParser *parser);

void         with_operator(char *symbol);
void         with_keyword(char *keyword);
void         parse(PetalParser *parser);

void         petal_inspect(PetalParser *parser);

#endif