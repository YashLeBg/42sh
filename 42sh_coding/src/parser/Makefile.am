#nom de la librairie a créer
lib_LIBRARIES = libparser.a

#fichiers à compiler
libparser_a_SOURCES = parser.c parser.h\
                    list.h list.c\
                    and_or.h and_or.c\
                    command.h command.c\
                    compound_list.c compound_list.h\
                    else_clause.h else_clause.c\
                    pipeline.h pipeline.c\
                    rule_if.c rule_if.h\
                    shell_command.h shell_command.c\
                    simple_command.c simple_command.h

# -I = gcc -I, $(top_srcdir) = fichier source où est configure.ac
libparser_a_CPPFLAGS = -I$(top_srcdir)/src
#flags comme d'hab
libparser_a_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

#on créer la librairie mais sans l'installer
noinst_LIBRARIES = libparser.a
