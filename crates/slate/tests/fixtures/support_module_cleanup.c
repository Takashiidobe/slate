#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char digits[] = "123";
  printf("%d\n", atoi(digits));
  return 0;
}
// REWRITES-DAG: digits.parse::<i32>().unwrap_or(0)
// REWRITES-NOT: mod __slate_runtime
// REWRITES-NOT: fn atoi(
