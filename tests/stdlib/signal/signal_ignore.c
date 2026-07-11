#include <signal.h>
#include <stdio.h>
int main(void) {
  void (*prev)(int) = signal(SIGUSR1, SIG_IGN);
  int r = raise(SIGUSR1);
  printf("%d %d\n", prev != SIG_ERR, r == 0);
  return 0;
}
