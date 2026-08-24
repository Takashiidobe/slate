#include <stdio.h>

static long ladd(long a, long b) {
  long c = a + b;
  return c;
}
// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: println!("{}

int main(void) {
  long          a  = 5000000000L;
  unsigned long ua = 9000000000UL;
  printf("%ld\n", ladd(a, 1));
  printf("%lu\n", ua);
  return 0;
}
