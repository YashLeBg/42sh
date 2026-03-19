#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <stdio.h>

#define CAPACITY 64

struct chartoken
{
    size_t capacity;
    size_t len;
    char *value;
};

enum Token_type
{
    IF_TOKEN,
    THEN_TOKEN,
    ELIF_TOKEN,
    ELSE_TOKEN,
    FI_TOKEN,
    SCOL_TOKEN,
    BACKN_TOKEN,
    AND_TOKEN,
    OR_TOKEN,
    PIPE_TOKEN,
    NOT_TOKEN,
    WORD_TOKEN,
    ASSIGNMENT_WORD,
    EOF_TOKEN
};

struct Token
{
    char *value;
    enum Token_type type;
};

struct Lexer
{
    FILE *file;
    struct Token *next;
};

// char *create_value(struct Lexer *lexer);
// struct Token *create_token(char *value);
void free_token(struct Token *token);
struct Token *pop(struct Lexer *lexer);
void peak(struct Lexer *lexer);

struct Lexer *create_lexer(int argc, char **argv, int *nb_argc);
void free_lexer(struct Lexer *lexer);

void pretty_print_lexer(struct Lexer *lexer);

#endif /* !LEXER_H */
