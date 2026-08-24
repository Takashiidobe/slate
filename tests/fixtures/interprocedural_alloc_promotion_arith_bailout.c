#include <stdio.h>
#include <stdlib.h>

int *alloc(void) { return malloc(sizeof(int) * 10); }

int main(void) {
  int *x = NULL;
  x      = alloc();
  x      = x + 1;
  *x     = 10;
  printf("%d\n", *x);
  free(x - 1);
  return 0;
}
// REWRITES-DAG: fn alloc() -> *mut i32
// REWRITES-DAG: fn free(
// REWRITES-DAG: unsafe { free(
// REWRITES-NOT: Vec<i32>
