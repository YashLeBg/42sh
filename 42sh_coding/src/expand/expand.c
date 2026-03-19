#define _POSIX_C_SOURCE 200809L
#include "expand.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"
#include "variables.h"

// PRETTY PRINT FUNCTION
void hash_map_dump(struct hash_map *hash_map)
{
    if (!hash_map)
    {
        return;
    }
    for (size_t i = 0; i < hash_map->size; i++)
    {
        struct shell_var *first = hash_map->data[i];
        if (!first)
            continue;

        // Print first without comma
        printf("%s: %s\n", first->name, first->value);

        // Then the rest with preceding comma
        for (struct shell_var *current = first->next; current;
             current = current->next)
        {
            printf(", %s: %s\n", current->name, current->value);
        }
        putchar('\n');
        putchar('\n');
    }
}

static size_t hash(char *name)
{
    if (name)
    {
        uint32_t hash = 2166136261; // FNV offset basis
        uint32_t prime = 16777619; // FNV prime
        while (*name)
        {
            hash ^= *name;
            hash *= prime;
            name++;
        }
        return hash;
    }
    return 0;
}

static struct shell_var *find_at(const struct hash_map *hash_map,
                                 size_t hash_value, char *name)
{
    size_t index = hash_value % hash_map->size;
    struct shell_var *shell_var = hash_map->data[index];
    while (shell_var && strcmp(shell_var->name, name) != 0)
    {
        shell_var = shell_var->next;
    }
    return shell_var;
}

/*static int hash_map_insert(struct hash_map *hash_map, char *name, char *value)
{
    if (!hash_map || hash_map->size == 0 || !name)
        return 1;
    size_t hash_value = hash(name);
    size_t index = hash_value % hash_map->size;
    struct shell_var *existing = find_at(hash_map, hash_value, name);
    if (existing)
    {
        existing->value = value;
        return 2;
    }
    struct shell_var *new_shell_var = malloc(sizeof(struct shell_var));
    if (!new_shell_var)
    {
        return 1;
    }
    new_shell_var->name = name;
    new_shell_var->value = value;
    new_shell_var->next = hash_map->data[index];
    hash_map->data[index] = new_shell_var;
    hash_map->size++;
    return 2;
}*/

char *hash_map_get(struct hash_map *hash_map, char *name)
{
    if (!hash_map || hash_map->size == 0 || !name)
    {
        return NULL;
    }
    size_t hash_value = hash(name);
    struct shell_var *shell_var = find_at(hash_map, hash_value, name);
    if (!shell_var)
    {
        return NULL;
    }
    return shell_var->value;
}

struct hash_map *hash_map_init(void)
{
    struct hash_map *new_hash_map = malloc(sizeof(struct hash_map));
    if (!new_hash_map)
        return NULL;
    struct shell_var **shell_var = calloc(CAPACITY, sizeof(struct shell_var *));
    if (!shell_var)
    {
        free(new_hash_map);
        return NULL;
    }
    new_hash_map->size = CAPACITY;
    new_hash_map->data = shell_var;
    return new_hash_map;
}

void hash_map_free(struct hash_map *hash_map)
{
    if (!hash_map)
    {
        return;
    }
    for (size_t i = 0; i < hash_map->size; i++)
    {
        struct shell_var *tmp = hash_map->data[i];
        while (tmp)
        {
            struct shell_var *tmp2 = tmp->next;
            free(tmp);
            tmp = tmp2;
        }
    }
    free(hash_map->data);
    free(hash_map);
}

struct env_shell *env_shell_init(struct hash_map *hmap, int argc, char **argv)
{
    struct env_shell *env = malloc(sizeof(struct env_shell));
    env->hmap = hmap;
    env->argc = argc;
    env->argv = argv;
    env->status = -1;
    env->oldpwd = calloc(PATH_MAX, sizeof(char));
    env->assword = 1;
    return env;
}

static int push_char(struct chartoken *res, int ch)
{
    if (ensure_capacity(res, 1))
    {
        return 1;
    }
    res->value[res->len] = ch;
    res->len++;
    return 0;
}

static char **expand_at_variable(struct env_shell *env, int *count)
{
    char **result = malloc(sizeof(char *) * (env->argc));
    *count = 0;
    for (int j = 1; j < env->argc; j++)
    {
        result[*count] = env->argv[j];
        (*count)++;
    }
    return result;
}

static char **interprete_var_name(char *var_name, struct env_shell *env,
                                  int *count)
{
    if (var_name[0] == '@')
    {
        char **res = expand_at_variable(env, count);
        return res;
    }
    else
    {
        struct chartoken *name_tmp = init_chartoken();
        push_char(name_tmp, '\0');
        int len = strlen(var_name);
        int i = 0;
        char *var_value = NULL;
        for (; i < len; i++)
        {
            name_tmp->value[i] = var_name[i];
            push_char(name_tmp, '\0');
            var_value = return_def_var(env, name_tmp->value);
            if (var_value)
            {
                i++;
                break;
            }
        }
        if (!var_value)
        {
            char *tmp = hash_map_get(env->hmap, var_name);
            if (!tmp)
            {
                tmp = "";
            }
            var_value = strdup(tmp);
        }
        char **result = malloc(sizeof(char *) * 2);
        if (var_value)
        {
            var_value =
                realloc(var_value, strlen(var_value) + strlen(var_name) + 1);
            strcat(var_value, var_name + i);
            result[0] = var_value;
            result[1] = NULL;
            *count = 1;
        }
        destroy_chartoken(name_tmp);
        return result;
    }
}

static char **expand_dollar(char *res, int *i, struct env_shell *env,
                            int *count)
{
    (*i)++;
    if (res[*i] == '\0' || res[*i] == ' ')
    {
        *count = 0;
        return malloc(sizeof(char *));
    }

    struct chartoken *var_name = init_chartoken();
    char **result = malloc(sizeof(char *) * 2);
    *count = 0;

    if (res[*i] == '{')
    {
        (*i)++;
        while (res[*i] != '}' && res[*i] != '\0')
        {
            push_char(var_name, res[*i]);
            (*i)++;
        }
        if (res[*i] == '}')
        {
            (*i)++;
        }
    }
    else
    {
        while (res[*i] != '\0' && res[*i] != ' ' && res[*i] != '$')
        {
            push_char(var_name, res[*i]);
            (*i)++;
        }
    }
    push_char(var_name, '\0');
    result = interprete_var_name(var_name->value, env, count);
    free(var_name->value);
    free(var_name);
    return result;
}

static char **handle_double_quotes(char *res, int *i, struct env_shell *env)
{
    struct chartoken *value = init_chartoken();
    while (res[*i] != '\"' && res[*i] != '\0')
    {
        if (res[*i] == '$')
        {
            int count = 0;
            char **expanded_vars = expand_dollar(res, i, env, &count);
            for (int j = 0; j < count; j++)
            {
                if (expanded_vars[j])
                {
                    char *str = expanded_vars[j];
                    while (*str)
                    {
                        push_char(value, *str);
                        str++;
                    }
                    if (j < count - 1)
                    {
                        push_char(value, ' ');
                    }
                }
            }
            free(expanded_vars);
        }
        else
        {
            push_char(value, res[*i]);
            (*i)++;
        }
    }
    push_char(value, '\0');
    if (res[*i] == '\"')
    {
        (*i)++;
    }
    char **result = malloc(sizeof(char *) * 2);
    result[0] = value->value;
    result[1] = NULL;
    free(value);
    return result;
}

static void concat(struct expand_arguments *expanded, char **new_values,
                   struct chartoken **value)
{
    if (new_values && new_values[0])
    {
        push_char(*value, '\0');
        ensure_capacity(*value, (*value)->len + strlen(new_values[0]));
        (*value)->value = strcat((*value)->value, new_values[0]);
        if (expanded->nb_arg + 2 >= expanded->capacity)
        {
            expanded->capacity *= 2;
            expanded->arg = realloc(expanded->arg,
                                    sizeof(char *) * (expanded->capacity + 1));
        }
        expanded->arg[expanded->nb_arg++] = (*value)->value;
        expanded->arg[expanded->nb_arg + 1] = NULL;
        free(*value);
        *value = init_chartoken();
    }
}

int expand_word(char *res, struct env_shell *env,
                struct expand_arguments *expanded)
{
    int i = 0;
    struct chartoken *value = init_chartoken();
    while (res[i] != '\0')
    {
        if (res[i] == '\"')
        {
            i++;
            char **new_values = handle_double_quotes(res, &i, env);
            concat(expanded, new_values, &value);
            free(new_values);
        }
        else if (res[i] == '\'')
        {
            i++;
            while (res[i] != '\'' && res[i] != '\0')
            {
                push_char(value, res[i]);
                i++;
            }
            if (res[i] == '\'')
            {
                i++;
            }
        }
        else if (res[i] == '$')
        {
            int count = 0;
            char **new_values = expand_dollar(res, &i, env, &count);
            for (int j = 0; j < count; j++)
            {
                concat(expanded, new_values + j, &value);
            }
            free(new_values);
        }
        else
        {
            push_char(value, res[i]);
            i++;
        }
    }
    if (value->len > 0)
    {
        push_char(value, '\0');
        if (expanded->nb_arg + 2 >= expanded->capacity)
        {
            expanded->capacity *= 2;
            expanded->arg = realloc(expanded->arg,
                                    sizeof(char *) * (expanded->capacity + 1));
        }
        expanded->arg[expanded->nb_arg++] = value->value;
        expanded->arg[expanded->nb_arg + 1] = NULL;
    }
    else
    {
        free(value->value);
    }
    free(value);
    return 0;
}

struct expand_arguments *convert_ll_to_char(struct AST_Node *ast, int *err,
                                            struct env_shell *env)
{
    struct expand_arguments *expanded = malloc(sizeof(struct expand_arguments));
    expanded->nb_arg = 0;
    expanded->capacity = 64;
    expanded->arg = malloc(sizeof(char *) * expanded->capacity);
    struct Linked_List *tmp = ast->structType.simple_command.ll;
    while (tmp)
    {
        *err = expand_word(tmp->value->value, env, expanded);
        tmp = tmp->next;
    }
    return expanded;
}

void free_command(struct expand_arguments *command)
{
    if (command)
    {
        int i = 0;
        while (i < command->nb_arg)
        {
            free(command->arg[i]);
            i++;
        }
        free(command->arg);
        free(command);
    }
}

void destroy_env_shell(struct env_shell *env)
{
    hash_map_free(env->hmap);
    free(env);
}