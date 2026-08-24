#include <stdio.h>

static int likely_nonzero(int x) {
  if (__builtin_expect(x != 0, 1)) {
    __builtin_assume(x != 0);
    return x + 10;
  }
  return -1;
}

static int assume_true(int x) {
  __builtin_assume(1);
  return x + 1;
}

static int guarded_trap(int x) {
  if (x < 0) {
    __builtin_trap();
  }
  return x;
}

static int guarded_unreachable(int x) {
  if (x < 0) {
    __builtin_unreachable();
  }
  return x * 2;
}

int main(void) {
  volatile int input = 5;
  int          a     = likely_nonzero(input);
  int          b     = assume_true(input);
  int          c     = guarded_trap(input);
  int          d     = guarded_unreachable(input);
  printf("%d %d %d %d\n", a, b, c, d);
  return 0;
}
// REWRITES-LABEL: {{^}}fn likely_nonzero(
// REWRITES-DAG: unsafe { core::hint::assert_unchecked(
// REWRITES: {{^}}}
