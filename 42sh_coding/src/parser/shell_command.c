#include "shell_command.h"

#include "rule_if.h"

// shell_command = rule_if
struct AST_Node *shell_command(struct Lexer *lexer, int *err)
{
    peak(lexer);
    return rule_if(lexer, err);
}
