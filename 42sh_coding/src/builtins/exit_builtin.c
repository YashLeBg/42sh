#include "exit_builtin.h"

#include <stdio.h>
#include <stdlib.h>

#include "../expand/expand.h"

static int is_int(const char *value)
{
    while (*value != '\0' && *value >= '0' && *value <= '9')
    {
        value++;
    }
    if (*value != '\0')
    {
        return 1;
    }
    return 0;
}

int exit_builtin(int argc, char **argv, struct env_shell *env)
{
    if (argc == 1)
    {
        if (is_int(argv[0]) == 1)
        {
            return 2;
        }
        int code = atoi(argv[0]);
        if (code >= 0 && code <= 255)
            env->status = code;
    }
    else if (argc == 0)
    {
        env->status = 0;
    }
    else
    {
        printf("exit: too many arguments");
        return 2;
    }

    return 1;
}
