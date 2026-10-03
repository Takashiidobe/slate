#include <stdio.h>
#include <x86intrin.h>

static unsigned long long elapsed(void) {
  unsigned long long start = __rdtsc();
  volatile int       sink  = 0;
  for (int i = 0; i < 1000; i++) {
    sink += i;
  }
  return _rdtsc() - start;
}

int main(void) {
  unsigned long long ticks = elapsed();
  printf("%d\n", ticks > 0 && ticks < (1ull << 40));
  return 0;
}
