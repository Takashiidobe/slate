
#include <stdio.h>

static int cached_value = 0;
static int computed     = 0;

static int compute(void) { return 42; }

int get_value(void) {
  if (!computed) {
    cached_value = compute();
    computed     = 1;
  }
  return cached_value;
}

int main(void) {
  printf("%d\n", get_value());
  printf("%d\n", get_value());
  return 0;
}
