#include "else_clause.h"

#include <stdlib.h>

#include "compound_list.h"

/*
else_clause =
      'else' compound_list
    | 'elif' compound_list 'then' compound_list [else_clause]
*/
struct AST_Node *else_clause(struct Lexer *lexer, int *err)
{
    peak(lexer);
    struct AST_Node *ast = NULL;
    if (lexer->next->type == ELSE_TOKEN)
    {
        free_token(pop(lexer));
        return compound_list(lexer, err);
    }
    else if (lexer->next->type == ELIF_TOKEN)
    {
        free_token(pop(lexer));
        ast = create_ast_node(IF_NODE);
        ast->structType.if_node.condition = compound_list(lexer, err);
        peak(lexer);
        if (lexer->next->type != THEN_TOKEN)
        {
            destroy_ast(ast);
            *err = 2;
            return NULL;
        }
        free_token(pop(lexer));
        ast->structType.if_node.then_branch = compound_list(lexer, err);
        peak(lexer);
        if (lexer->next->type == ELSE_TOKEN)
        {
            ast->structType.if_node.else_branch = else_clause(lexer, err);
            if (!ast->structType.if_node.else_branch)
            {
                destroy_ast(ast);
                *err = 2;
                return NULL;
            }
        }
    }
    else
    {
        destroy_ast(ast);
        *err = 2;
        return NULL;
    }
    return ast;
}
