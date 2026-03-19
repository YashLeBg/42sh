#ifndef SHELL_COMMAND_H
#define SHELL_COMMAND_H

#include <stddef.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

struct AST_Node *shell_command(struct Lexer *lexer, int *err);

#endif /* ! SHELL_COMMAND_H */
