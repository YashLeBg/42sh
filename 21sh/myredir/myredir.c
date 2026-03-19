#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// Yo, petite explication du code avec les commentaires

int main(int argc, char **argv) {
  // moins de 3 arguments = manque soit la commande soit le fichier de
  // redirection
  if (argc < 3) {
    errx(2, "missing some argument");
  }

  // ouvre ou créer le fichier demandé
  int file_open = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0644);

  // départ de la fork pour executé la commande
  pid_t pid = fork();

  if (pid == 0) {
    // dup2 pour rediriger ce qui sort sur stdout dans le fichier demandé
    if (dup2(file_open, STDOUT_FILENO) < 0) {
      perror("fail dup2");
      close(file_open);
      exit(127);
    }
    // dup2 copie le file descriptor donc on ferme celui ouvert avant (début du
    // programme)
    close(file_open);
    // on éxecute la commande demander
    execvp(argv[2], &argv[2]);
    // si la commande est trouver elle remplace le programme (cf trove) donc
    // message d'erreur  si execvp ne trouve pas la commande
    fprintf(stderr, "myredir: %s: command not found\n", argv[2]);
    exit(127);
  }

  // l'enfant a fermé le fichier mais pas le parent donc on ferme
  close(file_open);

  // on atends l'enfant et on récupère sa sortie (stderr)
  int status;
  waitpid(pid, &status, 0);

  // vérifie que l'enfant s'est terminé normalement
  if (WIFEXITED(status)) {
    // on récupére le numéro de l'erreur
    int code = WEXITSTATUS(status);

    // 127 = erreur que l'on doit gérer donc on return 1
    if (code == 127) {
      return 1;
    }

    // erreur que l'on ne doit pas gérer message avec le code renvoyer pas
    // execpv et fin du programme normal avec 0
    printf("%s exited with %d!\n", argv[2], code);
    return 0;
  }

  // l'enfant a fermé bizzarement = erreur
  return 1;
}
