/* { dg-options "-fdefer-ts" } */
#include <stdio.h>
#include <stddefer.h>

static int unwind(int limit) {
  int total = 0;
  for (int i = 0; i < limit; i++) {
    defer total += i;
    if (i == 2)
      continue;
    defer printf("leave %d\n", i);
  }
  return total;
}

int main(void) {
  {
    defer puts("outer");
    puts("inner");
  }
  printf("%d %d\n", unwind(4), __STDC_DEFER_TS25755__ > 0);
  return 0;
}
