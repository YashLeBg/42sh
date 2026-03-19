#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define CAPACITY 64

/////////
// UTILS//
/////////

void delete_backn(struct Lexer *lexer)
{
    peak(lexer);
    while (lexer->next->type == BACKN_TOKEN)
    {
        free_token(pop(lexer));
        peak(lexer);
    }
}

void free_ll(struct Linked_List *ll)
{
    if (!ll)
    {
        return;
    }
    struct Linked_List *tmp;
    while (ll)
    {
        tmp = ll;
        ll = ll->next;
        free_token(tmp->value);
        free(tmp);
    }
}

// init chartoken
struct chartoken *init_chartoken(void)
{
    struct chartoken *res = malloc(sizeof(struct chartoken));
    res->capacity = CAPACITY;
    res->len = 0;
    res->value = malloc(res->capacity);
    if (!res->value)
    {
        return NULL;
    }

    return res;
}

int ensure_capacity(struct chartoken *res, size_t extra)
{
    while (res->len + extra + 1 >= res->capacity)
    {
        size_t new_capacity = res->capacity * 2;
        char *new_value = realloc(res->value, new_capacity);
        if (!new_value)
        {
            return 1;
        }
        res->value = new_value;
        res->capacity = new_capacity;
    }
    return 0;
}

void destroy_chartoken(struct chartoken *res)
{
    free(res->value);
    free(res);
}

struct list_pid *init_list_pid(void)
{
    struct list_pid *new = malloc(sizeof(struct list_pid));
    new->capacity = CAPACITY;
    new->len = 0;
    new->to_wait = malloc(sizeof(pid_t) * new->capacity);
    return new;
}

void add_pid(struct list_pid *list, pid_t pid)
{
    if (list->capacity < list->len)
    {
        list->capacity *= 2;
        list->to_wait = realloc(list->to_wait, list->len);
    }
    list->to_wait[list->len] = pid;
    list->len++;
}

int wait_all_and_free(struct list_pid *list)
{
    int err = 0;
    int i = 0;
    int status = 0;
    while (i < list->len)
    {
        waitpid(list->to_wait[i], &status, 0);
        err = WEXITSTATUS(status);
        i++;
    }
    free(list->to_wait);
    free(list);
    return err;
}
