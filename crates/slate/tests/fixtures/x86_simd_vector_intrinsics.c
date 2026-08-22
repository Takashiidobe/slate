#include <immintrin.h>
#include <stdio.h>
#include <tmmintrin.h>

static void sse2_probe(int out[4]) {
  __m128i a = _mm_set1_epi32(5);
  __m128i b = _mm_set1_epi32(3);
  __m128i c = _mm_add_epi32(a, b);
  _mm_storeu_si128((__m128i *)out, c);
}

__attribute__((target("ssse3"))) static void ssse3_probe(signed char out[16]) {
  __m128i a     = _mm_set1_epi8(-1);
  __m128i idx   = _mm_setzero_si128();
  __m128i abs_a = _mm_abs_epi8(a);
  __m128i shuf  = _mm_shuffle_epi8(abs_a, idx);
  _mm_storeu_si128((__m128i *)out, shuf);
}

__attribute__((target("avx2"))) static void avx2_probe(int out[8]) {
  __m256i a = _mm256_set1_epi32(7);
  __m256i b = _mm256_set1_epi32(2);
  __m256i c = _mm256_add_epi32(a, b);
  _mm256_storeu_si256((__m256i *)out, c);
}

int main(void) {
  int sse2_out[4];
  signed char ssse3_out[16];
  int avx2_out[8];

  sse2_probe(sse2_out);
  ssse3_probe(ssse3_out);
  avx2_probe(avx2_out);

  printf("%d %d %d %d\n", sse2_out[0], sse2_out[1], sse2_out[2], sse2_out[3]);
  printf("%d %d\n", ssse3_out[0], ssse3_out[15]);
  printf("%d %d %d %d %d %d %d %d\n", avx2_out[0], avx2_out[1], avx2_out[2],
         avx2_out[3], avx2_out[4], avx2_out[5], avx2_out[6], avx2_out[7]);
  return 0;
}
