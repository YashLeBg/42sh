#nom de la librairie a créer
lib_LIBRARIES = libast.a

#fichiers à compiler
libast_a_SOURCES = ast.c ast.h
# -I = gcc -I, $(top_srcdir) = fichier source où est configure.ac
libast_a_CPPFLAGS = -I$(top_srcdir)/src
#flags comme d'hab
libast_a_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

#on créer la librairie mais sans l'installer
noinst_LIBRARIES = libast.a
