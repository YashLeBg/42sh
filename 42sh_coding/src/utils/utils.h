#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>
#include <sys/wait.h>

#include "../lexer/lexer.h"

struct Linked_List
{
    struct Token *value;
    struct Linked_List *next;
};

struct list_pid
{
    pid_t *to_wait;
    int len;
    int capacity;
};

void delete_backn(struct Lexer *lexer);
void free_ll(struct Linked_List *ll);
struct chartoken *init_chartoken(void);
int ensure_capacity(struct chartoken *res, size_t extra);
void destroy_chartoken(struct chartoken *res);
struct list_pid *init_list_pid(void);
void add_pid(struct list_pid *list, pid_t pid);
int wait_all_and_free(struct list_pid *list);

#endif /* ! UTILS_H */
