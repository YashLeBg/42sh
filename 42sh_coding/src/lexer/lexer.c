#define _POSIX_C_SOURCE 200809L
#include "lexer.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../utils/utils.h"

// Delete all spaces before the next token
static void delete_spaces(FILE *file)
{
    int ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch != ' ' && ch != '\t')
        {
            ungetc(ch, file);
            break;
        }
    }
}

// cette fonction skip tout jusqu'a un \n ou un EOF
static void skip_comment(struct Lexer *lexer)
{
    int ch = 0;

    while ((ch = fgetc(lexer->file)) != EOF
           && ch != '\n') // juste ici la boucle est vide puisque le fgetsc
                          // avance le "curseur"
    {
        continue;
    }

    /*if (ch == '\n')
    {
        ungetc(ch, lexer->file);
    }*/
}

// pour voir si le premier char (len=0) est un #
static int hashtag_found(int len, int ch)
{
    if (len == 0 && ch == '#')
    {
        return 1;
    }
    return 0;
}

// nouveau helpers pour le and or

// securite pour que res->value ait suffisament de place

static int push_char(struct chartoken *res, int ch)
{
    if (ensure_capacity(res, 1))
    {
        return 1;
    }
    res->value[res->len] =
        ch; // Append le character et avance la length du char
    res->len++;
    return 0;
}

static int handle_sep(struct Lexer *lexer, struct chartoken *res, int ch)
{
    if (ch != '\n' && ch != ';')
    {
        return 0;
    }
    // si jamais on a pas commencer a construire le token le separateur eest le
    // token
    if (res->len == 0)
    {
        if (push_char(res, ch))
        {
            return -1;
        }
        return 1;
    }

    ungetc(ch, lexer->file);
    return 1;
}

// j'ai rework la fonction, dites moi si ca vous va
// en gros la fonction append les characters jusqy'a le EOF
static int pass_from_to(FILE *file, int end, struct chartoken *res)
{
    int ch;

    while ((ch = fgetc(file)) != EOF)
    {
        if (push_char(res, ch))
        {
            return 1;
        }
        if (ch == end)
        {
            return 0;
        }
    }
    return 1;
}

static int read_and_or(FILE *file, struct chartoken *res, int ch)
{
    int next;

    if (res->len != 0)
    {
        ungetc(ch, file);
        return 1;
    }

    next = fgetc(file);
    if (next == ch) // verifie que le prochain char soit comme le precedent si
                    // non bah retour a la normalite
    {
        if (push_char(res, ch) || push_char(res, ch))
        {
            return 1;
        }
        return 0;
    }
    if (next != EOF)
    {
        ungetc(next, file);
    }
    if (ch == '|')
    {
        push_char(res, ch);
        return 0;
    }
    ungetc(ch, file);
    return 1;
}

// celui la c'est pour les truc specials, genre les commentaires les ; et le \n
// mais aussi pour le && et ||
static int handle_special(struct Lexer *lexer, struct chartoken *res, int ch)
{
    int sep;
    if (hashtag_found(res->len, ch))
    {
        skip_comment(lexer);
        res->value[0] = '#';
        res->len = 1;
        return 1;
    }
    // ici les ; et le \n
    sep = handle_sep(lexer, res, ch);
    if (sep != 0)
    {
        return sep;
    }
    // les et et ou
    if ((ch == '&' || ch == '|') && res->len == 0)
    {
        if (read_and_or(lexer->file, res, ch) == 0)
        {
            return 1;
        }
    }
    // les single quotes
    if (ch == '\'' || ch == '\"')
    {
        if (push_char(res, ch) || pass_from_to(lexer->file, ch, res))
        {
            return -1;
        }
        return 2;
    }

    return 0;
}

static struct Token lookup_table[] = {
    { "if", IF_TOKEN },     { "then", THEN_TOKEN }, { "elif", ELIF_TOKEN },
    { "else", ELSE_TOKEN }, { "fi", FI_TOKEN },     { ";", SCOL_TOKEN },
    { "&&", AND_TOKEN },    { "||", OR_TOKEN },     { "|", PIPE_TOKEN },
    { "!", NOT_TOKEN }
};

// j'ai rework create value puisque si non mon truc faisait 10000 lignes
static char *create_value(struct Lexer *lexer)
{
    struct chartoken res;
    int ch;
    // on inicialise le buffer pour build le token
    res.capacity = CAPACITY;
    res.len = 0;
    res.value = malloc(res.capacity);
    if (!res.value)
    {
        return NULL;
    }

    delete_spaces(lexer->file);

    // lire jusqu'au EOF ou du vide
    while ((ch = fgetc(lexer->file)) != EOF && ch != ' ' && ch != '\t')
    {
        int st = handle_special(lexer, &res, ch);

        if (st == -1)
        {
            free(res.value);
            return NULL;
        }
        if (st == 1)
        {
            break;
        }
        if (st == 2)
        {
            continue;
        }

        if (push_char(&res, ch))
        {
            free(res.value);
            return NULL;
        }
    }

    res.value[res.len] = '\0'; // important de ne pas oublier le '\0'
    return res.value;
}

// regarde si le token est un assigment_word
static int is_assigment_word(char *value)
{
    int len = strlen(value);
    int equal = 0;
    int quote = 0;
    int i = 0;
    while (value[i] != '=' && i < len)
    {
        if (value[i] == '\'' || value[i] == '\"')
        {
            quote = 1;
        }
        i++;
    }
    if (i < len && value[i] == '=')
    {
        equal = 1;
    }
    return (equal && !quote);
}

// Give the good type for the created token
static void init_type(struct Token *new, char *token)
{
    if (token[0] == '\n' || token[0] == '#')
    {
        new->type = BACKN_TOKEN;
        return;
    }
    if (token[0] == '\0')
    {
        new->type = EOF_TOKEN;
        return;
    }
    if (is_assigment_word(token) == 1)
    {
        new->type = ASSIGNMENT_WORD;
        return;
    }
    // On divise la taille par la ligne par une ligne pour avoir le nombre de
    // cle valeur attendu
    size_t table_size = sizeof(lookup_table) / sizeof(lookup_table[0]);
    for (size_t i = 0; i < table_size; i++)
    {
        if (strcmp(token, lookup_table[i].value) == 0)
        {
            new->type = lookup_table[i].type;
            return;
        }
    }

    // Type par defaut
    new->type = WORD_TOKEN;
    return;
}

// Create a token
static struct Token *create_token(char *token)
{
    struct Token *new = malloc(sizeof *new);
    if (!new)
    {
        return NULL;
    }
    // The good value
    new->value = strdup(token);

    // The good type for the given token
    init_type(new, token);

    return new;
}

struct Token *pop(struct Lexer *lexer)
{
    if (lexer->next)
    {
        struct Token *token = lexer->next;
        lexer->next = NULL;
        return token;
    }

    delete_spaces(lexer->file);

    char *value = create_value(lexer);
    struct Token *token = create_token(value);
    free(value);

    return token;
}

void peak(struct Lexer *lexer)
{
    if (!lexer->next)
    {
        lexer->next = pop(lexer);
    }
}

// Free the token_list
void free_token(struct Token *token)
{
    if (!token)
    {
        return;
    }
    if (token->value)
        free(token->value);
    free(token);
}

struct Lexer *create_lexer(int argc, char **argv, int *nb_argc)
{
    struct Lexer *lexer = malloc(sizeof *lexer);
    // if it's a char *
    if (argc > 2 && strcmp(argv[1], "-c") == 0)
    {
        lexer->file = fmemopen(argv[2], strlen(argv[2]), "r");
        *nb_argc += 2;
    }
    // if it's a FILE
    else if (argc > 1)
    {
        lexer->file = fopen(argv[1], "r");
        *nb_argc += 1;
    }
    else
    {
        lexer->file = stdin;
    }
    lexer->next = NULL;
    return lexer;
}

void free_lexer(struct Lexer *lexer)
{
    if (!lexer)
        return;

    if (lexer->file && lexer->file != stdin)
        fclose(lexer->file);

    if (lexer->next)
        free_token(lexer->next);

    free(lexer);
}

////////////////
// PRETTY-PRINT//
////////////////]

// Fonction annexe pour fermer la commande avec }
void print_end_command(int *cmd)
{
    if (*cmd == 1)
    {
        printf("} ");
        *cmd = 0;
    }
}

// Print the good string for the given token
static int pretty_print(enum Token_type type, int *cmd)
{
    if (type == EOF_TOKEN)
    {
        return 0;
    }
    if (type == BACKN_TOKEN)
    {
        print_end_command(cmd);
        printf("\n");
        return 0;
    }
    if (type == SCOL_TOKEN)
    {
        print_end_command(cmd);
        printf("; ");
        return 0;
    }
    // On divise la taille par la ligne par une ligne pour avoir le nombre de
    // cle valeur attendu
    size_t table_size = sizeof(lookup_table) / sizeof(lookup_table[0]);
    for (size_t i = 0; i < table_size; i++)
    {
        if (type == lookup_table[i].type)
        {
            // IF pour tous les tokens comme IF, FOR. WHILE...
            if (type != IF_TOKEN)
                print_end_command(cmd);

            printf("%s ", lookup_table[i].value);
            return 0;
        }
    }

    return 1;
}

// Ce sera utile pour debugger si jamais
void pretty_print_lexer(struct Lexer *lexer)
{
    struct Token *token;
    int *cmd = malloc(sizeof(int));
    *cmd = 0;
    while ((token = pop(lexer))->type != EOF_TOKEN)
    {
        if (pretty_print(token->type, cmd))
        {
            if (*cmd == 0)
            {
                printf("{ command \"%s\" %d ", token->value, token->type);
                *cmd = 1;
            }
            else
            {
                printf("\"%s\" ", token->value);
            }
        }
        free_token(token);
    }
    print_end_command(cmd);
    printf("EOF");
    free_token(token);
    free(cmd);
    puts("\n");
}
