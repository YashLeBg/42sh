#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

#include "../ast/ast.h"
#include "../lexer/lexer.h"

struct AST_Node *parser(struct Lexer *lexer, int *err);

#endif /* ! PARSER_H */
