#include <stdio.h>
#include <stdlib.h>

static void just_free(int *p) { free(p); }

int main(void) {
  int *x = malloc(sizeof(int));
  *x = 4;
  printf("%d\n", *x);
  just_free(x);
  return 0;
}
// REWRITES-DAG: fn just_free(arg0: *mut i32)
// REWRITES-DAG: fn free(
// REWRITES-DAG: unsafe { free((p as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-DAG: just_free(x);
// REWRITES-NOT: Box<i32>
