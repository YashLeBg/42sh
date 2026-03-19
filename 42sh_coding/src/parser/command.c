#include "command.h"

#include "shell_command.h"
#include "simple_command.h"
/*
command =
    simple_command
    | shell_command
*/
struct AST_Node *command(struct Lexer *lexer, int *err)
{
    peak(lexer);
    if (lexer->next->type == IF_TOKEN)
    {
        return shell_command(lexer, err);
    }
    if (lexer->next->type == WORD_TOKEN)
    {
        return simple_command(lexer, err);
    }
    *err = 2;
    return NULL;
}
