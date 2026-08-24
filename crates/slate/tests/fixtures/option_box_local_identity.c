#include <stdio.h>
#include <stdlib.h>

int compute(int flag) {
  int *p = NULL;
  int *q = NULL;
  if (flag) {
    p = malloc(sizeof(int));
    q = malloc(sizeof(int));
  }
  if (p == q) {
    printf("same\n");
  } else {
    printf("diff\n");
  }
  if (p) {
    *p = 1;
    printf("%d\n", *p);
    free(p);
  }
  if (q) {
    *q = 2;
    printf("%d\n", *q);
    free(q);
  }
  return 0;
}

int main(void) {
  compute(1);
  compute(0);
  return 0;
}
// REWRITES-DAG: let mut p: Option<Box<i32>> = None;
// REWRITES-DAG: let mut q: Option<Box<i32>> = None;
// REWRITES-DAG: std::ptr::eq(
// REWRITES-DAG: .as_deref().map_or(std::ptr::null(),
// REWRITES-NOT: p == q
// REWRITES-NOT: fn malloc(
// REWRITES-NOT: unsafe
