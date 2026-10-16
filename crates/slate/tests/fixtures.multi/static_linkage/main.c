#include <stdio.h>

static int base = 10;
static int local(int x) { return x + base; }

int compute(int x);

int main(void) {
  printf("%d %d\n", local(5), compute(3));
  return 0;
}
