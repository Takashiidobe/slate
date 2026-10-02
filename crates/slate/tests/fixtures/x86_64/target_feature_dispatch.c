#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ATTRIBUTE_TARGET_POPCNT __attribute__((target("popcnt")))
#define ATTRIBUTE_TARGET_AVX2 __attribute__((target("avx2,fma")))
#define ATTRIBUTE_TARGET_AVX512 __attribute__((target("avx512f, no-sse4a")))

static long long popcount_scalar(const uint64_t *words, size_t count) {
  long long bits = 0;
  for (size_t i = 0; i < count; i++) {
    uint64_t word = words[i];
    while (word) {
      word &= word - 1;
      bits++;
    }
  }
  return bits;
}

ATTRIBUTE_TARGET_POPCNT
static long long popcount_hw(const uint64_t *words, size_t count) {
  long long bits = 0;
  for (size_t i = 0; i < count; i++) bits += __builtin_popcountll(words[i]);
  return bits;
}

static void add_scalar(int32_t *out, const int32_t *a, const int32_t *b, size_t count) {
  for (size_t i = 0; i < count; i++) out[i] = a[i] + b[i];
}

ATTRIBUTE_TARGET_AVX2
static void add_avx2(int32_t *out, const int32_t *a, const int32_t *b, size_t count) {
  size_t i = 0;
  for (; i + 8 <= count; i += 8) {
    __m256i va = _mm256_loadu_si256((const __m256i *)(a + i));
    __m256i vb = _mm256_loadu_si256((const __m256i *)(b + i));
    _mm256_storeu_si256((__m256i *)(out + i), _mm256_add_epi32(va, vb));
  }
  for (; i < count; i++) out[i] = a[i] + b[i];
}

ATTRIBUTE_TARGET_AVX2
static void reverse_bytes_avx2(uint8_t *bytes) {
  __m256i mask = _mm256_setr_epi8(15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0,
                                  15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0);
  __m256i v = _mm256_loadu_si256((const __m256i *)bytes);
  _mm256_storeu_si256((__m256i *)bytes, _mm256_shuffle_epi8(v, mask));
}

static void reverse_bytes_scalar(uint8_t *bytes) {
  for (int lane = 0; lane < 32; lane += 16)
    for (int i = 0; i < 8; i++) {
      uint8_t t = bytes[lane + i];
      bytes[lane + i] = bytes[lane + 15 - i];
      bytes[lane + 15 - i] = t;
    }
}

ATTRIBUTE_TARGET_AVX512
static void add_avx512(int32_t *out, const int32_t *a, const int32_t *b, size_t count) {
  size_t i = 0;
  for (; i + 16 <= count; i += 16) {
    __m512i va = _mm512_loadu_si512((const void *)(a + i));
    __m512i vb = _mm512_loadu_si512((const void *)(b + i));
    _mm512_storeu_si512((void *)(out + i), _mm512_add_epi32(va, vb));
  }
  for (; i < count; i++) out[i] = a[i] + b[i];
}

typedef void add_fn(int32_t *, const int32_t *, const int32_t *, size_t);

static add_fn *select_add(void) {
  if (__builtin_cpu_supports("avx512f")) return add_avx512;
  if (__builtin_cpu_supports("avx2") && __builtin_cpu_supports("fma")) return add_avx2;
  return add_scalar;
}

int main(void) {
  __builtin_cpu_init();

  uint64_t words[37];
  for (size_t i = 0; i < 37; i++) words[i] = 0x9e3779b97f4a7c15ull * (i + 1) ^ (i << 40);
  long long bits = __builtin_cpu_supports("popcnt") ? popcount_hw(words, 37)
                                                    : popcount_scalar(words, 37);
  printf("bits %lld %lld\n", bits, popcount_scalar(words, 37));

  int32_t a[45], b[45], out[45];
  for (int i = 0; i < 45; i++) {
    a[i] = i * 7 - 100;
    b[i] = 1000 - i * i;
  }
  add_fn *add = select_add();
  memset(out, 0, sizeof(out));
  add(out, a, b, 45);
  long long sum = 0;
  for (int i = 0; i < 45; i++) sum = sum * 31 + out[i];
  printf("add %lld %d %d\n", sum, out[0], out[44]);

  uint8_t bytes[32];
  for (int i = 0; i < 32; i++) bytes[i] = (uint8_t)(i * 3);
  if (__builtin_cpu_supports("avx2"))
    reverse_bytes_avx2(bytes);
  else
    reverse_bytes_scalar(bytes);
  for (int i = 0; i < 32; i++) printf("%d%c", bytes[i], i == 31 ? '\n' : ' ');
  return 0;
}
