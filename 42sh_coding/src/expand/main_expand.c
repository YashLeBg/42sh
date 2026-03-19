#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expand.h"

int main(void)
{
    struct env_shell *env = env_shell_init(NULL, 0, NULL);
    struct expand_arguments *exp = malloc(sizeof(struct expand_arguments));
    exp->nb_arg = 0;
    exp->capacity = 4;
    exp->arg = calloc(exp->capacity, sizeof(char *));

    expand_word("H$PWD!", env, exp);
    printf("%s\n", exp->arg[0]);

    expand_word("\"$RANDOM! try\"", env, exp);
    printf("%s\n", exp->arg[1]);

    expand_word("Hello", env, exp);
    printf("%s\n", exp->arg[2]);

    expand_word("\'$PWD!\'", env, exp);
    printf("%s\n", exp->arg[3]);

    free(exp->arg);
    free(exp);
    destroy_env_shell(env);
    return 0;
}