#define _POSIX_C_SOURCE 200809L

#include "and_or.h"

#include <stdlib.h>

#include "../ast/ast.h"
#include "../parser/pipeline.h"
#include "../utils/utils.h"

// la fonction pour le parsisng de and_or
struct AST_Node *and_or(struct Lexer *lexer, int *err)
{
    struct AST_Node *left_side =
        pipeline(lexer, err); // on commence a parser du coter gauche

    if (!left_side)
    {
        return NULL;
    }

    peak(lexer);
    while (lexer->next
           && (lexer->next->type == AND_TOKEN
               || lexer->next->type
                   == OR_TOKEN)) // on construi une chaine d'operations
    {
        enum operator_type op;

        if (lexer->next->type == AND_TOKEN)
        {
            op = OP_AND;
        }
        else
        {
            op = OP_OR;
        }

        free_token(pop(lexer));
        delete_backn(lexer);

        struct AST_Node *right_side = pipeline(lexer, err);
        if (!right_side)
        {
            destroy_ast(left_side);
            *err = 2;
            return NULL;
        }

        struct AST_Node *node = create_ast_node(OPERATORS);
        node->structType.operators.op = op;
        node->structType.operators.left = left_side;
        node->structType.operators.right = right_side;
        if (!node)
        {
            destroy_ast(left_side);
            destroy_ast(right_side);
            *err = 2;
            return NULL;
        }

        left_side = node;
        peak(lexer);
    }

    return left_side;
}
