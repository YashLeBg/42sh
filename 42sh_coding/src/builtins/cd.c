#include "cd.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../expand/expand.h"

int cd(int argc, char **argv, struct env_shell *env)
{
    char *path;
    char current_dir[PATH_MAX];

    char * res = getcwd(current_dir, PATH_MAX);
    if (!res)
    {
        return 2;
    }

    if (argc == 0)
    {
        path = getenv("HOME");
        if (!path)
            path = ".";
    }
    else if (argc == 1 && (strcmp(argv[0], "-") == 0))
    {
        if (env->oldpwd[0] != 0)
        {
            path = env->oldpwd;
        }
        else
        {
            path = current_dir;
        }
        printf("%s\n", path);
    }
    else if (argc == 1)
    {
        path = argv[0];
    }
    else
    {
        fprintf(stderr, "trop d'arg\n");
        return 2;
    }

    // comportement de cd
    if (chdir(path) != 0)
        return 1;
    strcpy(env->oldpwd, current_dir);
    fflush(stdout);

    return 0;
}
