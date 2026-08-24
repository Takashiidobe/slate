#include <stdio.h>

int main(void) {
  int x    = 300;
  int prec = 2;
  printf("%.*hhd\n", prec, x);
  printf("%.*hd\n", prec, x);
  return 0;
}
// REWRITES-DAG: fn printf(
// REWRITES-DAG: unsafe { printf(
// REWRITES-NOT: println!(
