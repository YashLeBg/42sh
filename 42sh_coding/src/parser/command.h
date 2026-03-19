#ifndef COMMAND_H
#define COMMAND_H

#include <stddef.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

struct AST_Node *command(struct Lexer *lexer, int *err);

#endif /* ! COMMAND_H */
