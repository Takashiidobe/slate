#include <stdio.h>

static long long lladd(long long a, long long b) {
  long long c = a + b;
  return c;
}
// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: println!("{}

int main(void) {
  long long          a  = 9000000000000LL;
  unsigned long long ua = 18000000000000ULL;
  printf("%lld\n", lladd(a, 1));
  printf("%llu\n", ua);
  return 0;
}
