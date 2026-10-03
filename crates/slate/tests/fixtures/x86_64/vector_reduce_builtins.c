#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

typedef int32_t v4si __attribute__((vector_size(16)));
typedef uint32_t v4su __attribute__((vector_size(16)));
typedef int64_t v2di __attribute__((vector_size(16)));
typedef uint8_t v16qu __attribute__((vector_size(16)));

static long long fold(const int64_t *lanes, int count) {
  long long h = 17;
  for (int i = 0; i < count; i++) h = h * 1000003 + lanes[i];
  return h;
}

#define PRINT(label, v, n)                                  \
  do {                                                      \
    int64_t lanes[n];                                       \
    for (int i = 0; i < n; i++) lanes[i] = (int64_t)(v)[i]; \
    printf("%s %lld\n", label, fold(lanes, n));             \
  } while (0)

static int calls;

static v4si counted(v4si v) {
  calls++;
  return v;
}

static void generic(void) {
  v4si a = {7, -3, 1000000, -2147483647};
  v4su u = {7, 0xfffffffdu, 1000000, 0x80000001u};
  v2di d = {0x123456789abcdefll, -0x0fedcba987654321ll};
  printf("reduce %d %d %d %d %d %d %d\n", __builtin_reduce_add(a), __builtin_reduce_mul(a),
         __builtin_reduce_and(a), __builtin_reduce_or(a), __builtin_reduce_xor(a),
         __builtin_reduce_max(a), __builtin_reduce_min(a));
  printf("ureduce %u %u %lld\n", __builtin_reduce_max(u), __builtin_reduce_min(u),
         (long long)__builtin_reduce_add(d));
  PRINT("popcount", __builtin_elementwise_popcount(u), 4);
  PRINT("max", __builtin_elementwise_max(a, (v4si){0, 0, 5, 5}), 4);
  PRINT("umax", __builtin_elementwise_max(u, (v4su){8, 8, 8, 8}), 4);
  PRINT("min", __builtin_elementwise_min(a, (v4si){0, 0, 5, 5}), 4);
  v16qu bytes = {0, 1, 2, 250, 251, 252, 253, 254, 255, 9, 10, 11, 12, 13, 14, 15};
  PRINT("bytemax", __builtin_elementwise_max(bytes, (v16qu){128, 128, 128, 128, 128, 128, 128,
                                                             128, 3, 3, 3, 3, 3, 3, 3, 3}),
        16);
  int once = __builtin_reduce_add(counted(a));
  printf("once %d %d\n", once, calls);
}

__attribute__((target("avx2,fma"))) static void avx2(void) {
  __m256i bytes = _mm256_setr_epi8(1, 200, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18,
                                   19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 250);
  __m256i other = _mm256_set1_epi8(16);
  __m256i max = _mm256_max_epu8(bytes, other);
  int64_t out[2];
  _mm_storeu_si128((__m128i *)out, _mm256_extracti128_si256(max, 1));
  printf("hi %lld\n", fold(out, 2));
  _mm_storeu_si128((__m128i *)out, _mm256_extracti128_si256(max, 0));
  printf("lo %lld\n", fold(out, 2));
  __m256 floats = _mm256_setr_ps(1, 2, 3, 4, 5, 6, 7, 8);
  __m256 fused = _mm256_fmadd_ps(floats, floats, _mm256_set1_ps(0.5f));
  float f[4];
  _mm_storeu_ps(f, _mm256_extractf128_ps(fused, 1));
  printf("extractf %g %g %g %g\n", f[0], f[1], f[2], f[3]);
  _mm_storeu_ps(f, _mm256_extractf128_ps(fused, 0));
  printf("extractf %g %g %g %g\n", f[0], f[1], f[2], f[3]);
}

__attribute__((target("avx512f,avx512vpopcntdq"))) static void avx512(void) {
  __m512i words = _mm512_setr_epi64(1, 3, 7, -1, 0x5555, 0, 255, 0x8000000000000000ll);
  printf("popcnt %lld\n", _mm512_reduce_add_epi64(_mm512_popcnt_epi64(words)));
  __m512i ints = _mm512_set1_epi32(0x0f0f0f0f);
  __m512i ones = _mm512_set1_epi32(0x33333333);
  __m512i zero = _mm512_setzero_si512();
  __m512i tern = _mm512_ternarylogic_epi32(ints, ones, zero, 0xEA);
  printf("tern %d\n", _mm512_reduce_add_epi32(tern));
  __m512i seq = _mm512_setr_epi64(10, 20, 30, 40, 50, 60, 70, 80);
  int64_t out[4];
  _mm256_storeu_si256((__m256i *)out, _mm512_extracti64x4_epi64(seq, 1));
  printf("x4 %lld\n", fold(out, 4));
  _mm256_storeu_si256((__m256i *)out, _mm512_extracti64x4_epi64(seq, 0));
  printf("x4 %lld\n", fold(out, 4));
  __m512 floats = _mm512_setr_ps(1e8f, 1, -1e8f, 1, 0.1f, 0.2f, 0.3f, 0.4f, 3, 5e7f, 7, -5e7f,
                                 1e-3f, 2e-3f, 4e-3f, 8e-3f);
  __m512 fused = _mm512_fmadd_ps(floats, _mm512_set1_ps(1.5f), _mm512_set1_ps(0.25f));
  printf("fadd %.9g %.9g\n", _mm512_reduce_add_ps(floats), _mm512_reduce_add_ps(fused));
}

int main(void) {
  __builtin_cpu_init();
  generic();
  if (__builtin_cpu_supports("avx2") && __builtin_cpu_supports("fma"))
    avx2();
  else
    printf("no avx2\n");
  if (__builtin_cpu_supports("avx512f") && __builtin_cpu_supports("avx512vpopcntdq"))
    avx512();
  else
    printf("no avx512\n");
  return 0;
}
