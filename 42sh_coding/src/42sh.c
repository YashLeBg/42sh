#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast/ast.h"
#include "exec/exec.h"
#include "expand/expand.h"
#include "lexer/lexer.h"

int main(int argc, char **argv)
{
    int nb_argc = 0;
    struct Lexer *lexer = create_lexer(argc, argv, &nb_argc);
    struct AST_Node *ast = NULL;
    struct hash_map *hmap = hash_map_init();
    struct env_shell *env =
        env_shell_init(hmap, (argc - nb_argc), argv + nb_argc);
    int *err = malloc(sizeof(int));
    *err = 0;
    while (feof(lexer->file) == 0)
    {
        ast = parser(lexer, err);
        if (*err != 0)
        {
            break;
        }
        *err = exec_ast(ast, env);
        if (*err != 0)
        {
            break;
        }
        destroy_ast(ast);
        ast = NULL;
    }
    free_lexer(lexer);
    destroy_env_shell(env);
    if (*err != 0)
    {
        destroy_ast(ast);
    }
    int ret = *err;
    free(err);
    if (env->status != -1)
    {
        return env->status;
    }
    if (ret != 0)
    {
        fprintf(stderr, "erreur");
    }
    return ret;
}
