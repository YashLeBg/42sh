#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int exec_pipe(char **argv_left, char **argv_right) {
  // servira plus tard pour le pipe
  int fds[2];
  // PID pour les deux forks (enfants)
  pid_t left_pid, right_pid;
  // status de fin de la fork right
  int status_right = 1;
  // création du pipe (fds[0] = lecture, fds[1] = écriture)
  if (pipe(fds) == -1) {
    return 1;
  }
  // création de la première fork (left)
  left_pid = fork();
  // échec de création
  if (left_pid == -1) {
    // close la pipe et quitte
    close(fds[0]);
    close(fds[1]);
    return 1;
  }
  if (left_pid == 0) {
    // redirection stdout -> fds[1]
    if (dup2(fds[1], STDOUT_FILENO) == -1) {
      // échec de la redirection
      return 1;
    }
    // close la pipe de la fork left
    close(fds[0]);
    close(fds[1]);
    // éxecuter la commande demandé, comme stdout = fds[1], écriture dans la
    // pipe
    execvp(argv_left[0], argv_left);
    // si la commande réussi elle remplace la fork, sinon on revient ici
    return 1;
  }
  // création de la deuxième fork (right)
  right_pid = fork();
  // échec de création
  if (right_pid == -1) {
    // close la pipe et attendre la fin de la fork left
    close(fds[0]);
    close(fds[1]);
    waitpid(left_pid, NULL, 0);
    return 1;
  }
  if (right_pid == 0) {
    // redirection fds[0] -> stdin
    if (dup2(fds[0], STDIN_FILENO) == -1) {
      return 1;
    }
    // close la pipe de la fork right
    close(fds[0]);
    close(fds[1]);
    // on éxecute la deuxième commande
    execvp(argv_right[0], argv_right);
    return 1;
  }
  // close la pipe du parent
  close(fds[0]);
  close(fds[1]);
  // on attend la fork right
  if (waitpid(right_pid, &status_right, 0) == -1) {
    status_right = 1;
  }
  // de même pour la fork left
  waitpid(left_pid, NULL, 0);
  // si la fork right termine normalemt, on récupère sa valeur de sortie
  if (WIFEXITED(status_right)) {
    // on return le status de la fork right
    return WEXITSTATUS(status_right);
  } else
    // return 1 si les forks ne termine pas normalement
    return 1;
}
