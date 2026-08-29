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
// REWRITES-DAG: fn just_free(mut arg0: Box<i32>)
// REWRITES-DAG: just_free(unsafe { Box::from_raw(
// REWRITES-NOT: unsafe { free(
