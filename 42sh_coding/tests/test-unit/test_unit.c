#include <criterion/criterion.h>
#include <criterion/logging.h>
#include <criterion/redirect.h>
#include <stddef.h>
#include <string.h>

#include "echo.h"
#include "exec.h"
#include "expand.h"
#include "lexer.h"
#include "structs.h"

// tests simple pour voir si criterion criterionne bien

Test(sanity, criterion_runs)
{
    cr_assert(1);
}

Test(sanity, basic_math)
{
    cr_assert_eq(2 + 67, 69);
    cr_assert_neq(2 + 67, 100);
}

Test(sanity, string_compare)
{
    cr_assert_str_eq("42sh", "42sh");
    cr_assert_str_neq("42sh", "ash");
}

// celui la c'est juste un helper pour avoir le stdout du echo
static void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// BUILTINS: true et false

Test(builtins, true_returns_0)
{
    cr_assert_eq(true_builtin(), 0);
}

Test(builtins, false_returns_1)
{
    cr_assert_eq(false_builtin(), 1);
}

// BUILTINS: echo

Test(builtins_echo, echo_simple, .init = redirect_all_stdout)
{
    char *argv[] = { "hello", NULL };

    cr_assert_eq(echo(1, argv), 0);
    cr_assert_stdout_eq_str("hello\n");
}

Test(builtins_echo, echo_deux_args, .init = redirect_all_stdout)
{
    // il fait hello world mais avec l'espace
    char *argv[] = { "hello", "world", NULL };

    cr_assert_eq(echo(2, argv), 0);
    cr_assert_stdout_eq_str("hello world\n");
}

Test(builtins_echo, echo_n_no_newline, .init = redirect_all_stdout)
{
    char *argv[] = { "-n", "hello", NULL };

    cr_assert_eq(echo(2, argv), 0);
    cr_assert_stdout_eq_str("hello");
}

Test(builtins_echo, echo_e, .init = redirect_all_stdout)
{
    // il fait le \n et \t
    char *argv[] = { "-e", "a\\nb\\tX\\\\", NULL };

    cr_assert_eq(echo(2, argv), 0);
    cr_assert_stdout_eq_str("a\nb\tX\\\n");
}

Test(builtins_echo, echo_E, .init = redirect_all_stdout)
{
    // print literalement les backslashes
    char *argv[] = { "-E", "a\\nb\\tX\\\\", NULL };

    cr_assert_eq(echo(2, argv), 0);
    cr_assert_stdout_eq_str("a\\nb\\tX\\\\\n");
}

Test(builtins_echo, echo_default, .init = redirect_all_stdout)
{
    // sans -e, pareil que -E
    char *argv[] = { "a\\nb", NULL };

    cr_assert_eq(echo(1, argv), 0);
    cr_assert_stdout_eq_str("a\\nb\n");
}

// test du expand

Test(lexer, token_if_then_fi_nl)
{
    // ce test est pour verifier les tokens, si un seul test est faux alors
    // faill le test
    char *argv[] = { "42sh", "-c", "if true; then echo ok; fi\n", NULL };
    struct Lexer *lx = create_lexer(3, argv);
    cr_assert_not_null(lx);

    struct Token *t;

    t = pop(lx);
    cr_assert_eq(t->type, IF_TOKEN);
    cr_assert_str_eq(t->value, "if");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, WORD_TOKEN);
    cr_assert_str_eq(t->value, "true");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, SCOL_TOKEN);
    cr_assert_str_eq(t->value, ";");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, THEN_TOKEN);
    cr_assert_str_eq(t->value, "then");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, WORD_TOKEN);
    cr_assert_str_eq(t->value, "echo");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, WORD_TOKEN);
    cr_assert_str_eq(t->value, "ok");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, SCOL_TOKEN);
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, FI_TOKEN);
    cr_assert_str_eq(t->value, "fi");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, BACKN_TOKEN);
    cr_assert_str_eq(t->value, "\n");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, EOF_TOKEN);
    free_token(t);

    free_lexer(lx);
}

Test(lexer, skip_space_tab)
{
    // dans ce test les espaces et les tabs devraient etre ignorer et on devrais
    // avoir ces tokens la echo hello world \n EOF
    char *argv[] = { "42sh", "-c", "   \techo\t  hello\tworld  \n", NULL };
    struct Lexer *lx = create_lexer(3, argv);
    cr_assert_not_null(lx);

    struct Token *t;

    t = pop(lx);
    cr_assert_eq(t->type, WORD_TOKEN);
    cr_assert_str_eq(t->value, "echo");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, WORD_TOKEN);
    cr_assert_str_eq(t->value, "hello");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, WORD_TOKEN);
    cr_assert_str_eq(t->value, "world");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, BACKN_TOKEN);
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, EOF_TOKEN);
    free_token(t);

    free_lexer(lx);
}

Test(lexer, ptvirgule_split)
{
    // celui la est juste pour check que il y a pas besoin d'espace just du ;
    // pour separe
    char *argv[] = { "42sh", "-c", "echo a;echo b;\n", NULL };
    struct Lexer *lx = create_lexer(3, argv);
    cr_assert_not_null(lx);

    struct Token *t;

    t = pop(lx);
    cr_assert_eq(t->type, WORD_TOKEN);
    cr_assert_str_eq(t->value, "echo");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, WORD_TOKEN);
    cr_assert_str_eq(t->value, "a");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, SCOL_TOKEN);
    cr_assert_str_eq(t->value, ";");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, WORD_TOKEN);
    cr_assert_str_eq(t->value, "echo");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, WORD_TOKEN);
    cr_assert_str_eq(t->value, "b");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, SCOL_TOKEN);
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, BACKN_TOKEN);
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, EOF_TOKEN);
    free_token(t);

    free_lexer(lx);
}

Test(lexer, ignore_single_quotes)
{
    // ce test la est pour voir si il ignoire les single quotes (il devrait pas)
    // on attend ce output "'hello   world'" avec un retour a la ligne
    char *argv[] = { "42sh", "-c", "echo 'hello   world'\n", NULL };
    struct Lexer *lx = create_lexer(3, argv);
    cr_assert_not_null(lx);

    struct Token *t;

    t = pop(lx);
    cr_assert_eq(t->type, WORD_TOKEN);
    cr_assert_str_eq(t->value, "echo");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, WORD_TOKEN);
    // le lexer garde les quotes dans la value
    cr_assert_str_eq(t->value, "'hello   world'");
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, BACKN_TOKEN);
    free_token(t);

    t = pop(lx);
    cr_assert_eq(t->type, EOF_TOKEN);
    free_token(t);

    free_lexer(lx);
}

Test(lexer, peak_keep_token)
{
    // peak devrait regarder le prochain token et pas le consomer
    char *argv[] = { "42sh", "-c", "echo x\n", NULL };
    struct Lexer *lx = create_lexer(3, argv);
    cr_assert_not_null(lx);

    peak(lx);
    cr_assert_not_null(lx->next);
    cr_assert_eq(lx->next->type, WORD_TOKEN);
    cr_assert_str_eq(lx->next->value, "echo");

    // pop() doit donner le meme token
    struct Token *t = pop(lx);
    cr_assert_eq(t->type, WORD_TOKEN);
    cr_assert_str_eq(t->value, "echo");
    free_token(t);

    free_lexer(lx);
}
