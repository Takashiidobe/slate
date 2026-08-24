#include <stdio.h>

int main(void) {
  double a = 3.14159;
  double b = -3.14159;
  printf("%8.2f|%+8.2f|%08.2f|%-8.2f|%+.2f\n", a, a, b, a, b);
  return 0;
}
// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: println!("{:8.2}|{:+8.2}|{:08.2}|{:<8.2}|{:+.2}", a, a, b, a, b);
