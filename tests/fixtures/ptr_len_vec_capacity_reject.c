#include <stdio.h>
#include <stdlib.h>

static int consume_extra_capacity(int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i) {
    values[i] = i + 1;
    sum += values[i];
  }
  free(values);
  return sum;
}

int main(void) {
  int len = 3;
  int *values = malloc((len + 1) * sizeof(int));
  int sum = consume_extra_capacity(values, len);
  printf("%d\n", sum);
  return 0;
}

// REWRITES: fn consume_extra_capacity(arg{{[0-9]+}}: *mut i32, arg{{[0-9]+}}: i32) -> i32
// REWRITES-NOT: Vec::from_raw_parts(
