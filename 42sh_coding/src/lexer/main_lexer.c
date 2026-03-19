#include "lexer.h"

int main(int argc, char **argv)
{
    struct Lexer *lexer = create_lexer(argc, argv);
    pretty_print_lexer(lexer);
}
