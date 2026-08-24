#include <stdio.h>
#include <stdlib.h>

int *alloc(void) { return malloc(sizeof(int) * 10); }

int main(void) {
  int *x = NULL;
  x      = alloc();
  x[0]   = 10;
  printf("%d\n", x[0]);
  free(x);
  return 0;
}
// REWRITES-DAG: fn alloc() -> Vec<i32>
// REWRITES-DAG: vec![0; 10usize]
// REWRITES-DAG: let mut x: Vec<i32>
// REWRITES-DAG: x[0] = 10;
// REWRITES-DAG: println!("{}", x[0]);
// REWRITES-NOT: *mut i32
// REWRITES-NOT: unsafe
