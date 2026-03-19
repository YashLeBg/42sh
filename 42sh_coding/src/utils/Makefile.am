#nom de la librairie a créer
lib_LIBRARIES = libutils.a

#fichiers à compiler
libutils_a_SOURCES = utils.c utils.h
# -I = gcc -I, $(top_srcdir) = fichier source où est configure.ac
libutils_a_CPPFLAGS = -I$(top_srcdir)/src
#flags comme d'hab
libutils_a_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

#on créer la librairie mais sans l'installer
noinst_LIBRARIES = libutils.a
