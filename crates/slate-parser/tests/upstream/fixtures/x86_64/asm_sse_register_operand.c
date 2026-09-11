#include <stdio.h>

static double add_via_x(double a, double b) {
  double result;
  __asm__("addsd %2, %0" : "=x"(result) : "0"(a), "x"(b));
  return result;
}

static int roundtrip_via_x(int a) {
  int mid, result;
  __asm__("movd %1, %0" : "=x"(mid) : "r"(a));
  __asm__("movd %1, %0" : "=r"(result) : "x"(mid));
  return result;
}

int main(void) {
  printf("%.2f %d\n", add_via_x(1.5, 2.25), roundtrip_via_x(42));
  return 0;
}


