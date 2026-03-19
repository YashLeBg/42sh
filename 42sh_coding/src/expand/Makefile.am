#nom de la librairie a créer
lib_LIBRARIES = libexpand.a

#fichiers à compiler
libexpand_a_SOURCES = expand.c expand.h\
                        variables.h variables.c
# -I = gcc -I, $(top_srcdir) = fichier source où est configure.ac
libexpand_a_CPPFLAGS = -I$(top_srcdir)/src
#flags comme d'hab
libexpand_a_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

#on créer la librairie mais sans l'installer
noinst_LIBRARIES = libexpand.a
