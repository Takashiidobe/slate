#include <immintrin.h>
#include <stdio.h>

static int gnu_sse2_intrinsics(void) {
  __m128i left  = _mm_setr_epi32(1, 2, 3, 4);
  __m128i right = _mm_set1_epi32(10);
  __m128i sum   = _mm_add_epi32(left, right);
  int     values[4];
  _mm_storeu_si128((__m128i *)values, sum);
  return values[0] + values[1] + values[2] + values[3];
}

__attribute__((target("ssse3"))) static int gnu_ssse3_intrinsics(void) {
  __m128i values   = _mm_setr_epi32(-1, -2, -3, -4);
  __m128i absolute = _mm_abs_epi32(values);
  int     result[4];
  _mm_storeu_si128((__m128i *)result, absolute);
  return result[0] + result[1] + result[2] + result[3];
}

__attribute__((target("avx2"))) static int gnu_avx2_intrinsics(void) {
  __m256i left    = _mm256_set1_epi32(3);
  __m256i right   = _mm256_set1_epi32(4);
  __m256i product = _mm256_mullo_epi32(left, right);
  int     values[8];
  _mm256_storeu_si256((__m256i *)values, product);
  return values[0] + values[7];
}

__attribute__((target("bmi2"))) static unsigned int gnu_bmi2_intrinsics(void) {
  return _pdep_u32(5u, 0x55u);
}

__attribute__((target("sse4.2"))) static unsigned int
gnu_crc32_intrinsics(void) {
  return _mm_crc32_u32(0u, 0x12345678u);
}

int main(void) {
  int ssse3 = __builtin_cpu_supports("ssse3") ? gnu_ssse3_intrinsics() : -1;
  int avx2  = __builtin_cpu_supports("avx2") ? gnu_avx2_intrinsics() : -1;
  unsigned int bmi2 =
      __builtin_cpu_supports("bmi2") ? gnu_bmi2_intrinsics() : 0;
  unsigned int crc =
      __builtin_cpu_supports("sse4.2") ? gnu_crc32_intrinsics() : 0;
  printf("%d %d %d %u %u\n", gnu_sse2_intrinsics(), ssse3, avx2, bmi2, crc);
  return 0;
}
