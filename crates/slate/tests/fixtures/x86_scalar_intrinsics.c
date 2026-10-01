#include <stdio.h>

static int instructions(void) {
  unsigned long long flags = __builtin_ia32_readeflags_u64();
  __builtin_ia32_writeeflags_u64(flags);
  __builtin_ia32_pause();
  __builtin_ia32_lfence();
  unsigned long long first = __builtin_ia32_rdtsc();
  __builtin_ia32_mfence();
  unsigned long long second = __builtin_ia32_rdtsc();
  __builtin_ia32_sfence();
  return first != 0 && second >= first;
}

int main(void) {
  printf("%d\n", instructions());
  return 0;
}
