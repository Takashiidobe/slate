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
// REWRITES-DAG: fn use_and_free(mut arg0: Box<i32>) -> i32
// REWRITES-DAG: unsafe { touch(y) };
// REWRITES-NOT: unsafe { free(
