#nom de l'éxecutable
bin_PROGRAMS = 42sh

#fichier à compiler
42sh_SOURCES = 42sh.c
# -I%D% = -I.   //// Exemple: gcc -I. hello hello_world.c
42sh_CPPFLAGS = -I%D%
#flags comme d'hab
42sh_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla


#sous-dossiers aussi avec un Makefile.am
SUBDIRS = ast\
	lexer\
	parser

#récupère ce qu'on a compilé dans les sous-dossiers
42sh_LDADD = ast/libast.a lexer/liblexer.a parser/libparser.a
