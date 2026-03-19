#nom de la librairie a créer
lib_LIBRARIES = libexec.a

#fichiers à compiler
libexec_a_SOURCES = exec.c exec.h
# -I = gcc -I, $(top_srcdir) = fichier source où est configure.ac
libexec_a_CPPFLAGS = -I$(top_srcdir)/src
#flags comme d'hab
libexec_a_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

#on créer la librairie mais sans l'installer
noinst_LIBRARIES = libexec.a
