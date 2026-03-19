#ifndef LIST_H
#define LIST_H

#include <stddef.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

struct AST_Node *list(struct Lexer *lexer, int *err);

#endif /* ! LIST_H */
