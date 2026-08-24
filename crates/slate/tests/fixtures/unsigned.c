#include <stdio.h>

static unsigned int uadd(unsigned int a, unsigned int b) {
  unsigned int c = a + b;
  return c;
}
// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: println!("{}

int main(void) {
  unsigned int a = 4000000000u;
  unsigned int b = 500000000u;
  printf("%u\n", uadd(a, b));
  printf("%u\n", a);
  printf("%u\n", b);
  return 0;
}
