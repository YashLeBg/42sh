#include <stdbool.h>
#include <stdio.h>

void print_not_back(char *str) {
  while (*str != '\0') {
    if (*str == '\\') {
      putchar('\\');
    } else if (*str == '\n') {
      putchar('\\');
      putchar('n');
    } else if (*str == '\t') {
      putchar('\\');
      putchar('t');
    } else {
      putchar(*str);
    }
    str++;
  }
}

int echo(int argc, char **argv) {
  int e = true;
  int n = false;
  int i = 0;
  while (i < argc && argv[i][0] == '-' && argv[i][1] != '\0') {
    char *current = argv[i] + 1;
    int option = true;

    for (char *verif = current; *verif; verif++) {
      if (*verif != 'e' && *verif != 'E' && *verif != 'n') {
        option = false;
        break;
      }
    }
    if (!option) {
      break;
    }
    for (; *current; current++) {
      if (*current == 'e') {
        e = true;
      } else if (*current == 'E') {
        e = false;
      } else {
        n = true;
      }
    }
    i++;
  }
  for (; i < argc; i++) {
    if (e) {
      printf("%s", argv[i]);
    } else {
      print_not_back(argv[i]);
    }
    if (i + 1 < argc) {
      putchar(' ');
    }
  }
  if (!n) {
    putchar('\n');
  }
  return 0;
}

int main(void) {
  char *argv[] = {"-nE", "kk\npp\np"};
  echo(2, argv);
}