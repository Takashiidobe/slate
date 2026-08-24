#include <stdio.h>
#include <stdlib.h>

int *alloc(void) { return malloc(sizeof(int) * 10); }

int *indirect(void) { return alloc(); }

int main(void) {
  int *x = NULL;
  x      = indirect();
  x[0]   = 10;
  printf("%d\n", x[0]);
  free(x);
  return 0;
}
// REWRITES-DAG: fn alloc() -> Vec<i32>
// REWRITES-DAG: fn indirect() -> Vec<i32>
// REWRITES-DAG: let mut x: Vec<i32>
// REWRITES-DAG: x[0] = 10;
// REWRITES-NOT: *mut i32
// REWRITES-NOT: unsafe
