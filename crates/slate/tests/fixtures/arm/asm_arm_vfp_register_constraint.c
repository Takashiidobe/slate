#include <stdio.h>

static float copy_via_t(float value) {
  float result;
  __asm__ volatile("vmov.f32 %0, %1" : "=t"(result) : "t"(value));
  return result;
}

static double copy_via_w(double value) {
  double result;
  __asm__ volatile("fcpyd %0, %1" : "=w"(result) : "w"(value));
  return result;
}

static double copy_via_x(double value) {
  double result;
  __asm__ volatile("fcpyd %0, %1" : "=x"(result) : "x"(value));
  return result;
}

int main(void) {
  printf("%.1f %.1f %.1f\n", copy_via_t(1.5f), copy_via_w(2.5), copy_via_x(3.5));
  return 0;
}
