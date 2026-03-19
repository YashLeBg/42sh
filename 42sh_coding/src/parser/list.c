#include "list.h"

#include <stdlib.h>

#include "and_or.h"

// list = and_or { ';' and_or } [ ';' ] ;
struct AST_Node *list(struct Lexer *lexer, int *err)
{
    peak(lexer);
    struct AST_Node *ast = NULL;
    struct AST_Node *tmp = NULL;
    while (lexer->next->type != EOF_TOKEN && lexer->next->type != BACKN_TOKEN)
    {
        struct AST_Node *child = and_or(lexer, err);
        if (!child)
        {
            destroy_ast(ast);
            return NULL;
        }
        struct AST_Node *new = create_ast_node(COMMAND_LIST);
        peak(lexer);
        if (lexer->next->type == SCOL_TOKEN)
        {
            free_token(pop(lexer));
        }
        new->structType.command_list.child = child;
        new->structType.command_list.next = NULL;
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
    return ast;
}
