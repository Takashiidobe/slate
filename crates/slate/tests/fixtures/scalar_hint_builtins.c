#include <stdio.h>

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

static int calls;

static int next(void) { return ++calls; }

static long weigh(const unsigned long long *words, int count) {
  long total = 0;
  for (int i = 0; i < count; i++) {
    __builtin_prefetch(words + i + 4);
    __builtin_prefetch(words + i + 8, 0, 3);
    unsigned long long word = words[i];
    if (unlikely(word == 0)) continue;
    total += __builtin_popcountll(word) * 100 + __builtin_ctzll(word);
    if (likely(word & 1)) total += __builtin_popcountl((unsigned long)(word >> 32));
    total += __builtin_ctz((unsigned)word | 0x80000000u) + __builtin_popcount((unsigned)word);
    total += __builtin_ctzl((unsigned long)word);
  }
  return total;
}

int main(void) {
  unsigned long long words[12] = {
      0, 1, 0x8000000000000000ull, 0xffffffffffffffffull, 0x00f0f0f0f0f0f000ull, 12345,
      0x9e3779b97f4a7c15ull, 2, 0x100000000ull, 0x7fffffffull, 6, 0x5555555555555555ull,
  };
  printf("%ld\n", weigh(words, 12));
  long e = __builtin_expect(next(), 1) + __builtin_expect(next() * 10, 20);
  printf("%ld %d\n", e, calls);
  if (__builtin_expect_with_probability(calls == 2, 1, 0.9)) printf("probable\n");
  return 0;
}
