#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = malloc(sizeof(int));
  *p     = 41;
  *p     = *p + 1;
  printf("%d\n", *p);
  free(p);
  return 0;
}
// REWRITES-DAG: let mut p: Box<i32> = Box::<i32>::new(41);
// REWRITES-DAG: *p = *p + 1;
// REWRITES-DAG: println!("{}", *p);
// REWRITES-NOT: fn malloc(
// REWRITES-NOT: fn free(
// REWRITES-NOT: *p = 41;
