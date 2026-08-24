#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = calloc(4, sizeof(int));
  printf("%d\n", p[0] + p[3]);
  free(p);
  return 0;
}
// REWRITES-DAG: let p: Vec<i32> = vec![0; 4usize];
// REWRITES-DAG: println!("{}", p[0] + p[3]);
// REWRITES-NOT: fn calloc(
// REWRITES-NOT: fn free(
// REWRITES-NOT: .add(
