#nom de la librairie a créer
lib_LIBRARIES = liblexer.a

#fichiers à compiler
liblexer_a_SOURCES = lexer.c lexer.h
# -I = gcc -I, $(top_srcdir) = fichier source où est configure.ac
liblexer_a_CPPFLAGS = -I$(top_srcdir)/src
#flags comme d'hab
liblexer_a_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

#on créer la librairie mais sans l'installer
noinst_LIBRARIES = liblexer.a
