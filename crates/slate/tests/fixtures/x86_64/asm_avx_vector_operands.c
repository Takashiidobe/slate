#include <stdio.h>

typedef float v8sf __attribute__((vector_size(32)));
typedef int v16si __attribute__((vector_size(64)));

static float sqrt_via_x(float value) {
  float result;
  __asm__("sqrtss %1, %0" : "=x"(result) : "x"(value));
  return result;
}

__attribute__((target("avx"))) static float avx_sum(void) {
  v8sf left = {1, 2, 3, 4, 5, 6, 7, 8};
  v8sf right = {10, 20, 30, 40, 50, 60, 70, 80};
  v8sf result;
  __asm__("vaddps %2, %1, %0" : "=x"(result) : "x"(left), "x"(right));
  float sum = 0;
  for (int i = 0; i < 8; i++) {
    sum += result[i];
  }
  return sum;
}

__attribute__((target("avx512f"))) static int avx512_masked_sum(void) {
  v16si left = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  v16si right = {100, 100, 100, 100, 100, 100, 100, 100,
                 100, 100, 100, 100, 100, 100, 100, 100};
  unsigned short mask = 0x00f0;
  v16si result;
  __asm__("vpaddd %2, %1, %0%{%3%}%{z%}"
          : "=v"(result)
          : "v"(left), "v"(right), "Yk"(mask));
  int sum = 0;
  for (int i = 0; i < 16; i++) {
    sum += result[i];
  }
  return sum;
}

__attribute__((target("avx512f"))) static unsigned avx512_equal_lanes(void) {
  v16si left = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16};
  v16si right = {1, 0, 3, 0, 5, 0, 7, 0, 9, 0, 11, 0, 13, 0, 15, 0};
  unsigned short mask;
  __asm__("vpcmpeqd %2, %1, %0" : "=k"(mask) : "v"(left), "v"(right));
  return mask;
}

int main(void) {
  printf("%.1f\n", sqrt_via_x(6.25f));
  if (__builtin_cpu_supports("avx")) {
    printf("%.1f\n", avx_sum());
  }
  if (__builtin_cpu_supports("avx512f")) {
    printf("%d %#x\n", avx512_masked_sum(), avx512_equal_lanes());
  }
  return 0;
}
