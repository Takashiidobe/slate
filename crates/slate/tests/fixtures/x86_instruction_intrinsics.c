#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

__attribute__((target("sse4.2"))) static unsigned long long crc32_probe(void) {
  unsigned int crc = _mm_crc32_u8(0u, 0x12u);
  crc = _mm_crc32_u16(crc, 0x3456u);
  crc = _mm_crc32_u32(crc, 0x789abcdeu);
  return _mm_crc32_u64(crc, 0x123456789abcdef0ull);
}

int main(void) {
  unsigned int auxiliary = 0;
  __builtin_ia32_pause();
  __builtin_ia32_lfence();
  unsigned long long first = __builtin_ia32_rdtsc();
  __builtin_ia32_mfence();
  unsigned long long second = __builtin_ia32_rdtscp(&auxiliary);
  __builtin_ia32_sfence();
  printf("%d %d %llu\n", first != 0, second >= first, crc32_probe());
  return 0;
}
