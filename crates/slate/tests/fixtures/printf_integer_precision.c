#include <stdio.h>

int main(void) {
  int          a   = 5;
  int          neg = -5;
  unsigned int u   = 5u;
  unsigned int hex = 255u;
  printf("%.3d %.3d\n", a, neg);
  printf("%8.3d|%-8.3d|%+.3d\n", a, neg, a);
  printf("%08.3d\n", neg);
  printf("%.3u %.4x %.4X %.4o\n", u, hex, hex, hex);
  return 0;
}
// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: format!("-{:03}", __slate_printf_arg.unsigned_abs())
// REWRITES-DAG: format!("{:>8}",
// REWRITES-DAG: format!("{:<8}",
// REWRITES-DAG: format!("+{:03}", __slate_printf_arg)
// REWRITES-DAG: format!("{:04x}", hex)
