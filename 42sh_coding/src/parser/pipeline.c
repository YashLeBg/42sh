#include "pipeline.h"

#include "../utils/utils.h"
#include "command.h"

static struct AST_Node *create_negation(struct Lexer *lexer)
{
    struct AST_Node *negation = NULL;
    int totnot = 0;
    while (lexer->next->type == NOT_TOKEN)
    {
        free_token(pop(lexer));
        totnot++;
        peak(lexer);
    }
    if (totnot % 2 == 1)
    {
        negation = create_ast_node(NEGATION);
    }
    return negation;
}

//  ['!'] command { '|' {'\n'} command }
struct AST_Node *pipeline(struct Lexer *lexer, int *err)
{
    peak(lexer);
    struct AST_Node *negation = create_negation(lexer);
    struct AST_Node *pipeline = NULL;
    struct AST_Node *tmp = NULL;

    while (lexer->next->type == PIPE_TOKEN || pipeline == NULL)
    {
        if (lexer->next->type == PIPE_TOKEN)
            free_token(pop(lexer));
        struct AST_Node *new = create_ast_node(PIPELINE);
        delete_backn(lexer);

        struct AST_Node *child = command(lexer, err);
        if (!child)
        {
            destroy_ast(new);
            destroy_ast(negation);
            destroy_ast(pipeline);
            return NULL;
        }
        new->structType.command_list.child = child;

        if (!pipeline)
        {
            pipeline = new;
            tmp = new;
        }
        else
        {
            tmp->structType.command_list.next = new;
            tmp = new;
        }
        peak(lexer);
    }
    if (negation)
    {
        negation->structType.negation.child = pipeline;
        return negation;
    }
    else
    {
        return pipeline;
    }
}
