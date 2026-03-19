#ifndef SIMPLE_COMMAND_H
#define SIMPLE_COMMAND_H

#include <stddef.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

struct AST_Node *simple_command(struct Lexer *lexer, int *err);

#endif /* ! SIMPLE_COMMAND_H */
