#ifndef RULE_IF_H
#define RULE_IF_H

#include <stddef.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

struct AST_Node *rule_if(struct Lexer *lexer, int *err);

#endif /* ! RULE_IF_H */
