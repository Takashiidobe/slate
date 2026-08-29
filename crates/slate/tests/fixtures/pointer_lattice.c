#include <stdio.h>
#include <stdlib.h>

static void bump(int *p) {
  *p = *p + 1;
}

static int peek(int *p) {
  return *p + 1;
}

static int use_and_free(int *y) {
  *y = *y + 1;
  int v = *y;
  free(y);
  return v;
}

int main(void) {
  int a = 1;
  bump(&a);

  int b = 10;
  int peeked = peek(&b);

  int *c = malloc(sizeof(int));
  *c = 100;
  int v = use_and_free(c);

  printf("%d %d %d\n", a, peeked, v);
  return a + peeked + v;
}
// REWRITES-DAG: fn bump(arg2: &mut i32)
// REWRITES-DAG: fn peek(arg1: &i32) -> i32
// REWRITES-DAG: fn use_and_free(mut arg0: Box<i32>) -> i32
// REWRITES-NOT: fn use_and_free({{.*}}*mut i32
// REWRITES-NOT: free(
