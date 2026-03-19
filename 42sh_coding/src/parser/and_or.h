#ifndef AND_OR_H
#define AND_OR_H

#include <stddef.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

struct AST_Node *and_or(struct Lexer *lexer, int *err);

#endif /* ! AND_OR_H */
