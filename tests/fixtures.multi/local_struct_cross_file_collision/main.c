#include <stdio.h>

int run_a(void);
int run_b(void);

int main(void) {
  printf("%d %d\n", run_a(), run_b());
  return 0;
}
