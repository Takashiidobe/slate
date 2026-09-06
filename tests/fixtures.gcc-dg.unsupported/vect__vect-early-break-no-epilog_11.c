/* { dg-add-options vect_early_break } */
/* { dg-do run } */
/* { dg-require-effective-target vect_early_break_hw } */
/* { dg-require-effective-target vect_long } */

/* { dg-additional-options "-mavx512f" { target { i?86-*-* x86_64-*-* } } } */
/* { dg-require-effective-target avx512f_runtime { target i?86-*-* x86_64-*-* } } */

/* Check if system supports SIMD */
#include <signal.h>

#if defined(__i386__) || defined(__x86_64__)
#include "cpuid.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
extern void abort(void);
extern void exit(int);
#ifdef __cplusplus
}
#endif

static void sig_ill_handler(int sig) { exit(0); }

static void __attribute__((noinline)) check_vect(void) {
  signal(SIGILL, sig_ill_handler);
#if defined(__PAIRED__)
  /* 750CL paired-single instruction, 'ps_mul %v0,%v0,%v0'.  */
  asm volatile(".long 0x10000032");
#elif defined(__ppc__) || defined(__ppc64__) || defined(__powerpc__) ||        \
    defined(powerpc)
  /* Altivec instruction, 'vor %v0,%v0,%v0'.  */
  asm volatile(".long 0x10000484");
#elif defined(__i386__) || defined(__x86_64__)
  {
    unsigned int a, b, c, d, want_level, want_b = 0, want_c = 0, want_d = 0;

    /* Determine what instruction set we've been compiled for, and detect
       that we're running with it.  This allows us to at least do a compile
       check for, e.g. SSE4.1 when the machine only supports SSE2.  */
#if defined(__AVX512VL__)
    want_level = 7, want_b = bit_AVX512VL;
#elif defined(__AVX512F__)
    want_level = 7, want_b = bit_AVX512F;
#elif defined(__AVX2__)
    want_level = 7, want_b = bit_AVX2;
#elif defined(__AVX__)
    want_level = 1, want_c = bit_AVX;
#elif defined(__SSE4_2__)
    want_level = 1, want_c = bit_SSE4_2;
#elif defined(__SSE4_1__)
    want_level = 1, want_c = bit_SSE4_1;
#elif defined(__SSSE3__)
    want_level = 1, want_c = bit_SSSE3;
#else
    want_level = 1, want_d = bit_SSE2;
#endif

    if (!__get_cpuid_count(want_level, 0, &a, &b, &c, &d) ||
        ((b & want_b) | (c & want_c) | (d & want_d)) == 0)
      exit(0);
  }
#elif defined(__sparc__)
  asm volatile(".word\t0x81b007c0");
#elif defined(__arm__)
  {
    /* On some processors without NEON support, this instruction may
       be a no-op, on others it may trap, so check that it executes
       correctly.  */
    long long a = 0, b = 1;
    asm("vorr %P0, %P1, %P2" : "=w"(a) : "0"(a), "w"(b));
    if (a != 1)
      exit(0);
  }
#elif defined(__mips_msa)
  asm volatile("or.v $w0,$w0,$w0");
#elif defined(__loongarch__)
  asm volatile("vor.v\t$vr0,$vr0,$vr0");
#endif
  signal(SIGILL, SIG_DFL);
}

#if defined(__ARM_FEATURE_SVE)
#if __ARM_FEATURE_SVE_BITS == 0
#define VECTOR_BITS 1024
#else
#define VECTOR_BITS __ARM_FEATURE_SVE_BITS
#endif
#else
#define VECTOR_BITS 128
#endif

/* Which most of our tests are going to #define for internal use, and
   which might be exposed by system headers related to signal.h on some
   targets, notably VxWorks.  */
#undef OFFSET

__attribute__((noipa)) int f1(const unsigned long *restrict a,
                              const unsigned long *b, int n) {
  for (int i = 0; i < n; ++i) {
    if (a[i] < b[i])
      return 0;
    if (a[i] > b[i])
      return 1;
  }
  return 1;
}

__attribute__((noipa)) int f2(const unsigned long *restrict a,
                              const unsigned long *b, int n) {
  for (int i = 0; i < n; ++i) {
    if (a[i] < b[i])
      return 1;
    if (a[i] > b[i])
      return 1;
  }
  return 0;
}

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  check_vect();

  static unsigned long a[3] __attribute__((aligned(16))) = {10, 1, 0};
  static unsigned long b[3] __attribute__((aligned(16))) = {9, 2, 0};

  if (f1(a, b, 3) != 1)
    __builtin_abort();

  if (f2(a, b, 3) != 1)
    __builtin_abort();

  return 0;
}
// @rewrite-fn-end
// @lowering-fn-end

/* { dg-final { scan-tree-dump-times "LOOP VECTORIZED" 2 "vect" } } */
/* { dg-final { scan-tree-dump-not "early break does not require epilog" "vect" } } */
