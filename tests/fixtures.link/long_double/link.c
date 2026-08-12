/*
 * long_double link fixture: native helper side.
 *
 * Compiled to an object file (link.o) and linked into BOTH the C reference
 * binary and the translated Rust binary.  Do not translate this file.
 */
#include <float.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_MSC_VER)
#define NOINLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
#define NOINLINE __attribute__((noinline))
#else
#define NOINLINE
#endif

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define ALIGNOF(T) _Alignof(T)
#else
#define ALIGNOF(T)                                                             \
  offsetof(                                                                    \
      struct {                                                                 \
        char c;                                                                \
        T    x;                                                                \
      },                                                                       \
      x)
#endif

struct ld_box {
  unsigned char tag;
  long double   x;
  uint32_t      tail;
};

struct ld_pair {
  long double a;
  long double b;
};

struct ld_nested {
  uint16_t       head;
  struct ld_pair pair;
  unsigned char  bytes[3];
  long double    z;
};

union ld_union {
  long double   ld;
  unsigned char bytes[sizeof(long double)];
  uint64_t      u64;
};

typedef long double (*ld_binary_fn)(long double, long double);
typedef long double (*ld_callback_fn)(long double, long double);
long double         ext_global_ld = 0x1.23456789abcdefp+10L;

NOINLINE long double ext_identity(long double x) { return x; }
NOINLINE long double ext_add(long double a, long double b) { return a + b; }
NOINLINE long double ext_sub(long double a, long double b) { return a - b; }
NOINLINE long double ext_mul(long double a, long double b) { return a * b; }
NOINLINE long double ext_div(long double a, long double b) { return a / b; }
NOINLINE long double ext_neg(long double a) { return -a; }

NOINLINE int ext_eq(long double a, long double b) { return a == b; }
NOINLINE int ext_lt(long double a, long double b) { return a < b; }
NOINLINE int ext_le(long double a, long double b) { return a <= b; }

NOINLINE long double ext_from_i64(int64_t x) { return (long double)x; }
NOINLINE long double ext_from_u64(uint64_t x) { return (long double)x; }
NOINLINE int64_t     ext_to_i64(long double x) { return (int64_t)x; }
NOINLINE uint64_t    ext_to_u64(long double x) { return (uint64_t)x; }
NOINLINE long double ext_from_double(double x) { return (long double)x; }
NOINLINE double      ext_to_double(long double x) { return (double)x; }
NOINLINE long double ext_from_float(float x) { return (long double)x; }
NOINLINE float       ext_to_float(long double x) { return (float)x; }

NOINLINE long double ext_sum10(long double a0, long double a1, long double a2,
                               long double a3, long double a4, long double a5,
                               long double a6, long double a7, long double a8,
                               long double a9) {
  return (((((a0 + a1) + a2) + a3) + a4) + ((((a5 + a6) + a7) + a8) + a9));
}

/* Mix integer, double, float and long-double arguments so each ABI has to
 * classify a nontrivial call correctly.  Volatile temporaries prevent this
 * helper from becoming an accidental constant-expression oracle. */
NOINLINE long double ext_mix_abi(int64_t i0, double d0, long double a,
                                 uint32_t u0, long double b, float f0,
                                 long double c, int i1, double d1,
                                 long double d) {
  volatile long double r = a;
  r                      = r + b;
  r                      = r - c;
  r                      = r + d;
  r                      = r + (long double)i0;
  r                      = r - (long double)u0;
  r                      = r + (long double)d0;
  r                      = r - (long double)f0;
  r                      = r + (long double)i1;
  r                      = r - (long double)d1;
  return r;
}

NOINLINE struct ld_box ext_box_roundtrip(struct ld_box b) {
  b.x     = b.x + 0x1p-9L;
  b.tag   = (unsigned char)(b.tag ^ 0x5aU);
  b.tail ^= UINT32_C(0xa5a55a5a);
  return b;
}

NOINLINE struct ld_pair ext_pair_make(long double a, long double b) {
  struct ld_pair p;
  p.a = a;
  p.b = b;
  return p;
}

NOINLINE struct ld_nested ext_nested_roundtrip(struct ld_nested n) {
  n.pair.a   += 1.0L;
  n.pair.b   -= 2.0L;
  n.z        *= 0.5L;
  n.head     ^= UINT16_C(0x55aa);
  n.bytes[0] ^= 1U;
  n.bytes[1] ^= 2U;
  n.bytes[2] ^= 4U;
  return n;
}

NOINLINE long double ext_array_sum(const long double *p, size_t n) {
  long double r = 0.0L;
  size_t      i;
  for (i = 0; i < n; ++i)
    r += p[i];
  return r;
}

NOINLINE void        ext_store(long double *p, long double x) { *p = x; }
NOINLINE long double ext_load(const long double *p) { return *p; }

NOINLINE long double ext_call_cb(ld_callback_fn cb, long double a,
                                 long double b) {
  return cb(a, b);
}

NOINLINE long double ext_vsum(int n, ...) {
  va_list     ap;
  long double r = 0.0L;
  int         i;
  va_start(ap, n);
  for (i = 0; i < n; ++i)
    r += va_arg(ap, long double);
  va_end(ap);
  return r;
}

NOINLINE void        ext_global_set(long double x) { ext_global_ld = x; }
NOINLINE long double ext_global_get(void) { return ext_global_ld; }

NOINLINE size_t ext_sizeof_ld(void) { return sizeof(long double); }
NOINLINE size_t ext_alignof_ld(void) { return ALIGNOF(long double); }
NOINLINE size_t ext_sizeof_box(void) { return sizeof(struct ld_box); }
NOINLINE size_t ext_alignof_box(void) { return ALIGNOF(struct ld_box); }
NOINLINE size_t ext_offset_box_x(void) { return offsetof(struct ld_box, x); }
NOINLINE size_t ext_offset_box_tail(void) {
  return offsetof(struct ld_box, tail);
}
NOINLINE size_t ext_sizeof_pair(void) { return sizeof(struct ld_pair); }
NOINLINE size_t ext_alignof_pair(void) { return ALIGNOF(struct ld_pair); }
NOINLINE size_t ext_sizeof_nested(void) { return sizeof(struct ld_nested); }
NOINLINE size_t ext_alignof_nested(void) { return ALIGNOF(struct ld_nested); }
NOINLINE size_t ext_offset_nested_pair(void) {
  return offsetof(struct ld_nested, pair);
}
NOINLINE size_t ext_offset_nested_z(void) {
  return offsetof(struct ld_nested, z);
}
NOINLINE size_t ext_sizeof_union(void) { return sizeof(union ld_union); }
NOINLINE size_t ext_alignof_union(void) { return ALIGNOF(union ld_union); }
