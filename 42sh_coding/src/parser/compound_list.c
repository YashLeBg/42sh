#include "compound_list.h"

#include <stdlib.h>

#include "../utils/utils.h"
#include "and_or.h"

// compound_list = {'\n'} and_or { ( ';' | '\n' ) {'\n'} and_or } [';'] {'\n'} ;
struct AST_Node *compound_list(struct Lexer *lexer, int *err)
{
    delete_backn(lexer);
    peak(lexer);
    struct AST_Node *ast = NULL;
    struct AST_Node *tmp = NULL;
    while (lexer->next->type != THEN_TOKEN && lexer->next->type != ELIF_TOKEN
           && lexer->next->type != ELSE_TOKEN && lexer->next->type != FI_TOKEN)
    {
        struct AST_Node *child = and_or(lexer, err);
        if (!child)
        {
            destroy_ast(ast);
            *err = 2;
            return NULL;
        }
        struct AST_Node *new = create_ast_node(COMMAND_LIST);
        new->structType.command_list.child = child;
        peak(lexer);
        if (lexer->next->type == SCOL_TOKEN || lexer->next->type == BACKN_TOKEN)
        {
            free_token(pop(lexer));
        }
        delete_backn(lexer);
        if (!ast)
        {
            ast = new;
            tmp = new;
        }
        else
        {
            tmp->structType.command_list.next = new;
            tmp = new;
        }
        peak(lexer);
    }
    if (!ast)
    {
        *err = 2;
    }
    delete_backn(lexer);
    return ast;
}
