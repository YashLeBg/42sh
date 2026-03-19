#nom de la librairie a créer
lib_LIBRARIES = libbuiltins.a

#fichiers à compiler
libbuiltins_a_SOURCES = echo.c echo.h cd.c cd.h exit_builtin.c exit_builtin.h
# -I = gcc -I, $(top_srcdir) = fichier source où est configure.ac
libbuiltins_a_CPPFLAGS = -I$(top_srcdir)/src
#flags comme d'hab
libbuiltins_a_CFLAGS = -std=c99 -pedantic -Werror -Wall -Wextra -Wvla

#on créer la librairie mais sans l'installer
noinst_LIBRARIES = libbuiltins.a
