#include <stdio.h>
#include <stdlib.h>

int compute(int flag) {
  int *p = NULL;
  if (flag) {
    p = malloc(sizeof(int));
  }
  if (p) {
    *p    = 41;
    int v = *p;
    printf("%d\n", v);
    free(p);
  } else {
    printf("no alloc\n");
  }
  return 0;
}

int main(void) {
  compute(1);
  compute(0);
  return 0;
}
// REWRITES-DAG: let mut p: Option<Box<i32>> = None;
// REWRITES-DAG: p = Some(Box::<i32>::new(0));
// REWRITES-DAG: match p.take() {
// REWRITES-DAG: *p = 41;
// REWRITES-DAG: let v: i32 = *p;
// REWRITES-NOT: fn free(
// REWRITES-NOT: fn malloc(
// REWRITES-NOT: std::ptr::null_mut()
// REWRITES-NOT: unsafe
