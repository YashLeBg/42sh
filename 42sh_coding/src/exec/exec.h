#ifndef EXEC_H
#define EXEC_H

#include "../expand/expand.h"
#include "../builtins/echo.h"
#include "../builtins/cd.h"
#include "../builtins/exit_builtin.h"

int true_builtin(void);
int false_builtin(void);
int exec_ast(struct AST_Node *ast, struct env_shell *env);

#endif /* ! EXEC_H */
