#include "rule_if.h"

#include <stdlib.h>

#include "compound_list.h"
#include "else_clause.h"

// rule_if = 'if' compound_list 'then' compound_list [else_clause] 'fi'
struct AST_Node *rule_if(struct Lexer *lexer, int *err)
{
    peak(lexer);
    struct AST_Node *ast;
    if (lexer->next->type == IF_TOKEN)
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
        if (lexer->next->type == ELSE_TOKEN || lexer->next->type == ELIF_TOKEN)
        {
            ast->structType.if_node.else_branch = else_clause(lexer, err);
            if (!ast->structType.if_node.else_branch)
            {
                destroy_ast(ast);
                *err = 2;
                return NULL;
            }
        }
        if (lexer->next->type != FI_TOKEN)
        {
            destroy_ast(ast);
            *err = 2;
            return NULL;
        }
        free_token(pop(lexer));
    }
    else
    {
        *err = 2;
        return NULL;
    }
    if (!ast->structType.if_node.condition
        || !ast->structType.if_node.then_branch)
    {
        destroy_ast(ast);
        *err = 2;
        return NULL;
    }
    return ast;
}
