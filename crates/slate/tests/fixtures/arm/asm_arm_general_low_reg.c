#include <stdio.h>

// @check-fn-begin
static int inc_via_r(int x) {
  __asm__("add %0, %0, #1" : "+r"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static int inc_via_l(int x) {
  __asm__("add %0, %0, #1" : "+l"(x));
  return x;
}
// @check-fn-end

int main(void) {
  printf("%d %d\n", inc_via_r(41), inc_via_l(41));
  return 0;
}
