#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof(int) * 3);
  p[0]   = 1;
  p[1]   = 2;
  p[2]   = 3;
  printf("%d\n", p[0] + p[1] + p[2]);
  free(p);
  return 0;
}
// REWRITES-DAG: let mut p: Vec<i32> = vec![0; 3usize];
// REWRITES-DAG: p[0] = 1;
// REWRITES-DAG: p[1] = 2;
// REWRITES-DAG: p[2] = 3;
// REWRITES-NOT: let _v
// REWRITES-NOT: fn malloc(
// REWRITES-NOT: fn free(
