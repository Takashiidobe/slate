#include <stdio.h>
#include <stdlib.h>

int compute(int flag) {
  int *p = NULL;
  if (flag) {
    p = malloc(sizeof(int) * 4);
  }
  if (p) {
    p  = p + 1;
    *p = 41;
    printf("%d\n", *p);
    free(p - 1);
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
// REWRITES-DAG: let mut p: *mut i32 = std::ptr::null_mut();
// REWRITES-DAG: .add(1)
// REWRITES-DAG: fn free(
// REWRITES-NOT: Option<Box
