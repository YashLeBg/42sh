#ifndef PIPELINE_H
#define PIPELINE_H

#include <stddef.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

struct AST_Node *pipeline(struct Lexer *lexer, int *err);

#endif /* ! PIPELINE_H */
