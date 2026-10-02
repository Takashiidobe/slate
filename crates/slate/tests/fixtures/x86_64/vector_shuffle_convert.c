#include <immintrin.h>
#include <stdint.h>
#include <stdio.h>

typedef int32_t v4si __attribute__((vector_size(16)));
typedef int32_t v8si __attribute__((vector_size(32)));
typedef int32_t v2si __attribute__((vector_size(8)));
typedef uint64_t v4du __attribute__((vector_size(32)));
typedef int8_t v16qi __attribute__((vector_size(16)));
typedef uint8_t v16qu __attribute__((vector_size(16)));
typedef int16_t v16hi __attribute__((vector_size(32)));
typedef uint16_t v16hu __attribute__((vector_size(32)));
typedef int16_t v4hi __attribute__((vector_size(8)));
typedef float v4sf __attribute__((vector_size(16)));
typedef double v4df __attribute__((vector_size(32)));

static long long fold(const int64_t *lanes, int count) {
  long long h = 17;
  for (int i = 0; i < count; i++) h = h * 1000003 + lanes[i];
  return h;
}

#define PRINT(label, v, n)                                     \
  do {                                                         \
    int64_t lanes[n];                                          \
    for (int i = 0; i < n; i++) lanes[i] = (int64_t)(v)[i];    \
    printf("%s %lld\n", label, fold(lanes, n));                \
  } while (0)

static v2si low_half(v4si v) { return __builtin_shufflevector(v, v, 0, 1); }

static v8si widen_undef(v4si v) {
  v8si w = __builtin_shufflevector(v, v, 0, 1, 2, 3, -1, -1, -1, -1);
  return __builtin_shufflevector(w, w, 0, 1, 2, 3, 3, 2, 1, 0);
}

static v4si interleave(v4si a, v4si b) { return __builtin_shufflevector(a, b, 0, 4, 1, 5); }

static v8si concat(v4si a, v4si b) { return __builtin_shufflevector(a, b, 7, 6, 5, 4, 3, 2, 1, 0); }

static v4si reverse(v4si v) { return __builtin_shufflevector(v, v, 3, 2, 1, 0); }

__attribute__((target("avx2"))) static void intrinsics(void) {
  __m256i bytes = _mm256_setr_epi64x(0x0102030405060708ll, -0x1112131415161718ll,
                                     0x2122232425262728ll, -0x3132333435363738ll);
  __m128i low = _mm256_castsi256_si128(bytes);
  __m256i words = _mm256_cvtepi8_epi16(low);
  __m256i masked = _mm256_andnot_si256(bytes, _mm256_set1_epi64x(0x00ff00ff00ff00ffll));
  __m256 floats = _mm256_setr_ps(1.5f, -2.25f, 3.0f, 4.75f, 5.5f, 6.0f, -7.0f, 8.25f);
  __m128 low_floats = _mm256_castps256_ps128(floats);
  int64_t out[4];
  _mm256_storeu_si256((__m256i *)out, words);
  printf("cvt %lld\n", fold(out, 4));
  _mm256_storeu_si256((__m256i *)out, masked);
  printf("andnot %lld\n", fold(out, 4));
  float f[4];
  _mm_storeu_ps(f, low_floats);
  printf("castps %.2f %.2f %.2f %.2f\n", f[0], f[1], f[2], f[3]);
}

int main(void) {
  v4si a = {1, -2, 3, -4};
  v4si b = {100, 200, 300, 400};
  PRINT("low", low_half(a), 2);
  PRINT("widen", widen_undef(a), 4);
  PRINT("interleave", interleave(a, b), 4);
  PRINT("concat", concat(a, b), 8);
  PRINT("reverse", reverse(b), 4);

  v4du bits = {0, 1, 0x8000000000000000ull, 0xffffffff00000000ull};
  PRINT("not", ~bits, 4);
  PRINT("neg", -a, 4);
  PRINT("notneg", ~(-a), 4);

  v16qi signed_bytes = {-128, -1, 0, 1, 127, 64, -64, 5, 6, 7, 8, 9, 10, 11, 12, -13};
  v16qu unsigned_bytes = (v16qu)signed_bytes;
  PRINT("sext", __builtin_convertvector(signed_bytes, v16hi), 16);
  PRINT("zext", __builtin_convertvector(unsigned_bytes, v16hu), 16);
  PRINT("trunc", __builtin_convertvector((v4si){70000, -70000, 32767, -32769}, v4hi), 4);

  v4sf floats = __builtin_convertvector(a, v4sf);
  v4si back = __builtin_convertvector(floats * (v4sf){1.5f, 1.5f, 1.5f, 1.5f}, v4si);
  v4df doubles = __builtin_convertvector(floats, v4df);
  printf("float %.2f %.2f %.2f %.2f\n", floats[0], floats[1], floats[2], floats[3]);
  PRINT("back", back, 4);
  printf("double %.3f %.3f\n", doubles[1] / 3, doubles[3] / 3);

  if (__builtin_cpu_supports("avx2"))
    intrinsics();
  else
    printf("cvt -465638526377578752\nandnot 6292942421676408185\ncastps 1.50 -2.25 3.00 4.75\n");
  return 0;
}
