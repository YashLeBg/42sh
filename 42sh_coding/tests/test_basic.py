import pytest

from conftest import run_shell

pytestmark = pytest.mark.functional


# =====================
# STEP 1  EASY tests
# =====================

def test_echo_easy():
    res = run_shell("echo salut\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "salut\n"


def test_true_easy():
    res = run_shell("true\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0


def test_false_easy():
    res = run_shell("false\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 1


def test_list_semicolon_easy():
    res = run_shell("echo a; echo b;\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "a\nb\n"


def test_comment_full_line_easy():
    res = run_shell("# comment only\necho ok\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "ok\n"


# =====================
# STEP 1  MEDIUM tests
# =====================

def test_if_then_else_medium():
    res = run_shell("if true; then echo YES; else echo NO; fi\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "YES\n"


def test_if_elif_medium():
    script = "if false; then echo A; elif true; then echo B; else echo C; fi\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "B\n"


def test_multiple_newlines_medium():
    script = "if false; then echo A; elif true; then echo B; else echo C; fi\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "B\n"


# =====================
# STEP 1 COMPLEX tests
# =====================


def test_single_quotes_edge():
    res = run_shell("echo 'hello   world'\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "hello   world\n"


def test_missing_fi_edge():
    # There is a grammar error here so we should get an error
    res = run_shell("if true; then echo ok;\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 2
    assert res.stderr != ""


def test_comment_not_first_char_edge():
    res = run_shell("echo not#comment\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "not#comment\n"


def test_if_newline_style_mixed():
    script = (
        "if true\n"
        "then\n"
        "echo A\n"
        "echo B\n"
        "fi\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "A\nB\n"

# ====================
# STEP 1 RANDOM tests
# ====================

def test_lists_and_comments():
    script = (
        "# comment at beginning\n"
        "echo A; echo B # trailing comment\n"
        "echo C\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "A\nB\nC\n"


def test_if_with_semicolons_and_newlines():
    script = (
        "if true; then\n"
        "echo YES; echo ALSO\n"
        "else\n"
        "echo NO\n"
        "fi\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "YES\nALSO\n"


def test_elif_and_compound_lists():
    script = (
        "if false; then echo A; "
        "elif false; then echo B; "
        "elif true; then echo C; echo D; "
        "else echo E; "
        "fi\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "C\nD\n"


# =======================
# TEST FOR REDIRECTIONS
# =======================


"""

def test_redirection_easy_01():
    script = "rm -f t1\n" "echo hi > t1\n" "cat t1\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "hi\n"


def test_redirection_easy_02():
    script = (
        "rm -f t2\n"
        "echo a > t2\n"
        "echo b >> t2\n"
        "cat t2\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "a\nb\n"


def test_redirection_easy_03():
    script = (
        "rm -f t3\n"
        "echo hello > t3\n"
        "cat < t3\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "hello\n"


def test_redirection_easy_04():
    # on garde le stdout libre pour le prochain builtin
    script = (
        "rm -f t4\n"
        "echo tofile > t4\n"
        "echo tostdout\n"
        "cat t4\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "tostdout\ntofile\n"


def test_redirection_easy_05():
    # >| devrait overwrite
    script = (
        "rm -f t5\n"
        "echo one > t5\n"
        "echo two >| t5\n"
        "cat t5\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "two\n"


def test_redirection_medium_01():
    # prefix redirection
    script = (
        "rm -f t6\n"
        "> t6 echo ok\n"
        "cat t6\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "ok\n"


def test_redirection_medium_02():
    script = (
        "rm -f t7 t8\n"
        "echo hi > t7 > t8\n"
        "cat t7\n"
        "cat t8\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    # Only last redirection gets output
    assert res.stdout == "hi\n"


def test_redirection_medium_03():
    script = (
        "rm -f in out\n"
        "echo abc > in\n"
        "cat < in > out\n"
        "cat out\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "abc\n"


def test_redirection_edge_01():
    # 2>&1 envoie stderr vers stdout
    script = "ls /no/such/path 2>&1\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode != 0
    assert res.stdout != ""


def test_redirection_edge_02():
    # 1>&2 puts stdout to stderr res.stdout is be empty
    script = "echo hi 1>&2\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == ""


def test_redirection_edge_03():
    # 1>&- closes stdout echo fails or produce no output
    script = "echo hi 1>&-\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    # behavior could be different so we just check to see that it is empty
    assert res.stdout == ""


# =================
# TEST PIPELINES
# =================

def test_pipeline_easy_01():
    res = run_shell("echo sup | cat\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "sup\n"


def test_pipeline_easy_02():
    res = run_shell("true | false\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 1


def test_pipeline_easy_03():
    res = run_shell("false | true\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0


def test_pipeline_easy_04():
    res = run_shell("echo hi | cat | cat\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "hi\n"


def test_pipeline_easy_05():
    script = "echo hi |\ncat\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "hi\n"


def test_pipeline_medium_01():
    script = "if true; then echo hello | cat; else echo NO; fi\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "hello\n"


def test_pipeline_medium_02():
    # last command fails -> pipeline fails
    script = "echo hi | cat | false\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 1


def test_pipeline_medium_03():
    script = (
        "rm -f p1\n"
        "echo hi | cat > p1\n"
        "cat p1\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "hi\n"


def test_pipeline_edge_01():
    script = "echo x | cat | cat | cat | cat | cat\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "x\n"


def test_pipeline_edge_02():
    res = run_shell("echo hi | echo there\n", timeout_s=1.0)
    assert not res.timeout
    # Accept either grammar error or there only
    assert res.returncode in (0, 2)

"""



# =================
# TEST NEGATION
# =================

def test_negation_easy_01():
    res = run_shell("! true\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 1


def test_negation_easy_02():
    res = run_shell("! false\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0


def test_negation_easy_03():
    res = run_shell("! true | false\n", timeout_s=1.0)
    assert not res.timeout
    # true|false -> 1; ! -> 0
    assert res.returncode == 0


def test_negation_easy_04():
    res = run_shell("! false; echo OK\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "OK\n"


def test_negation_medium_01():
    res = run_shell("! false && echo YES\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "YES\n"


def test_negation_medium_02():
    res = run_shell("if ! false; then echo T; else echo F; fi\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "T\n"


def test_negation_medium_03():
    script = "! echo hi |\ncat\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 1  # negation flips status of pipeline => 1
    assert res.stdout == "hi\n"


def test_negation_edge_01():
    # "!echo" should be a word and  not negation
    res = run_shell("!echo hi\n", timeout_s=1.0)
    assert not res.timeout
    # unknown command =>127
    assert res.returncode != 0


def test_negation_edge_02():
    res = run_shell("! false | true\n", timeout_s=1.0)
    assert not res.timeout
    # false|true => 0 then ! => 1
    assert res.returncode == 1


def test_negation_edge_03():
    script = "! false\n! true\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 1


# ================
# WHILE / UNTIL
# ================

def test_while_easy_01():
    # condition false => while does not run
    res = run_shell("while false; do echo X; done\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == ""


def test_while_easy_02():
    # testing the break
    res = run_shell("while true; do echo A; break; done\n", timeout_s=1.0)
    assert not res.timeout


def test_until_easy_01_runs_once_then_stops():
    # until true stops
    res = run_shell("until true; do echo X; done\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == ""


def test_until_easy_02():
    # small loop with no variables
    script = (
        "rm -f u1\n"
        "echo ok > u1\n"
        "until cat u1 | true; do echo NO; done\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout


def test_while_easy_03():
    res = run_shell("while false; do echo hi | cat; done\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0


def test_while_medium_01():
    script = (
        "while false\n"
        "do\n"
        "echo A\n"
        "done\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0


def test_until_medium_01():
    res = run_shell("until false || true; do echo X; done\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == ""


def test_while_medium_02():
    script = (
        "while false; do\n"
        "until true; do echo X; done\n"
        "done\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0


def test_while_edge_01():
    # boucle infini 
    res = run_shell("while true; do true; done\n", timeout_s=0.2)
    assert res.timeout or res.returncode != 0


def test_until_edge_01():
    res = run_shell("until false; do true; done\n", timeout_s=0.2)
    assert res.timeout or res.returncode != 0


def test_while_edge_02():
    #loop with redirections
    script = (
        "rm -f wl\n"
        "while false; do echo X > wl; done\n"
        "test -f wl\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout

# ==================
# TEST && and ||
# ==================

def test_andor_easy_01():
    res = run_shell("true && echo OK\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "OK\n"


def test_andor_easy_02():
    res = run_shell("false && echo NO\n", timeout_s=1.0)
    assert not res.timeout
    assert res.stdout == ""


def test_andor_easy_03():
    res = run_shell("true || echo NO\n", timeout_s=1.0)
    assert not res.timeout
    assert res.stdout == ""


def test_andor_easy_04():
    res = run_shell("false || echo YES\n", timeout_s=1.0)
    assert not res.timeout
    assert res.stdout == "YES\n"


def test_andor_easy_05():
    res = run_shell("true && true && echo OK\n", timeout_s=1.0)
    assert not res.timeout
    assert res.stdout == "OK\n"


def test_andor_medium_01():
    # (true && false) || echo OK
    res = run_shell("true && false || echo OK\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "OK\n"


def test_andor_medium_02():
    script = "true &&\necho OK\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.stdout == "OK\n"


def test_andor_medium_03():
    res = run_shell("echo hi | cat && echo ok\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "hi\nok\n"


def test_andor_edge_01():
    #testing with many operators
    res = run_shell("false || false || true && echo X\n", timeout_s=1.0)
    assert not res.timeout
    assert res.stdout == "X\n"


def test_andor_edge_02():
    script = (
        "rm -f sc\n"
        "false && echo hi > sc\n"
        "test -f sc\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout


def test_andor_edge_03():
    script = (
        "rm -f sc2\n"
        "true || echo hi > sc2\n"
        "test -f sc2\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout



# ======================================
# TEST DOUBLE QUOTES + ESCAPE CHARACTER
# ======================================

"""

def test_quotes_easy_01():
    res = run_shell('echo "hello   world"\n', timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "hello   world\n"


def test_quotes_easy_02():
    res = run_shell('echo "ab"cd\n', timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "abcd\n"


def test_quotes_easy_03():
    res = run_shell("echo hello\\ world\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "hello world\n"


def test_quotes_easy_04():
    res = run_shell("echo \\#escaped\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "#escaped\n"


def test_quotes_easy_05():
    res = run_shell('echo "#quoted"\n', timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "#quoted\n"


def test_quotes_medium_01():
    script = 'i\'f\' true; t"hen" echo OK; f"i"\n'
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "OK\n"


def test_quotes_medium_02():
    script = 'i\'f\' false; t"hen" echo A; e"l""se" echo B; f"i"\n'
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "B\n"


def test_quotes_medium_03():
    res = run_shell('echo a"b"\'c\'d\n', timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "abcd\n"


def test_quotes_edge_0():
    res = run_shell("echo \\\\\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "\\\n"


def test_quotes_edge_02():
    # Must interpret \
    res = run_shell('echo "a\\nb"\n', timeout_s=1.0)
    assert not res.timeout


def test_quotes_edge_03():
    res = run_shell("echo \\#x #comment\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "#x\n"


# ============================================================
# TESTS VARIABLES 
# ============================================================

def test_vars_easy_01():
    res = run_shell("TITO=bar\necho $TITO\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "bar\n"


def test_vars_easy_02():
    res = run_shell("A=hello\necho ${A}\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "hello\n"


def test_vars_easy_03():
    res = run_shell("false\necho $?\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "1\n"


def test_vars_easy_04():
    res = run_shell("echo $$\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout.strip() != ""
    assert res.stdout.strip().isdigit()


def test_vars_easy_05():
    res = run_shell("echo $PWD\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout.strip() != ""


def test_vars_medium_01():
    #tests $# aka number of positional parameters
    res = run_shell("echo $#\n", timeout_s=1.0)
    assert not res.timeout


def test_vars_medium_02():
    # tests $* expands positional params joined
    res = run_shell("echo $*\n", timeout_s=1.0)
    assert not res.timeout


def test_vars_medium_03():
    #tests $UID expands
    res = run_shell("echo $UID\n", timeout_s=1.0)
    assert not res.timeout


def test_vars_edge_01():
    #tests undefined var expansion
    res = run_shell("echo X$UNDEFY\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "X\n"


def test_vars_edge_02():
    # tests ${A} expansion connected directly to the text
    res = run_shell("A=hi\necho ${A}there\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "hithere\n"


def test_vars_edge_03_dollar_qmark_through_pipeline():
    #tests $? reflects last command status
    res = run_shell("true | false\necho $?\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "1\n"

"""

# ============================================================
# TEST FOR LOOPS
# ============================================================

def test_for_easy_01():
    # tests a basic for loop over words: a b c
    res = run_shell("for x in a b c; do echo $x; done\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "a\nb\nc\n"


def test_for_easy_02():
    # tests that the in list can continue on the next line
    script = "for x in a b\nc; do echo $x; done\n"
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout


def test_for_easy_03():
    res = run_shell("for i; do echo X; done\n", timeout_s=1.0)
    assert not res.timeout


def test_for_easy_04():
    # multiple commands inside the loop
    res = run_shell("for x in a; do echo 1; echo 2; done\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "1\n2\n"


def test_for_easy_05():
    res = run_shell("for x in a b; do echo $x | cat; done\n", timeout_s=1.0)
    assert not res.timeout


def test_for_medium_01():
    #tests appending to a file inside the loop
    script = (
        "rm -f f1\n"
        "for x in a b; do echo $x >> f1; done\n"
        "cat f1\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout


def test_for_medium_02():
    ## tests && inside the loop
    res = run_shell("for x in a b; do true && echo $x; done\n", timeout_s=1.0)
    assert not res.timeout


def test_for_medium_03():
    # tests for loop inside an if
    res = run_shell("if true; then for x in a b; do echo $x; done; fi\n", timeout_s=1.0)
    assert not res.timeout


def test_for_edge_01():
    # tests for loop with an empty list
    res = run_shell("for x in ; do echo NO; done\n", timeout_s=1.0)
    assert not res.timeout


def test_for_edge_02():
    res = run_shell("for x in 1 2 3 4 5 6 7 8 9 10; do echo $x; done\n", timeout_s=1.0)
    assert not res.timeout


def test_for_edge_03():
    #tests items with spaces using quotes
    res = run_shell('for x in "a b" c; do echo $x; done\n', timeout_s=1.0)
    assert not res.timeout


# ============================================================
# STEP 2  REMIX
# ============================================================

def test_remix_step2_01_redir_pipe_andor():
    script = (
        "rm -f r1\n"
        "echo hello | cat > r1 && echo OK\n"
        "cat r1\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout


def test_remix_step2_02_vars_quotes_and_comment_escape():
    script = (
        "A=hi\n"
        'echo "$A there"\n'
        "echo \\#escaped # comment\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout


def test_remix_step2_03_if_with_negation_pipeline_and_vars():
    script = (
        "true | false\n"
        "if ! true | false; then echo OK; else echo NO; fi\n"
        "echo $?\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout


def test_remix_step2_04_for_with_redir_and_pipeline():
    script = (
        "rm -f r2\n"
        "for x in a b; do echo $x | cat >> r2; done\n"
        "cat r2\n"
    )
    res = run_shell(script, timeout_s=1.0)
    assert not res.timeout


def test_remix_step2_05_while_guard_timeout():
    res = run_shell("while false; do echo X; done\n", timeout_s=1.0)
    assert not res.timeout







# ============================================================
# TEST EXIT BUILTIN
# ============================================================

def test_exit_easy_01():
    # tests exit with no number => 0
    res = run_shell("exit\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 0


def test_exit_easy_02():
    # tests exit 42 returns 42
    res = run_shell("exit 42\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 42


def test_exit_easy_03():
    # tests exit stops the script
    res = run_shell("echo A\nexit 3\necho B\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode == 3
    assert res.stdout == "A\n"


def test_exit_medium_01():
    # tests exit argument can be expanded
    res = run_shell("X=12\nexit $X\n", timeout_s=1.0)
    assert not res.timeout


def test_exit_medium_02():
    # tests exit 1 2 returns error
    res = run_shell("exit 1 2\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode != 0


def test_exit_hard_01():
    # tests exit abc returns an error
    res = run_shell("exit abc\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode != 0


def test_exit_hard_02():
    # tests large numbers
    res = run_shell("exit 300\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode in (44, 300)


# ============================================================
# TEST CD BUILTIN
# ============================================================

"""


def test_cd_easy_01():
    # tests cd to an absolute path changes directory
    script = (
        "mkdir -p /tmp/42sh_cd_abs\n"
        "cd /tmp/42sh_cd_abs\n"
        "pwd\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout.strip() == "/tmp/42sh_cd_abs"


def test_cd_easy_02():
    # tests cd with a relative path
    script = (
        "mkdir -p /tmp/42sh_cd_rel/a/b\n"
        "cd /tmp/42sh_cd_rel\n"
        "cd a/b\n"
        "pwd\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout.strip() == "/tmp/42sh_cd_rel/a/b"


def test_cd_easy_03():
    # tests cd .. takes us to parent folder
    script = (
        "mkdir -p /tmp/42sh_cd_dd/a\n"
        "cd /tmp/42sh_cd_dd/a\n"
        "cd ..\n"
        "pwd\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout.strip() == "/tmp/42sh_cd_dd"


def test_cd_easy_04():
    # tests PWD and OLDPWD update after cd
    script = (
        "mkdir -p /tmp/42sh_cd_env/a /tmp/42sh_cd_env/b\n"
        "cd /tmp/42sh_cd_env/a\n"
        "echo P:$PWD\n"
        "cd /tmp/42sh_cd_env/b\n"
        "echo P:$PWD\n"
        "echo O:$OLDPWD\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    out = res.stdout.splitlines()
    assert out[0] == "P:/tmp/42sh_cd_env/a"
    assert out[1] == "P:/tmp/42sh_cd_env/b"
    assert out[2] == "O:/tmp/42sh_cd_env/a"


def test_cd_easy_05():
    # tests cd - takes u back to previous directory
    script = (
        "mkdir -p /tmp/42sh_cd_dash/one /tmp/42sh_cd_dash/two\n"
        "cd /tmp/42sh_cd_dash/one\n"
        "cd /tmp/42sh_cd_dash/two\n"
        "cd -\n"
        "pwd\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout.strip().endswith("/tmp/42sh_cd_dash/one")


def test_cd_easy_06():
    # tests cd with no arguments
    res = run_shell("pwd\ncd\npwd\n", timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    lines = res.stdout.splitlines()
    assert len(lines) >= 2
    assert lines[0] != lines[1]


def test_cd_medium_01():
    # tests cd - prints the path it switched to
    script = (
        "mkdir -p /tmp/42sh_cd_dashp/one /tmp/42sh_cd_dashp/two\n"
        "cd /tmp/42sh_cd_dashp/one\n"
        "cd /tmp/42sh_cd_dashp/two\n"
        "cd -\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    assert "/tmp/42sh_cd_dashp/one" in res.stdout


def test_cd_medium_02():
    # tests OLDPWD updates after using cd -
    script = (
        "mkdir -p /tmp/42sh_cd_dash2/a /tmp/42sh_cd_dash2/b\n"
        "cd /tmp/42sh_cd_dash2/a\n"
        "cd /tmp/42sh_cd_dash2/b\n"
        "cd -\n"
        "echo P:$PWD\n"
        "echo O:$OLDPWD\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    out = res.stdout.splitlines()
    assert out[0] == "P:/tmp/42sh_cd_dash2/a"
    assert out[1] == "O:/tmp/42sh_cd_dash2/b"


def test_cd_medium_03():
    # test doing cd - twice switches back and forth
    script = (
        "mkdir -p /tmp/42sh_cd_toggle/a /tmp/42sh_cd_toggle/b\n"
        "cd /tmp/42sh_cd_toggle/a\n"
        "cd /tmp/42sh_cd_toggle/b\n"
        "cd -\n"
        "pwd\n"
        "cd -\n"
        "pwd\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    out = [line.strip() for line in res.stdout.splitlines() if line.strip() != ""]
    assert out[-2] == "/tmp/42sh_cd_toggle/a"
    assert out[-1] == "/tmp/42sh_cd_toggle/b"


def test_cd_hard_01():
    # tests cd to a missing directory fails
    res = run_shell("cd /no/such/dir\n", timeout_s=2.0)
    assert not res.timeout
    assert res.returncode != 0
    assert res.stderr != ""


def test_cd_hard_02():
    # tests cd to a file fails
    script = (
        "rm -f /tmp/42sh_cd_file\n"
        "echo hi > /tmp/42sh_cd_file\n"
        "cd /tmp/42sh_cd_file\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode != 0


def test_cd_hard_03():
    # tests cd - without OLDPWD fail
    res = run_shell("unset OLDPWD\ncd -\n", timeout_s=2.0)
    assert not res.timeout
    assert res.returncode != 0


def test_cd_hard_04():
    # tests $PWD tracks shell path
    script = (
        "mkdir -p /tmp/42sh_pwd_real\n"
        "rm -f /tmp/42sh_pwd_link\n"
        "ln -s /tmp/42sh_pwd_real /tmp/42sh_pwd_link\n"
        "cd /tmp/42sh_pwd_link\n"
        "echo S:$PWD\n"
        "echo E:$(env -i pwd)\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    out = res.stdout.splitlines()
    assert out[0] == "S:/tmp/42sh_pwd_link"
    assert out[1] == "E:/tmp/42sh_pwd_real"

"""


# ============================================================
# BUILTIN EXPORT
# ============================================================

def test_export_easy_01():
    # test export NAME=VALUE appears in env output
    script = "export X=hello\nenv | cat | grep '^X=hello'\n"
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0


def test_export_easy_02():
    # test export NAME after NAME=value keeps the value
    script = "Y=world\nexport Y\nenv | cat | grep '^Y=world'\n"
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout


def test_export_easy_03():
    # tests export can overwrite a value
    script = "export Z=one\nexport Z=two\nenv | cat | grep '^Z=two'\n"
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout


def test_export_medium_01():
    # tests export rejects invalid variable names
    res = run_shell("export 9ABC=bad\n", timeout_s=2.0)
    assert not res.timeout
    assert res.returncode != 0


def test_export_medium_02():
    # tests export NAME with no value
    res = run_shell("export ONLYNAME\n", timeout_s=2.0)
    assert not res.timeout


def test_export_hard_01():
    # tests export with quotes keep spaces
    res = run_shell('export Q="a b"\necho $Q\n', timeout_s=2.0)
    assert not res.timeout


def test_export_hard_02():
    # tests unset removes an exported variable
    res = run_shell("export U=1\nunset U\necho X$U\n", timeout_s=2.0)
    assert not res.timeout


# ============================================================
# BUILTIN BREAK
# ============================================================

def test_break_easy_01():
    # break exits the loop
    res = run_shell("while true; do echo A; break; echo B; done\n", timeout_s=1.0)
    assert not res.timeout
    assert res.stdout == "A\n"


def test_break_easy_02_break_outside_loop_errors():
    # break outside loop returns an error
    res = run_shell("break\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode != 0


def test_break_easy_03_break_in_for_loop():
    #break works in a for loop
    res = run_shell("for x in a b; do echo $x; break; done\n", timeout_s=1.0)
    assert not res.timeout
    assert res.stdout == "a\n"


def test_break_medium():
    # tests break without number break
    script = (
        "for a in 1; do\n"
        "  while true; do echo IN; break; done\n"
        "  echo OUT\n"
        "done\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.stdout == "IN\nOUT\n"


def test_break_hard_01():
    # tests break bigger than loop depth returns error
    res = run_shell("while true; do break 99; done\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode != 0


def test_break_hard_02():
    # tests break with weird returns error
    res = run_shell("while true; do break foo; done\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode != 0


# ============================================================
# TEST BUILTIN CONTINUE
# ============================================================

def test_continue_easy_01():
    # tests continue skip the command after it in the loop
    res = run_shell("for x in a; do echo A; continue; echo B; done\n", timeout_s=1.0)
    assert not res.timeout
    assert res.stdout == "A\n"


def test_continue_easy_02():
    # tests continue outside a loop returns error
    res = run_shell("continue\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode != 0


def test_continue_medium_01():
    # tests continue 2 jumps out of two nested loops
    script = (
        "for a in 1 2; do\n"
        "  for b in 1; do\n"
        "    echo A$a\n"
        "    continue 2\n"
        "    echo NO\n"
        "  done\n"
        "  echo NO2\n"
        "done\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert "NO" not in res.stdout
    assert res.stdout == "A1\nA2\n"


def test_continue_medium_02():
    # tests continue used inside an if
    script = (
        "for x in a; do\n"
        "  if true; then continue; fi\n"
        "  echo NO\n"
        "done\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.stdout == ""


def test_continue_hard_01():
    # tests continue return error
    res = run_shell("for x in a; do continue 99; done\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode != 0


def test_continue_hard_02():
    # tests continue letter returns error
    res = run_shell("for x in a; do continue foo; done\n", timeout_s=1.0)
    assert not res.timeout
    assert res.returncode != 0


# ======================
# BUILTIN DOT
# ======================

def test_dot_easy_01():
    # tests . runs commands from a file
    script = (
        "rm -f /tmp/dot_ok.sh\n"
        "cat > /tmp/dot_ok.sh <<'EOF'\n"
        "echo one\n"
        "echo two\n"
        "EOF\n"
        ". /tmp/dot_ok.sh\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "one\ntwo\n"


def test_dot_easy_02():
    # test returns the status of last command
    script = (
        "rm -f /tmp/dot_rc.sh\n"
        "cat > /tmp/dot_rc.sh <<'EOF'\n"
        "echo ok\n"
        "false\n"
        "EOF\n"
        ". /tmp/dot_rc.sh\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.stdout == "ok\n"
    assert res.returncode == 1


def test_dot_easy_03():
    # tests errors when file does not exist
    res = run_shell(". /no/such/file\n", timeout_s=2.0)
    assert not res.timeout
    assert res.returncode != 0


def test_dot_medium_01():
    # tests can find a file from PATH when no slash
    script = (
        "rm -f /tmp/dotpath\n"
        "cat > /tmp/dotpath <<'EOF'\n"
        "echo PATHOK\n"
        "EOF\n"
        "PATH=/tmp:$PATH\n"
        ". dotpath\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "PATHOK\n"


def test_dot_medium_02():
    # tests on an empty file returns success
    script = "printf \"\\n\\n\" > /tmp/dot_empty.sh\n. /tmp/dot_empty.sh\n"
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0


def test_dot_hard_01():
    # tests variable set in sourced file
    script = (
        "rm -f /tmp/dot_var.sh\n"
        "cat > /tmp/dot_var.sh <<'EOF'\n"
        "A=hello\n"
        "EOF\n"
        ". /tmp/dot_var.sh\n"
        "echo $A\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "hello\n"


def test_dot_hard_02():
    # tests unset inside sourced file
    script = (
        "rm -f /tmp/dot_unset.sh\n"
        "cat > /tmp/dot_unset.sh <<'EOF'\n"
        "unset MYVAR\n"
        "EOF\n"
        "MYVAR=hi\n"
        ". /tmp/dot_unset.sh\n"
        "echo X$MYVAR\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "X\n"


# ============================================================
# TESTING BUILTIN UNSET
# ============================================================

def test_unset_easy_01():
    # tests unset removes a variable
    res = run_shell("A=hi\nunset A\necho X$A\n", timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 0
    assert res.stdout == "X\n"


def test_unset_easy_02():
    # tests unset invalid option returns 127
    res = run_shell("unset -z\n", timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 127


def test_unset_easy_03():
    # invalide name returns code 127
    res = run_shell("unset 9ABC\n", timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 127


def test_unset_medium_01():
    # tests unset still unsets valid names even when one is invalid
    script = "A=1\nB=2\nunset 9BAD A B\necho X$A$B\n"
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert res.stdout == "X\n"


def test_unset_medium_02():
    # tests unset -- treats next args as names
    res = run_shell("unset -- -v\n", timeout_s=2.0)
    assert not res.timeout
    assert res.returncode == 127


def test_unset_hard_01():
    # tests unset -v removes variable
    res = run_shell("MYVAR=hello\nunset -v MYVAR\necho X$MYVAR\n", timeout_s=2.0)
    assert not res.timeout
    assert res.stdout == "X\n"


def test_unset_hard_02():
    # tests unset -f removes a function definition
    script = (
        "foo() { echo OK; }\n"
        "foo\n"
        "unset -f foo\n"
        "foo\n"
    )
    res = run_shell(script, timeout_s=2.0)
    assert not res.timeout
    assert "OK\n" in res.stdout
    assert res.returncode != 0


# ============================================================
# STEP 3 OTHER TEST GO HERE
# ============================================================
