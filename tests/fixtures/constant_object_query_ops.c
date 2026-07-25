#include <stdio.h>

static int global_array[4];

int main(void) {
  int local[6];
  int *p = local;
  volatile int v = 3;

  int c_literal = __builtin_constant_p(42);
  int c_expr = __builtin_constant_p(7 + 5);
  int c_volatile = __builtin_constant_p(v);

  unsigned long local_whole = __builtin_object_size(local, 0);
  unsigned long local_remaining = __builtin_object_size(&local[2], 1);
  unsigned long global_whole = __builtin_object_size(global_array, 0);
  unsigned long unknown = __builtin_object_size(p, 0);
  unsigned long unknown_upper = __builtin_object_size(p, 2);

  printf("%d %d %d %lu %lu %lu %lu %lu\n", c_literal, c_expr, c_volatile,
         local_whole, local_remaining, global_whole, unknown, unknown_upper);
  return 0;
}
