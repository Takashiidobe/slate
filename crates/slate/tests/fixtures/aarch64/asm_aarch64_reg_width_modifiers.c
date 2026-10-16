#include <stdio.h>

// @check-fn-begin
static int inc32_via_r(int x) {
  __asm__("add %0, %0, #1" : "+r"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static long inc64_via_r(long x) {
  __asm__("add %0, %0, #1" : "+r"(x));
  return x;
}
// @check-fn-end

// @check-fn-begin
static long inc_w_then_x(long x) {
  __asm__("add %w0, %w0, #1\n\tadd %x0, %x0, #0" : "+r"(x));
  return x;
}
// @check-fn-end

int main(void) {
  printf("%d %ld %ld\n", inc32_via_r(41), inc64_via_r(41),
         inc_w_then_x(0xFFFFFFFF00000005L));
  return 0;
}
