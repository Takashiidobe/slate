#include <stdio.h>
#include <stdint.h>

#define TEST(W, T, X) do { \
  T x = (T)(X); \
  unsigned counts[] = {0, 1, W - 1, W, W + 1, 255}; \
  for (unsigned i = 0; i < 6; ++i) { \
    unsigned n = counts[i]; \
    printf("%u %u %llu %llu\n", W, n, \
      (unsigned long long)__builtin_rotateleft##W(x, n), \
      (unsigned long long)__builtin_rotateright##W(x, n)); \
  } \
} while (0)

int main(void) {
  TEST(8, uint8_t, 0x93);
  TEST(16, uint16_t, 0x9137);
  TEST(32, uint32_t, 0x9137abcd);
  TEST(64, uint64_t, 0x9137abcdef012345ULL);
  unsigned x = 3, n = 1;
  unsigned r = __builtin_rotateleft32(x++, n++);
  printf("%u %u %u\n", r, x, n);
  return 0;
}
