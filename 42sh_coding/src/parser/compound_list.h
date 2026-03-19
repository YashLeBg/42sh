#ifndef COMPOUND_LIST_H
#define COMPOUND_LIST_H

#include <stddef.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

struct AST_Node *compound_list(struct Lexer *lexer, int *err);

#endif /* ! COMPOUND_LIST_H */
