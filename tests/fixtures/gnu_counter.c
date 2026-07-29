#include <stdio.h>

#define NEXT_COUNTER() __COUNTER__
#define COUNTER_PAIR(a, b) ((a) * 10 + (b))

static int direct_zero = __COUNTER__;
static int macro_one = NEXT_COUNTER();

#if 0
static int ignored = __COUNTER__;
#endif

static int pair = COUNTER_PAIR(__COUNTER__, NEXT_COUNTER());

int main(void) {
  int local_four = __COUNTER__;
  printf("%d %d %d %d\n", direct_zero, macro_one, pair, local_four);
  return 0;
}
