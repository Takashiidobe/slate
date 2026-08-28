#include <stdio.h>
#include <stdlib.h>

void touch(int *y) { *y += 1; }

static int use_and_free(int *y) {
  touch(y);
  int v = *y;
  free(y);
  return v;
}

int main(void) {
  int *y = malloc(sizeof(int));
  *y = 41;
  printf("%d\n", use_and_free(y));
  return 0;
}
// REWRITES-DAG: fn use_and_free(arg0: *mut i32)
// REWRITES-DAG: unsafe { touch(y) };
// REWRITES-DAG: unsafe { free((y as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NOT: Box<i32>
