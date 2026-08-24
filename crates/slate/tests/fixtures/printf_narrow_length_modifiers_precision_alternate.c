#include <stdio.h>

int main(void) {
  int          a = 300;
  int          b = -5;
  unsigned int c = 400;
  printf("%.2hhd %.3hd %#hhx %08hhx\n", a, b, a, a);
  printf("%.4hho %.2hhx %hhu\n", a, a, c);
  printf("%.5hd %#hx\n", b, c);
  return 0;
}
// REWRITES-NOT: fn printf(
// REWRITES-NOT: unsafe { printf(
// REWRITES-DAG: let __slate_printf_arg = (a as u8) as i8;
// REWRITES-DAG: let __slate_printf_arg = (b as u16) as i16;
// REWRITES-DAG: format!("{:04o}", a as u8)
// REWRITES-DAG: format!("{:02x}", a as u8)
