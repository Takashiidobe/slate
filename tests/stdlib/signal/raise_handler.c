#include <signal.h>
#include <stdio.h>
static volatile sig_atomic_t hit = 0;
static void handler(int s) { hit = s; }
int main(void) {
  signal(SIGUSR1, handler);
  raise(SIGUSR1);
  printf("%d\n", hit == SIGUSR1);
  return 0;
}
