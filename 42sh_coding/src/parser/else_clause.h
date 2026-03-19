#ifndef ELSE_CLAUSE_H
#define ELSE_CLAUSE_H

#include <stddef.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

struct AST_Node *else_clause(struct Lexer *lexer, int *err);

#endif /* ! ELSE_CLAUSE_H */
