#initialiser le projet
AC_INIT([deuteragonist], [1.0], [antonin.bessieres @epita.fr])

#préviens que l'on va générer des Makefiles, foreign pour moins de restrictions
AM_INIT_AUTOMAKE([subdir - objects], [foreign])

#pretty - print pour les Makefiles
AM_SILENT_RULES([yes])

#détecte le présence d'un outils pour la création de librairies statiques (les .a)
AM_PROG_AR

#pour créer des librairies
AC_PROG_RANLIB

#détecte le compilateur C
AC_PROG_CC

#les flags classiques
AX_COMPILER_FLAGS([], [], [],
                  [-std = c99 - pedantic - Werror - Wall - Wextra - Wvla])

#makefile à faire
AC_CONFIG_FILES([Makefile src / Makefile src / ast / Makefile src / lexer /
                 Makefile src / parser /
    Makefile])

#crée les fichiers
AC_OUTPUT
