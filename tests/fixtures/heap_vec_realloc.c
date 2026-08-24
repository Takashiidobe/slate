#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof(int) * 2);
  p[0]   = 1;
  p[1]   = 2;
  p      = realloc(p, sizeof(int) * 4);
  p[2]   = 3;
  p[3]   = 4;
  printf("%d\n", p[0] + p[1] + p[2] + p[3]);
  free(p);
  return 0;
}
// REWRITES-DAG: let mut p: Vec<i32> = vec![0; 2usize];
// REWRITES-DAG: p.resize(4usize, 0);
// REWRITES-DAG: p[2] = 3;
// REWRITES-DAG: p[3] = 4;
// REWRITES-NOT: fn realloc(
// REWRITES-NOT: unsafe { realloc(
// REWRITES-NOT: .add(
