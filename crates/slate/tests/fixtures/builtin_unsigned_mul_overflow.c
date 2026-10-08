#include <stdio.h>
#include <limits.h>

#define TEST(NAME, T, MAX) do { \
  T r = 0; \
  T values[][2] = {{0, MAX}, {1, MAX}, {MAX, 2}, {MAX, MAX}}; \
  for (unsigned i = 0; i < 4; ++i) { \
    int overflow = NAME(values[i][0], values[i][1], &r); \
    printf("%d %llu\n", overflow, (unsigned long long)r); \
  } \
} while (0)

int main(void) {
  TEST(__builtin_umul_overflow, unsigned, UINT_MAX);
  TEST(__builtin_umull_overflow, unsigned long, ULONG_MAX);
  TEST(__builtin_umulll_overflow, unsigned long long, ULLONG_MAX);
  unsigned r;
  int overflow = __builtin_umul_overflow(0x100000001ULL, -1, &r);
  printf("%d %u\n", overflow, r);
  unsigned a = 3, b = 4, results[2] = {0, 0};
  unsigned *p = results;
  overflow = __builtin_umul_overflow(a++, b++, p++);
  printf("%d %u %u %u %ld\n", overflow, results[0], a, b, (long)(p - results));
  return 0;
}
