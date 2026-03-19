#include "simple_command.h"

#include <stdlib.h>

#include "../utils/utils.h"

// simple_command = WORD { element } ;
struct AST_Node *simple_command(struct Lexer *lexer, int *err)
{
    peak(lexer);
    struct Linked_List *ll = NULL;
    struct Linked_List *tmp = NULL;
    while (lexer->next->type == WORD_TOKEN
           || lexer->next->type == ASSIGNMENT_WORD)
    {
        struct Linked_List *new = malloc(sizeof(struct Linked_List));
        if (!new)
        {
            free_ll(ll);
            *err = 2;
            return NULL;
        }
        new->value = pop(lexer);
        new->next = NULL;
        if (!ll)
        {
            ll = new;
            tmp = new;
        }
        else
        {
            tmp->next = new;
            tmp = new;
        }
        peak(lexer);
    }
    struct AST_Node *node = create_ast_node(SIMPLE_COMMAND);
    if (!node)
    {
        free_ll(ll);
        *err = 2;
        return NULL;
    }
    node->structType.simple_command.ll = ll;
    return node;
}
