#include <stdio.h>

typedef float float32x4_t __attribute__((vector_size(16)));

static float32x4_t add_via_x(float32x4_t lhs, float32x4_t rhs) {
  float32x4_t result;
  __asm__ volatile("addps %1, %0" : "=x"(result) : "0"(lhs), "x"(rhs));
  return result;
}

int main(void) {
  float32x4_t result = add_via_x((float32x4_t){1.0f, 2.0f, 3.0f, 4.0f},
                                 (float32x4_t){5.0f, 6.0f, 7.0f, 8.0f});
  printf("%.1f %.1f %.1f %.1f\n", result[0], result[1], result[2], result[3]);
  return 0;
}


