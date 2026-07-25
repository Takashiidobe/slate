#include <stdio.h>
#include <stdlib.h>

_Noreturn void die(int code) {
  printf("dying with %d\n", code);
  exit(code);
}

int main(void) {
  if (0) {
    die(1);
  }
  printf("main\n");
  return 0;
}
