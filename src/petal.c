#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "../include/petal.h"

static PetalParser *petal_init(char *source) {
    PetalParser *petal = malloc(sizeof(PetalParser));

    petal->source = strdup(source);

    petal->token_capacity = 1;
    petal->token_count = 0;
    petal->tokens = malloc(sizeof(petal->token_capacity));

    petal->keyword_capacity = 1;
    petal->keyword_count = 0;
    petal->keywords = malloc(sizeof(char *));

    petal->symbol_capacity = 1;
    petal->symbol_count = 0;
    petal->symbols = malloc(sizeof(char *));

    return petal;
}

PetalParser *init_with_file(char *path) {
    FILE *fptr = fopen(path, "r");
    if (!fptr) {
        printf("unable to find the file at '%s'", path);
        return NULL;
    }

    fseek(fptr, 0, SEEK_END);
    long sz = ftell(fptr);
    rewind(fptr);

    char *buff = malloc(sz + 1);
    fread(buff, 1, sz, fptr);
    buff[sz] = '\0';

    fclose(fptr);

    PetalParser *petal = petal_init(buff);
    return petal;
}

PetalParser *init_with_string(char *source) {
    PetalParser *petal = petal_init(source);
    return petal;
}

void petal_free(PetalParser *petal) {
    free(petal->tokens);
    free(petal->source);
    free(petal);
}

static inline bool is_end(PetalParser *petal) {
    return petal->current >= (int)strlen(petal->source);
}

static inline void advance(PetalParser *petal) {
    petal->current++;
}

static inline char current(PetalParser *petal) {
    return is_end(petal) ? '\0' : petal->source[petal->current];
}

static PetalToken parse_token(PetalParser *petal) {
    PetalToken token;
    token.type = PETAL_UNRECOGNIZED;

    int sz = 1;
    char *lexeme = malloc(sz + 1);
    lexeme[0] = current(petal);
    lexeme[1] = '\0';

    token.lexeme = strdup(lexeme);
    free(lexeme);

    return token;
}

void with_operator(char *symbol) {

}

void with_keyword(char *keyword) {
    
}

static inline void add_token(PetalParser *petal, PetalToken token) {
    if (petal->token_count >= petal->token_capacity) {
        petal->token_capacity *= 2;
        petal->tokens = realloc(petal->tokens, sizeof(PetalToken) * petal->token_capacity);
    }
    
    petal->tokens[petal->token_count++] = token;
}

void parse(PetalParser *petal) {
    while (!is_end(petal)) {
        PetalToken token = parse_token(petal);
        add_token(petal, token);
    
        advance(petal);
    }
}

char *petal_token_type_to_str(PetalTokenType type) {
    switch (type) {
        case PETAL_UNRECOGNIZED: return "unrecognized lexeme";
        default: return "unknown character type";
    }
}

int len_of_num(unsigned int x) {
    if (x >= 1000000000) return 10;
    if (x >= 100000000)  return 9;
    if (x >= 10000000)   return 8;
    if (x >= 1000000)    return 7;
    if (x >= 100000)     return 6;
    if (x >= 10000)      return 5;
    if (x >= 1000)       return 4;
    if (x >= 100)        return 3;
    if (x >= 10)         return 2;
    return 1;
}

void petal_inspect(PetalParser *petal) {
    printf("\n  petal inspect results:\n");
    for (int i = 0; i < 40; i++) putchar('-');
    printf("\n");

    int max_width = len_of_num(petal->token_count - 1);
    for (int i = 0; i < petal->token_count; i++) {
        int cur_width = len_of_num(i);
        printf("%d", i);
        for (int j = 0; j < max_width - cur_width + 1; j++) {
            printf(" ");
        }
        printf("| '%s': %s\n", petal->tokens[i].lexeme, petal_token_type_to_str(petal->tokens[i].type));
    }
}
