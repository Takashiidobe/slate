#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p      = malloc(sizeof(int));
  int  marker = 7;
  marker      = marker + 1;
  *p          = 41;
  printf("%d %d\n", *p, marker);
  free(p);
  return 0;
}
// REWRITES-DAG: let p: Box<i32> = Box::<i32>::new(41);
// REWRITES-DAG: let mut marker: i32 = 7;
// REWRITES-DAG: marker += 1;
// REWRITES-NOT: *p = 41;
