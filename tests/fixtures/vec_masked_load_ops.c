#include <immintrin.h>
#include <stdio.h>

__attribute__((target("avx512f,avx512vl"))) static void masked_load_probe(
    int *p, unsigned mask, int *out) {
  __m128i w = _mm_set1_epi32(-1);
  __m128i v = _mm_mask_loadu_epi32(w, (__mmask8)mask, p);
  _mm_storeu_si128((__m128i *)out, v);
}

int main(void) {
  int data[4] = {10, 20, 30, 40};
  int out[4];
  masked_load_probe(data, 0x5u, out);
  printf("%d %d %d %d\n", out[0], out[1], out[2], out[3]);
  return 0;
}
