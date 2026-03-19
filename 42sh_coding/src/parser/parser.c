#define _POSIX_C_SOURCE 200809L
#include "parser.h"

#include <stdlib.h>

#include "list.h"

//////////
// PARSER//
//////////

/*
input =
    list '\n'
    | list EOF
    | '\n'
    | EOF
*/
struct AST_Node *parser(struct Lexer *lexer, int *err)
{
    peak(lexer);
    if (lexer->next->type == EOF_TOKEN || lexer->next->type == BACKN_TOKEN)
    {
        return NULL;
    }
    struct AST_Node *ast = list(lexer, err);
    if (!ast)
    {
        *err = 2;
        return NULL;
    }
    peak(lexer);
    if (lexer->next->type != EOF_TOKEN && lexer->next->type != BACKN_TOKEN)
    {
        destroy_ast(ast);
        *err = 2;
        return NULL;
    }
    if (lexer->next->type != EOF_TOKEN)
    {
        free_token(pop(lexer));
    }
    return ast;
}
