#ifndef EXPAND_H
#define EXPAND_H

#include "../parser/parser.h"

#define PATH_MAX 4096

struct expand_arguments
{
    int nb_arg;
    int capacity;
    char **arg;
};

struct shell_var
{
    char *name;
    char *value;
    struct shell_var *next;
};

struct env_shell
{
    struct hash_map *hmap;
    int argc;
    char **argv;
    int status; // code d'erreur actuel = sortie dernière commande
    char *oldpwd; // sert à stocker le dossier précédent
    int assword;
};

struct hash_map
{
    struct shell_var **data;
    size_t size;
};

void hash_map_dump(struct hash_map *hash_map);
struct hash_map *hash_map_init(void);
struct env_shell *env_shell_init(struct hash_map *map, int argc, char **argv);
int expand_word(char *res, struct env_shell *env,
                struct expand_arguments *expanded);
int expand(struct expand_arguments *expand);
struct expand_arguments *convert_ll_to_char(struct AST_Node *ast, int *err,
                                            struct env_shell *env);
void free_command(struct expand_arguments *command);
void destroy_env_shell(struct env_shell *env);
#endif /* ! EXPAND_H */
