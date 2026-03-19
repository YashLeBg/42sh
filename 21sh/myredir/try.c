#include <err.h>
#include <unistd.h>
int main(int argc, char **argv) {
  execvp(argv[1], argv + 1);
  err(1, "execvp");
  return 1;
}
