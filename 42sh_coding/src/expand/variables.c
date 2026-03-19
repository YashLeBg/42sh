#define _GNU_SOURCE

#include "variables.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../utils/utils.h"
#include "expand.h"

static void reverse(char s[])
{
    int i, j;
    char c;
    for (i = 0, j = strlen(s) - 1; i < j; i++, j--)
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

static void itoa(int n, struct chartoken *res)
{
    int i, sign;
    if ((sign = n) < 0)
    {
        n = -n;
    }
    i = 0;
    n *= 10;
    while ((n /= 10) > 0)
    {
        ensure_capacity(res, 1);
        res->value[i++] = n % 10 + '0';
    }
    if (sign < 0)
    {
        ensure_capacity(res, 1);
        res->value[i++] = '-';
    }
    res->value[i] = '\0';
    reverse(res->value);
}

static char *create_star(struct env_shell *env)
{
    int i = 0;
    struct chartoken *value = init_chartoken();
    while (i < env->argc)
    {
        ensure_capacity(value, value->len + strlen(env->argv[i]) + 1);
        strcat(value->value, env->argv[i]);
        i++;
    }
    char *res = value->value;
    free(value);
    return res;
}

static char *create_question_mark(struct env_shell *env)
{
    struct chartoken *value = init_chartoken();
    itoa(env->status, value);
    char *res = value->value;
    free(value);
    return res;
    return "";
}

static char *create_dollar(void)
{
    struct chartoken *value = init_chartoken();
    itoa((int)getpid(), value);
    char *res = value->value;
    free(value);
    return res;
}

static char *create_diese(struct env_shell *env)
{
    struct chartoken *value = init_chartoken();
    itoa(env->argc, value);
    char *res = value->value;
    free(value);
    return res;
}

static char *create_random(void)
{
    struct chartoken *value = init_chartoken();
    itoa(rand() % 32768, value);
    char *res = value->value;
    free(value);
    return res;
    return "";
}

static char *create_uid(void)
{
    struct chartoken *value = init_chartoken();
    itoa((int)getuid(), value);
    char *res = value->value;
    free(value);
    return res;
    return "";
}

static char *create_oldpwd(struct env_shell *env)
{
    if (env->oldpwd)
    {
        return env->oldpwd;
    }
    return strdup("");
}

static char *create_pwd(void)
{
    return get_current_dir_name();
}

char *return_def_var(struct env_shell *env, char *var_name)
{
    if (strcmp("*", var_name) == 0)
    {
        return create_star(env);
    }
    else if (strcmp("?", var_name) == 0)
    {
        return create_question_mark(env);
    }
    else if (strcmp("$", var_name) == 0)
    {
        return create_dollar();
    }
    else if (0 < atoi(var_name) && atoi(var_name) <= env->argc)
    {
        return env->argv[atoi(var_name) - 1];
    }
    else if (strcmp("#", var_name) == 0)
    {
        return create_diese(env);
    }
    else if (strcmp("RANDOM", var_name) == 0)
    {
        return create_random();
    }
    else if (strcmp("UID", var_name) == 0)
    {
        return create_uid();
    }
    else if (strcmp("OLDPWD", var_name) == 0)
    {
        return create_oldpwd(env);
    }
    else if (strcmp("PWD", var_name) == 0)
    {
        return create_pwd();
    }
    return NULL;
}
