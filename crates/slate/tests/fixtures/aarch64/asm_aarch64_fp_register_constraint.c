#include <stdio.h>

typedef float float32x4_t __attribute__((vector_size(16)));

static float copy_via_w(float value) {
  float result;
  __asm__ volatile("fmov %s0, %s1" : "=w"(result) : "w"(value));
  return result;
}

static float32x4_t copy_via_x(float32x4_t value) {
  float32x4_t result;
  __asm__ volatile("orr %0.16b, %1.16b, %1.16b" : "=x"(result) : "x"(value));
  return result;
}

int main(void) {
  float32x4_t value = {2.0f, 3.0f, 5.0f, 7.0f};
  float32x4_t result = copy_via_x(value);
  printf("%.1f %.1f\n", copy_via_w(1.5f), result[3]);
  return 0;
}
