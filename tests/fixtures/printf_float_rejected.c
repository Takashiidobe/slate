#include <stdio.h>

int main(void) {
  double x = 1.25;
  printf("%a\n", x);
  return 0;
}
// REWRITES-DAG: fn printf(
// REWRITES-DAG: unsafe { printf(
// REWRITES-NOT: println!("{:.
