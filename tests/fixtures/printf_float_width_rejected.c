#include <stdio.h>

int main(void) {
  double x    = 3.14159;
  int    prec = 2;
  printf("%.*f\n", prec, x);
  return 0;
}
// REWRITES-DAG: fn printf(
// REWRITES-DAG: unsafe { printf(
// REWRITES-NOT: println!(
