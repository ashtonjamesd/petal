#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#include "../include/petal.h"

#define recede() petal->current--;
#define advance() petal->current++;

#define current() petal->source[petal->current]

static Petal *petal_init(char *source) {
    Petal *petal = malloc(sizeof(Petal));
    petal->current = 0;

    petal->source = strdup(source);

    petal->has_identifiers = false;
    petal->has_whitespace = false;
    petal->has_integers = false;
    petal->has_floats = false;
    petal->skip_whitespace = false;
    petal->has_strings = false;
    petal->skip_unrecognized = false;

    petal->recognized_token_count = 0;
    petal->recognized_token_capacity = 1;
    petal->recognized_tokens = malloc(sizeof(PetalToken));
    
    petal->token_capacity = 1;
    petal->token_count = 0;
    petal->tokens = malloc(sizeof(PetalToken));

    return petal;
}

Petal *init_with_file(char *path) {
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

    Petal *petal = petal_init(buff);
    return petal;
}

Petal *init_with_string(char *source) {
    Petal *petal = petal_init(source);
    return petal;
}

void petal_free(Petal *petal) {
    for (int i = 0; i < petal->recognized_token_count; i++) {
        free(petal->recognized_tokens[i].lexeme);
    }
    free(petal->recognized_tokens);

    for (int i = 0; i < petal->token_count; i++) {
        free(petal->tokens[i].lexeme);
    }
    free(petal->tokens);
    free(petal->source);
    free(petal);
}

static inline PetalToken init_token(char *lexeme, PetalTokenType type) {
    PetalToken token;
    token.lexeme = strdup(lexeme);
    token.type = type;

    return token;
}

char *petal_token_type_to_str(PetalTokenType type) {
    switch (type) {
        case PETAL_UNRECOGNIZED: return "unrecognized lexeme";
        case PETAL_IDENTIFIER: return "identifier";
        case PETAL_KEYWORD: return "keyword";
        case PETAL_WHITESPACE: return "whitespace";
        case PETAL_FLOAT: return "float";
        case PETAL_INTEGER: return "integer";
        case PETAL_SYMBOL: return "symbol";
        case PETAL_STRING: return "string";
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

void petal_inspect(Petal *petal) {
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

static inline bool is_end(Petal *petal) {
    return petal->current >= (int)strlen(petal->source);
}

void with_whitespace(Petal *petal) {
    petal->has_whitespace = true;
}

void skip_whitespace(Petal *petal) {
    with_whitespace(petal);
    petal->skip_whitespace = true;
}

void skip_unrecognized(Petal *petal) {
    petal->skip_unrecognized = true;
}

void with_integers(Petal *petal) {
    petal->has_integers = true;
}

void with_floats(Petal *petal) {
    with_integers(petal);
    petal->has_floats = true;
}

void with_identifiers(Petal *petal) {
    petal->has_identifiers = true;
}

void with_strings(Petal *petal) {
    petal->has_strings = true;
}

void with_petal_keywords(Petal *petal, int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        char *keyword = va_arg(args, char *);

        PetalToken token;
        token.lexeme = strdup(keyword);
        token.type = PETAL_KEYWORD;
        
        if (petal->recognized_token_count >= petal->recognized_token_capacity) {
            petal->recognized_token_capacity *= 2;

            petal->recognized_tokens = realloc(
                petal->recognized_tokens, 
                sizeof(PetalToken) * petal->recognized_token_capacity
            );
        }
        petal->recognized_tokens[petal->recognized_token_count++] = token;
    }

    va_end(args);
}

void with_petal_symbols(Petal *petal, int count, ...) {
    va_list args;
    va_start(args, count);

    for (int i = 0; i < count; i++) {
        char *keyword = va_arg(args, char *);

        PetalToken token = init_token(keyword, PETAL_SYMBOL);

        if (petal->recognized_token_count >= petal->recognized_token_capacity) {
            petal->recognized_token_capacity *= 2;

            petal->recognized_tokens = realloc(
                petal->recognized_tokens, 
                sizeof(PetalToken) * petal->recognized_token_capacity
            );
        }
        petal->recognized_tokens[petal->recognized_token_count++] = token;
    }

    va_end(args);
}

static PetalToken parse_integer(Petal *petal) {
    int start = petal->current;

    PetalTokenType type = PETAL_INTEGER;
    
    bool has_seen_dot = false;
    while (!is_end(petal) && (isalnum(current()) || (current() == '.' && petal->has_floats))) { 
        if (current() == '.' && petal->has_floats) {
            has_seen_dot = true;
            type = PETAL_FLOAT;
        } else if (current() == '.' && has_seen_dot) {
            break;
        }

        advance();
    }

    int sz = petal->current - start;
    char *lexeme = malloc(sz + 1);
    strncpy(lexeme, petal->source + start, sz);
    lexeme[sz] = '\0';

    recede();

    PetalToken token = init_token(lexeme, type);
    free(lexeme);

    return token;
}

static bool is_recognized(Petal *petal, char *lexeme, PetalTokenType type) {
    for (int i = 0; i < petal->recognized_token_count; i++) {
        PetalToken token = petal->recognized_tokens[i];

        if (strcmp(token.lexeme, lexeme) == 0 && token.type == type) {
            return true;
        }
    }

    return false;
}

static PetalToken parse_identifier(Petal *petal) {
    int start = petal->current;

    while (!is_end(petal) && isalnum(current())) { 
        advance();
    }

    int sz = petal->current - start;
    char *lexeme = malloc(sz + 1);
    strncpy(lexeme, petal->source + start, sz);
    lexeme[sz] = '\0';

    PetalTokenType type = PETAL_IDENTIFIER;
    if (is_recognized(petal, lexeme, PETAL_KEYWORD)) {
        type = PETAL_KEYWORD;
    }

    recede();

    PetalToken token = init_token(lexeme, type);
    free(lexeme);

    return token;
}

static PetalToken parse_whitespace(Petal *petal) {
    int sz = 1;
    char *lexeme = malloc(sz + 1);
    lexeme[0] = current();
    lexeme[1] = '\0';

    PetalToken token = init_token(lexeme, PETAL_WHITESPACE);
    free(lexeme);

    return token;
}

static PetalToken parse_string(Petal *petal) {
    char quote = current();
    advance();

    int start = petal->current;
    while (!is_end(petal) && current() != quote) {
        if (current() == '\\' && !is_end(petal)) {
            advance();
        }
        advance();
    }

    int end = petal->current;
    int length = end - start;
    char *lexeme = malloc(length + 3);

    lexeme[0] = quote;
    strncpy(lexeme + 1, petal->source + start, length);
    lexeme[length + 1] = quote;
    lexeme[length + 2] = '\0';

    PetalToken token = init_token(lexeme, PETAL_STRING);
    free(lexeme);

    return token;
}

static PetalToken parse_symbol(Petal *petal) {
    int sz = 1;
    char *lexeme = malloc(sz + 1);
    lexeme[0] = current();
    lexeme[1] = '\0';

    PetalTokenType type = PETAL_UNRECOGNIZED;
    if (is_recognized(petal, lexeme, PETAL_SYMBOL)) {
        type = PETAL_SYMBOL;
    }

    PetalToken token = init_token(lexeme, type);
    free(lexeme);

    return token;
}

static PetalToken parse_token(Petal *petal) {
    if (isalpha(current()) && petal->has_identifiers) {
        return parse_identifier(petal);
    }
    else if (isdigit(current()) && petal->has_integers) {
        return parse_integer(petal);
    }
    else if (isspace(current()) && petal->has_whitespace) {
        return parse_whitespace(petal);
    }
    else if (current() == '\"' && petal->has_strings) {
        return parse_string(petal);
    }
    else {
        return parse_symbol(petal);
    }
}

static inline void add_token(Petal *petal, PetalToken token) {
    if (petal->token_count >= petal->token_capacity) {
        petal->token_capacity *= 2;
        petal->tokens = realloc(petal->tokens, sizeof(PetalToken) * petal->token_capacity);
    }
    
    petal->tokens[petal->token_count++] = token;
}

void parse(Petal *petal) {
    while (!is_end(petal)) {
        PetalToken token = parse_token(petal);
        if (token.type == PETAL_WHITESPACE && petal->skip_whitespace) {
            free(token.lexeme);
            advance();

            continue;
        }

        if (token.type == PETAL_UNRECOGNIZED && petal->skip_unrecognized) {
            free(token.lexeme);
            advance();

            continue;
        }

        add_token(petal, token);
    
        advance();
    }
}