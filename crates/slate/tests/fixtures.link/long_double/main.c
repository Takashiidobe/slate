/*
 * long_double link fixture: translated/test side.
 *
 * Translated to Rust and also compiled natively (linking link.o) as the C
 * reference.  The C binary and the translated Rust binary must print identical
 * output and exit with the same status.
 */
#include <errno.h>
#include <float.h>
#include <math.h>
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
extern long double  ext_global_ld;

extern long double ext_identity(long double);
extern long double ext_add(long double, long double);
extern long double ext_sub(long double, long double);
extern long double ext_mul(long double, long double);
extern long double ext_div(long double, long double);
extern long double ext_neg(long double);
extern int         ext_eq(long double, long double);
extern int         ext_lt(long double, long double);
extern int         ext_le(long double, long double);
extern long double ext_from_i64(int64_t);
extern long double ext_from_u64(uint64_t);
extern int64_t     ext_to_i64(long double);
extern uint64_t    ext_to_u64(long double);
extern long double ext_from_double(double);
extern double      ext_to_double(long double);
extern long double ext_from_float(float);
extern float       ext_to_float(long double);
extern long double ext_sum10(long double, long double, long double, long double,
                             long double, long double, long double, long double,
                             long double, long double);
extern long double ext_mix_abi(int64_t, double, long double, uint32_t,
                               long double, float, long double, int, double,
                               long double);
extern struct ld_box    ext_box_roundtrip(struct ld_box);
extern struct ld_pair   ext_pair_make(long double, long double);
extern struct ld_nested ext_nested_roundtrip(struct ld_nested);
extern long double      ext_array_sum(const long double *, size_t);
extern void             ext_store(long double *, long double);
extern long double      ext_load(const long double *);
extern long double      ext_call_cb(ld_callback_fn, long double, long double);
extern long double      ext_vsum(int, ...);
extern void             ext_global_set(long double);
extern long double      ext_global_get(void);
extern size_t           ext_sizeof_ld(void);
extern size_t           ext_alignof_ld(void);
extern size_t           ext_sizeof_box(void);
extern size_t           ext_alignof_box(void);
extern size_t           ext_offset_box_x(void);
extern size_t           ext_offset_box_tail(void);
extern size_t           ext_sizeof_pair(void);
extern size_t           ext_alignof_pair(void);
extern size_t           ext_sizeof_nested(void);
extern size_t           ext_alignof_nested(void);
extern size_t           ext_offset_nested_pair(void);
extern size_t           ext_offset_nested_z(void);
extern size_t           ext_sizeof_union(void);
extern size_t           ext_alignof_union(void);

static int failures;
static int checks;

#define CHECK(expr)                                                            \
  do {                                                                         \
    ++checks;                                                                  \
    if (!(expr)) {                                                             \
      ++failures;                                                              \
      fprintf(stderr, "FAIL line %d: %s\n", __LINE__, #expr);                  \
    }                                                                          \
  } while (0)

#define CHECK_LD_EQ(got_expr, expected_expr)                                   \
  do {                                                                         \
    long double check_got_      = (got_expr);                                  \
    long double check_expected_ = (expected_expr);                             \
    ++checks;                                                                  \
    if (!(check_got_ == check_expected_)) {                                    \
      ++failures;                                                              \
      fprintf(stderr, "FAIL line %d: %s != %s (got=%La expected=%La)\n",       \
              __LINE__, #got_expr, #expected_expr, check_got_,                 \
              check_expected_);                                                \
    }                                                                          \
  } while (0)

#define CHECK_LD_SAME_SIGNED_ZERO(got_expr, expected_expr)                     \
  do {                                                                         \
    long double check_got_      = (got_expr);                                  \
    long double check_expected_ = (expected_expr);                             \
    ++checks;                                                                  \
    if (!(check_got_ == check_expected_ &&                                     \
          !!signbit(check_got_) == !!signbit(check_expected_))) {              \
      ++failures;                                                              \
      fprintf(stderr, "FAIL line %d: signed-zero mismatch\n", __LINE__);       \
    }                                                                          \
  } while (0)

static const char *ld_kind(void) {
#if LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024
  return "binary64-like";
#elif LDBL_MANT_DIG == 64 && LDBL_MAX_EXP == 16384
  return "x87-extended";
#elif LDBL_MANT_DIG == 113 && LDBL_MAX_EXP == 16384
  return "binary128";
#else
  return "unknown";
#endif
}

static NOINLINE long double local_identity(long double x) {
  volatile long double y = x;
  return y;
}

static NOINLINE long double local_cb(long double a, long double b) {
  volatile long double x = a;
  volatile long double y = b;
  return (x * 3.0L) - (y / 2.0L) + 0x1p-17L;
}

static NOINLINE long double local_vsum(int n, ...) {
  va_list     ap;
  long double r = 0.0L;
  int         i;
  va_start(ap, n);
  for (i = 0; i < n; ++i)
    r += va_arg(ap, long double);
  va_end(ap);
  return r;
}

static void test_format_and_layout(void) {
  printf("long double: kind=%s sizeof=%zu align=%zu mant=%d max_exp=%d\n",
         ld_kind(), sizeof(long double), (size_t)ALIGNOF(long double),
         LDBL_MANT_DIG, LDBL_MAX_EXP);

  CHECK((LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024) ||
        (LDBL_MANT_DIG == 64 && LDBL_MAX_EXP == 16384) ||
        (LDBL_MANT_DIG == 113 && LDBL_MAX_EXP == 16384));

  CHECK(sizeof(long double) == ext_sizeof_ld());
  CHECK((size_t)ALIGNOF(long double) == ext_alignof_ld());

  CHECK(sizeof(struct ld_box) == ext_sizeof_box());
  CHECK((size_t)ALIGNOF(struct ld_box) == ext_alignof_box());
  CHECK(offsetof(struct ld_box, x) == ext_offset_box_x());
  CHECK(offsetof(struct ld_box, tail) == ext_offset_box_tail());

  CHECK(sizeof(struct ld_pair) == ext_sizeof_pair());
  CHECK((size_t)ALIGNOF(struct ld_pair) == ext_alignof_pair());

  CHECK(sizeof(struct ld_nested) == ext_sizeof_nested());
  CHECK((size_t)ALIGNOF(struct ld_nested) == ext_alignof_nested());
  CHECK(offsetof(struct ld_nested, pair) == ext_offset_nested_pair());
  CHECK(offsetof(struct ld_nested, z) == ext_offset_nested_z());

  CHECK(sizeof(union ld_union) == ext_sizeof_union());
  CHECK((size_t)ALIGNOF(union ld_union) == ext_alignof_union());
}

static void test_literals_and_precision_boundaries(void) {
  long double one = 1.0L;
  long double eps = LDBL_EPSILON;
  long double n1;
  long double n2;
  long double via_parse_64;
  long double via_parse_113;
  char       *end;
  uint64_t    u53p1 = UINT64_C(9007199254740993); /* 2^53 + 1 */

  CHECK(one + eps != one);
  CHECK_LD_EQ((one + eps) - one, eps);
  CHECK_LD_EQ(nextafterl(one, 2.0L) - one, eps);

  n1 = (long double)u53p1;
  n2 = (long double)UINT64_C(9007199254740992);
#if LDBL_MANT_DIG == 53
  CHECK(n1 == n2);
#else
  CHECK(n1 != n2);
#endif

  end          = NULL;
  via_parse_64 = strtold("0x1.0000000000000002p0", &end);
  CHECK(end != NULL && *end == '\0');
#if LDBL_MANT_DIG >= 64
  CHECK(via_parse_64 != 1.0L);
  CHECK_LD_EQ(via_parse_64, 0x1.0000000000000002p0L);
#else
  CHECK(via_parse_64 == 1.0L);
#endif

  end           = NULL;
  via_parse_113 = strtold("0x1.0000000000000000000000000001p0", &end);
  CHECK(end != NULL && *end == '\0');
#if LDBL_MANT_DIG >= 113
  CHECK(via_parse_113 != 1.0L);
  CHECK_LD_EQ(via_parse_113, 0x1.0000000000000000000000000001p0L);
#else
  CHECK(via_parse_113 == 1.0L);
#endif

  /* A literal with no L suffix is rounded as double before conversion. */
#if LDBL_MANT_DIG > DBL_MANT_DIG
  {
    long double wide           = 0x1.0000000000000002p0L;
    long double through_double = (long double)0x1.0000000000000002p0;
    CHECK(wide != through_double);
  }
#endif

#if LDBL_MAX_EXP > 1024
  {
    volatile long double huge_but_finite = 0x1p+2000L;
    CHECK(isfinite(huge_but_finite));
    CHECK(huge_but_finite > (long double)DBL_MAX);
  }
#endif
}

static void test_arithmetic_and_temporaries(void) {
  volatile long double a = 0x1.23456789abcdefp+20L;
  volatile long double b = -0x1.00fedcba987654p-7L;
  volatile long double c = 0x1.fffffffffffffffep+3L;
  long double          x;
  long double          y;

  CHECK_LD_EQ(a + b, ext_add(a, b));
  CHECK_LD_EQ(a - b, ext_sub(a, b));
  CHECK_LD_EQ(a * b, ext_mul(a, b));
  CHECK_LD_EQ(a / c, ext_div(a, c));
  CHECK_LD_EQ(-a, ext_neg(a));

  x = (((a + b) * c) / 0x1.8p+2L) - (((c - b) / 0x1.4p-3L) * 0x1.01p-11L);

  y = ext_sub(ext_div(ext_mul(ext_add(a, b), c), 0x1.8p+2L),
              ext_mul(ext_div(ext_sub(c, b), 0x1.4p-3L), 0x1.01p-11L));
  CHECK_LD_EQ(x, y);

  {
    volatile long double q  = 1.0L;
    q                      += 0.5L;
    q                      *= 3.0L;
    q                      -= 0.25L;
    q                      /= 2.0L;
    CHECK_LD_EQ(q, 2.125L);
    ++q;
    CHECK_LD_EQ(q, 3.125L);
    q--;
    CHECK_LD_EQ(q, 2.125L);
  }

  /* Conditional operator and comma operator force long-double temporaries. */
  CHECK_LD_EQ((1 ? a : (double)b), a);
  CHECK_LD_EQ((x = b, x + 1.0L), b + 1.0L);
}

static void test_casts_and_usual_conversions(void) {
  int64_t  si[] = {INT64_C(0),
                   INT64_C(1),
                   -INT64_C(1),
                   INT64_C(2147483647),
                   -INT64_C(2147483647),
                   INT64_C(9007199254740991),
                   -INT64_C(9007199254740991)};
  uint64_t ui[] = {UINT64_C(0),
                   UINT64_C(1),
                   UINT64_C(4294967295),
                   UINT64_C(9007199254740991),
                   UINT64_C(9007199254740993),
                   UINT64_MAX};
  size_t   i;

  for (i = 0; i < sizeof(si) / sizeof(si[0]); ++i)
    CHECK_LD_EQ((long double)si[i], ext_from_i64(si[i]));

  for (i = 0; i < sizeof(ui) / sizeof(ui[0]); ++i)
    CHECK_LD_EQ((long double)ui[i], ext_from_u64(ui[i]));

  CHECK((int64_t)1234567.875L == ext_to_i64(1234567.875L));
  CHECK((int64_t)-1234567.875L == ext_to_i64(-1234567.875L));
  CHECK((uint64_t)1234567.875L == ext_to_u64(1234567.875L));
  CHECK((int64_t)-0.875L == 0);

  {
    volatile double d   = 0x1.0000000000001p+20;
    volatile float  f   = 0x1.000002p+10f;
    long double     ld1 = (long double)d;
    long double     ld2 = (long double)f;
    CHECK_LD_EQ(ld1, ext_from_double(d));
    CHECK_LD_EQ(ld2, ext_from_float(f));
    CHECK((double)ld1 == ext_to_double(ld1));
    CHECK((float)ld2 == ext_to_float(ld2));
  }

  /* Usual arithmetic conversions: every expression must become long double. */
  {
    volatile int         i32 = -17;
    volatile uint64_t    u64 = UINT64_C(9007199254740993);
    volatile float       f   = 0.25f;
    volatile double      d   = -0.125;
    volatile long double ld  = 3.0L;

    CHECK_LD_EQ(ld + i32, ext_add(ld, (long double)i32));
    CHECK_LD_EQ(ld + u64, ext_add(ld, (long double)u64));
    CHECK_LD_EQ(ld + f, ext_add(ld, (long double)f));
    CHECK_LD_EQ(ld + d, ext_add(ld, (long double)d));
  }
}

static void test_special_values_and_comparisons(void) {
  long double pz   = 0.0L;
  long double nz   = -0.0L;
  long double inf  = INFINITY;
  long double ninf = -INFINITY;
  long double nan  = nanl("");

  CHECK(pz == nz);
  CHECK(!signbit(pz));
  CHECK(signbit(nz));
  CHECK_LD_SAME_SIGNED_ZERO(ext_identity(pz), pz);
  CHECK_LD_SAME_SIGNED_ZERO(ext_identity(nz), nz);
  CHECK_LD_SAME_SIGNED_ZERO(ext_neg(pz), nz);
  CHECK_LD_SAME_SIGNED_ZERO(ext_neg(nz), pz);

  CHECK(isinf(inf) && inf > 0.0L);
  CHECK(isinf(ninf) && ninf < 0.0L);
  CHECK(isnan(nan));
  CHECK(!(nan == nan));
  CHECK(!(nan < 0.0L));
  CHECK(!(nan > 0.0L));
  CHECK(!(nan <= 0.0L));
  CHECK(!(nan >= 0.0L));
  CHECK(nan != nan);

  CHECK(ext_eq(pz, nz));
  CHECK(ext_lt(-1.0L, 1.0L));
  CHECK(ext_le(1.0L, 1.0L));
  CHECK(!ext_eq(nan, nan));
  CHECK(!ext_lt(nan, 0.0L));

  CHECK(signbit(copysignl(1.0L, nz)));
  CHECK(!signbit(copysignl(1.0L, pz)));
}

static void test_libc_and_libm_externs(void) {
  const char *s   = "  -0x1.23456789abcdef0123456789p+17tail";
  char       *end = NULL;
  long double x;
  long double ip;
  long double fp;
  long double fr;
  int         e = 0;
  char        buf[256];
  char       *end2 = NULL;
  int         n;

  errno = 0;
  x     = strtold(s, &end);
  CHECK(end != NULL);
  CHECK(strcmp(end, "tail") == 0);
  CHECK(errno == 0);
  CHECK(x < 0.0L);

  /* External varargs call: passing long double to snprintf as %La. */
  n = snprintf(buf, sizeof(buf), "%La", x);
  CHECK(n > 0 && (size_t)n < sizeof(buf));
  CHECK_LD_EQ(strtold(buf, &end2), x);
  CHECK(end2 != NULL && *end2 == '\0');

  fp = modfl(-12345.75L, &ip);
  CHECK_LD_EQ(ip, -12345.0L);
  CHECK_LD_EQ(fp, -0.75L);

  fr = frexpl(0x1.8p+10L, &e);
  CHECK_LD_EQ(fr, 0.75L);
  CHECK(e == 11);
  CHECK_LD_EQ(ldexpl(fr, e), 0x1.8p+10L);

  CHECK_LD_EQ(sqrtl(4.0L), 2.0L);
  CHECK_LD_EQ(powl(1.5L, 3.0L), 3.375L);
  CHECK_LD_EQ(fmal(2.0L, 3.0L, 4.0L), 10.0L);
  CHECK_LD_EQ(fmodl(17.5L, 3.0L), 2.5L);
  CHECK_LD_EQ(remainderl(5.5L, 2.0L), -0.5L);
  CHECK_LD_EQ(floorl(-1.25L), -2.0L);
  CHECK_LD_EQ(ceill(-1.25L), -1.0L);
  CHECK_LD_EQ(truncl(-1.75L), -1.0L);

  {
    long double toward = nextafterl(1.0L, 2.0L);
    CHECK(toward > 1.0L);
    CHECK_LD_EQ(toward - 1.0L, LDBL_EPSILON);
  }

  {
    long double tiny = nextafterl(0.0L, 1.0L);
    CHECK(tiny > 0.0L);
    CHECK(tiny < LDBL_MIN);
    CHECK(fpclassify(tiny) == FP_SUBNORMAL);
  }
}

static void test_extern_function_abi(void) {
  long double a0 = 0x1p+0L;
  long double a1 = 0x1p-1L;
  long double a2 = 0x1p-2L;
  long double a3 = 0x1p-3L;
  long double a4 = 0x1p-4L;
  long double a5 = 0x1p-5L;
  long double a6 = 0x1p-6L;
  long double a7 = 0x1p-7L;
  long double a8 = 0x1p-8L;
  long double a9 = 0x1p-9L;
  long double expected =
      (((((a0 + a1) + a2) + a3) + a4) + ((((a5 + a6) + a7) + a8) + a9));

  CHECK_LD_EQ(ext_identity(0x1.123456789abcdef012345678p+40L),
              0x1.123456789abcdef012345678p+40L);
  CHECK_LD_EQ(ext_sum10(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9), expected);

  {
    int64_t              i0    = -INT64_C(123456789);
    double               d0    = 0x1.8p+7;
    long double          a     = 0x1.123456789abcdefp+5L;
    uint32_t             u0    = UINT32_C(12345);
    long double          b     = -0x1.987654321p-3L;
    float                f0    = 0.75f;
    long double          c     = 0x1.01p+9L;
    int                  i1    = -77;
    double               d1    = -0.125;
    long double          d     = 0x1.ffffp-11L;
    volatile long double local = a;

    local = local + b;
    local = local - c;
    local = local + d;
    local = local + (long double)i0;
    local = local - (long double)u0;
    local = local + (long double)d0;
    local = local - (long double)f0;
    local = local + (long double)i1;
    local = local - (long double)d1;

    CHECK_LD_EQ(ext_mix_abi(i0, d0, a, u0, b, f0, c, i1, d1, d), local);
  }

  /* Indirect call through a pointer to a native C function.  A translator
   * cannot solve only direct extern calls and ignore this case. */
  {
    volatile ld_binary_fn fp = ext_add;
    CHECK_LD_EQ(fp(0x1.23p+4L, -0x1.2p-7L), 0x1.23p+4L + -0x1.2p-7L);
  }

  /* Native C -> translated callback, with long double args and return. */
  {
    long double a = 0x1.23456789p+8L;
    long double b = -0x1.abcdep-4L;
    CHECK_LD_EQ(ext_call_cb(local_cb, a, b), local_cb(a, b));
  }
}

static void test_struct_union_array_and_pointer_abi(void) {
  struct ld_box    b;
  struct ld_box    r;
  struct ld_pair   p;
  struct ld_nested n;
  struct ld_nested nr;
  long double      arr[7];
  long double      copy;
  unsigned char    raw[sizeof(long double)];
  size_t           i;

  b.tag  = 0x31U;
  b.x    = 0x1.23456789abcdefp+12L;
  b.tail = UINT32_C(0x12345678);
  r      = ext_box_roundtrip(b);
  CHECK(r.tag == (unsigned char)(b.tag ^ 0x5aU));
  CHECK_LD_EQ(r.x, b.x + 0x1p-9L);
  CHECK(r.tail == (b.tail ^ UINT32_C(0xa5a55a5a)));

  p = ext_pair_make(-0x1.111111111111111p+3L, 0x1.222222222222222p-3L);
  CHECK_LD_EQ(p.a, -0x1.111111111111111p+3L);
  CHECK_LD_EQ(p.b, 0x1.222222222222222p-3L);

  memset(&n, 0, sizeof(n));
  n.head     = UINT16_C(0x1234);
  n.pair.a   = 9.0L;
  n.pair.b   = -11.0L;
  n.bytes[0] = 0x10U;
  n.bytes[1] = 0x20U;
  n.bytes[2] = 0x40U;
  n.z        = 7.5L;
  nr         = ext_nested_roundtrip(n);
  CHECK(nr.head == (uint16_t)(n.head ^ UINT16_C(0x55aa)));
  CHECK_LD_EQ(nr.pair.a, 10.0L);
  CHECK_LD_EQ(nr.pair.b, -13.0L);
  CHECK(nr.bytes[0] == (unsigned char)(n.bytes[0] ^ 1U));
  CHECK(nr.bytes[1] == (unsigned char)(n.bytes[1] ^ 2U));
  CHECK(nr.bytes[2] == (unsigned char)(n.bytes[2] ^ 4U));
  CHECK_LD_EQ(nr.z, 3.75L);

  for (i = 0; i < sizeof(arr) / sizeof(arr[0]); ++i)
    arr[i] = (long double)(i + 1) / 8.0L;
  CHECK_LD_EQ(ext_array_sum(arr, sizeof(arr) / sizeof(arr[0])), 3.5L);

  copy = 0.0L;
  ext_store(&copy, -0x1.abcdef012345678p+33L);
  CHECK_LD_EQ(copy, -0x1.abcdef012345678p+33L);
  CHECK_LD_EQ(ext_load(&copy), copy);

  /* Complete-object memcpy round trip.  Padding bytes may be unspecified,
   * so compare the reconstructed numeric value, not raw bytes. */
  copy = 0x1.13579bdf2468acep-20L;
  memcpy(raw, &copy, sizeof(copy));
  copy = 0.0L;
  memcpy(&copy, raw, sizeof(copy));
  CHECK_LD_EQ(copy, 0x1.13579bdf2468acep-20L);

  {
    union ld_union u;
    union ld_union v;
    u.ld = -0x1.2468ace13579bdfp+19L;
    memcpy(&v, &u, sizeof(u));
    CHECK_LD_EQ(v.ld, u.ld);
  }
}

static void test_varargs(void) {
  long double a        = 0x1p+0L;
  long double b        = 0x1p-2L;
  long double c        = -0x1p-4L;
  long double d        = 0x1p-8L;
  long double expected = a + b + c + d;
  char        buf[128];
  char       *end;
  int         n;

  /* Translated va_arg(long double). */
  CHECK_LD_EQ(local_vsum(4, a, b, c, d), expected);

  /* Translated caller -> native C variadic callee. */
  CHECK_LD_EQ(ext_vsum(4, a, b, c, d), expected);

  /* Native libc variadic callee. */
  n = snprintf(buf, sizeof(buf), "%.21La", expected);
  CHECK(n > 0 && (size_t)n < sizeof(buf));
  end = NULL;
  CHECK_LD_EQ(strtold(buf, &end), expected);
  CHECK(end != NULL && *end == '\0');
}

static void test_extern_global(void) {
  long double old = ext_global_ld;
  long double a   = -0x1.abcdef0123456789p+27L;
  long double b   = 0x1.0102030405060708p-31L;

  ext_global_ld = a;
  CHECK_LD_EQ(ext_global_get(), a);

  ext_global_set(b);
  CHECK_LD_EQ(ext_global_ld, b);

  ext_global_set(old);
  CHECK_LD_EQ(ext_global_ld, old);
}

static void test_control_flow_and_spills(void) {
  volatile long double seed = 0x1.0123456789abcdefp+4L;
  long double          x    = seed;
  int                  i;

  /* Force repeated long-double values across branches, loops, calls and
   * volatile stores. */
  for (i = 0; i < 200; ++i) {
    long double t = (long double)((i % 17) - 8) * 0x1p-12L;
    if ((i & 3) == 0)
      x = x + t;
    else if ((i & 3) == 1)
      x = x - t;
    else if ((i & 3) == 2)
      x = x * (1.0L + 0x1p-20L);
    else
      x = x / (1.0L + 0x1p-20L);

    if ((i % 19) == 0)
      x = local_identity(x);
    if ((i % 31) == 0)
      x = ext_identity(x);
  }

  /* Recompute with each primitive operation delegated to the native helper.
   * This catches a surprising number of temporary/spill/conversion bugs. */
  {
    long double y = seed;
    for (i = 0; i < 200; ++i) {
      long double t = ext_mul((long double)((i % 17) - 8), 0x1p-12L);
      if ((i & 3) == 0)
        y = ext_add(y, t);
      else if ((i & 3) == 1)
        y = ext_sub(y, t);
      else if ((i & 3) == 2)
        y = ext_mul(y, ext_add(1.0L, 0x1p-20L));
      else
        y = ext_div(y, ext_add(1.0L, 0x1p-20L));

      if ((i % 19) == 0)
        y = ext_identity(y);
      if ((i % 31) == 0)
        y = ext_identity(y);
    }
    CHECK_LD_EQ(x, y);
  }
}

int main(void) {
  test_format_and_layout();
  test_literals_and_precision_boundaries();
  test_arithmetic_and_temporaries();
  test_casts_and_usual_conversions();
  test_special_values_and_comparisons();
  test_libc_and_libm_externs();
  test_extern_function_abi();
  test_struct_union_array_and_pointer_abi();
  test_varargs();
  test_extern_global();
  test_control_flow_and_spills();

  if (failures != 0) {
    fprintf(stderr, "long-double torture: %d/%d checks FAILED\n", failures,
            checks);
    return 1;
  }

  printf("long-double torture: PASS (%d checks)\n", checks);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU: #![feature(c_variadic)]
// LOWERING-X86_64-GNU-NEXT: #![allow(
// LOWERING-X86_64-GNU-NEXT:     dead_code,
// LOWERING-X86_64-GNU-NEXT:     unused,
// LOWERING-X86_64-GNU-NEXT:     non_camel_case_types,
// LOWERING-X86_64-GNU-NEXT:     non_snake_case,
// LOWERING-X86_64-GNU-NEXT:     non_upper_case_globals,
// LOWERING-X86_64-GNU-NEXT:     arithmetic_overflow,
// LOWERING-X86_64-GNU-NEXT:     unconditional_panic,
// LOWERING-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-X86_64-GNU-NEXT:     unused_comparisons
// LOWERING-X86_64-GNU-NEXT: )]
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C, align(16))]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     type Output = LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// LOWERING-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// LOWERING-X86_64-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// LOWERING-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// LOWERING-X86_64-GNU-NEXT:         } else {
// LOWERING-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// LOWERING-X86_64-GNU-NEXT:             } else {
// LOWERING-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// LOWERING-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// LOWERING-X86_64-GNU-NEXT:                 } else {
// LOWERING-X86_64-GNU-NEXT:                     None
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C)]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct ld_box {
// LOWERING-X86_64-GNU-NEXT:     tag: u8,
// LOWERING-X86_64-GNU-NEXT:     x: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     tail: u32,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C)]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct ld_nested {
// LOWERING-X86_64-GNU-NEXT:     head: u16,
// LOWERING-X86_64-GNU-NEXT:     pair: ld_pair,
// LOWERING-X86_64-GNU-NEXT:     bytes: [u8; 3],
// LOWERING-X86_64-GNU-NEXT:     z: LongDouble,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C)]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: struct ld_pair {
// LOWERING-X86_64-GNU-NEXT:     a: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     b: LongDouble,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[repr(C)]
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// LOWERING-X86_64-GNU-NEXT: union ld_union {
// LOWERING-X86_64-GNU-NEXT:     ld: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     bytes: [u8; 16],
// LOWERING-X86_64-GNU-NEXT:     u64: u64,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: static mut checks: i32 = 0;
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: static mut failures: i32 = 0;
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     static mut ext_global_ld: LongDouble;
// LOWERING-X86_64-GNU-NEXT:     static mut stderr: *mut libc::FILE;
// LOWERING-X86_64-GNU-NEXT:     fn fprintf(_0: *mut libc::FILE, _1: *const core::ffi::c_char, ...) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn ext_sizeof_ld() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn ext_alignof_ld() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn ext_sizeof_box() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn ext_alignof_box() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn ext_offset_box_x() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn ext_offset_box_tail() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn ext_sizeof_pair() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn ext_alignof_pair() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn ext_sizeof_nested() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn ext_alignof_nested() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn ext_offset_nested_pair() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn ext_offset_nested_z() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn ext_sizeof_union() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn ext_alignof_union() -> usize;
// LOWERING-X86_64-GNU-NEXT:     fn nextafterl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn strtold(_0: *const core::ffi::c_char, _1: *mut *mut core::ffi::c_char) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_add(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_sub(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_mul(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_div(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_neg(_0: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_from_i64(_0: i64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_from_u64(_0: u64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_to_i64(_0: LongDouble) -> i64;
// LOWERING-X86_64-GNU-NEXT:     fn ext_to_u64(_0: LongDouble) -> u64;
// LOWERING-X86_64-GNU-NEXT:     fn ext_from_double(_0: f64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_from_float(_0: f32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_to_double(_0: LongDouble) -> f64;
// LOWERING-X86_64-GNU-NEXT:     fn ext_to_float(_0: LongDouble) -> f32;
// LOWERING-X86_64-GNU-NEXT:     fn nanl(_0: *const core::ffi::c_char) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_identity(_0: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_eq(_0: LongDouble, _1: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn ext_lt(_0: LongDouble, _1: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn ext_le(_0: LongDouble, _1: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn __errno_location() -> *mut i32;
// LOWERING-X86_64-GNU-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn snprintf(_0: *mut core::ffi::c_char, _1: usize, _2: *const core::ffi::c_char, ...) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn frexpl(_0: LongDouble, _1: *mut i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ldexpl(_0: LongDouble, _1: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn sqrtl(_0: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn powl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn fmodl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn remainderl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_sum10(
// LOWERING-X86_64-GNU-NEXT:         _0: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _1: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _2: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _3: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _4: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _5: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _6: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _7: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _8: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _9: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     ) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_mix_abi(
// LOWERING-X86_64-GNU-NEXT:         _0: i64,
// LOWERING-X86_64-GNU-NEXT:         _1: f64,
// LOWERING-X86_64-GNU-NEXT:         _2: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _3: u32,
// LOWERING-X86_64-GNU-NEXT:         _4: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _5: f32,
// LOWERING-X86_64-GNU-NEXT:         _6: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _7: i32,
// LOWERING-X86_64-GNU-NEXT:         _8: f64,
// LOWERING-X86_64-GNU-NEXT:         _9: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     ) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_call_cb(
// LOWERING-X86_64-GNU-NEXT:         _0: Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         _1: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _2: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     ) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_box_roundtrip(_0: ld_box) -> ld_box;
// LOWERING-X86_64-GNU-NEXT:     fn ext_pair_make(_0: LongDouble, _1: LongDouble) -> ld_pair;
// LOWERING-X86_64-GNU-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:     fn ext_nested_roundtrip(_0: ld_nested) -> ld_nested;
// LOWERING-X86_64-GNU-NEXT:     fn ext_array_sum(_0: *const LongDouble, _1: usize) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_store(_0: *mut LongDouble, _1: LongDouble);
// LOWERING-X86_64-GNU-NEXT:     fn ext_load(_0: *const LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn memcpy(
// LOWERING-X86_64-GNU-NEXT:         _0: *mut core::ffi::c_void,
// LOWERING-X86_64-GNU-NEXT:         _1: *const core::ffi::c_void,
// LOWERING-X86_64-GNU-NEXT:         _2: usize,
// LOWERING-X86_64-GNU-NEXT:     ) -> *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:     fn ext_vsum(_0: i32, ...) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_global_get() -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn ext_global_set(_0: LongDouble);
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: struct __SlateVaArg {
// LOWERING-X86_64-GNU-NEXT:     value: Box<dyn std::any::Any>,
// LOWERING-X86_64-GNU-NEXT:     size: usize,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl __SlateVaArg {
// LOWERING-X86_64-GNU-NEXT:     fn new<T: 'static>(value: T) -> Self {
// LOWERING-X86_64-GNU-NEXT:         Self {
// LOWERING-X86_64-GNU-NEXT:             value: Box::new(value),
// LOWERING-X86_64-GNU-NEXT:             size: std::mem::size_of::<T>(),
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// LOWERING-X86_64-GNU-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// LOWERING-X86_64-GNU-NEXT:             return *value;
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// LOWERING-X86_64-GNU-NEXT:         unsafe {
// LOWERING-X86_64-GNU-NEXT:             std::ptr::read_unaligned(
// LOWERING-X86_64-GNU-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// LOWERING-X86_64-GNU-NEXT:             )
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[derive(Clone)]
// LOWERING-X86_64-GNU-NEXT: struct __SlateVaArgs {
// LOWERING-X86_64-GNU-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// LOWERING-X86_64-GNU-NEXT:     index: usize,
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: impl __SlateVaArgs {
// LOWERING-X86_64-GNU-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// LOWERING-X86_64-GNU-NEXT:         Self {
// LOWERING-X86_64-GNU-NEXT:             args: Some(std::rc::Rc::new(args)),
// LOWERING-X86_64-GNU-NEXT:             index: 0,
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     const fn empty() -> Self {
// LOWERING-X86_64-GNU-NEXT:         Self {
// LOWERING-X86_64-GNU-NEXT:             args: None,
// LOWERING-X86_64-GNU-NEXT:             index: 0,
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// LOWERING-X86_64-GNU-NEXT:         let index = self.index;
// LOWERING-X86_64-GNU-NEXT:         self.index += 1;
// LOWERING-X86_64-GNU-NEXT:         if std::mem::size_of::<T>() == 0 {
// LOWERING-X86_64-GNU-NEXT:             return unsafe { std::mem::zeroed() };
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// LOWERING-X86_64-GNU-NEXT:         args[index].read::<T>()
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let mut __retval: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     test_format_and_layout();
// LOWERING-X86_64-GNU-NEXT:     test_literals_and_precision_boundaries();
// LOWERING-X86_64-GNU-NEXT:     test_arithmetic_and_temporaries();
// LOWERING-X86_64-GNU-NEXT:     test_casts_and_usual_conversions();
// LOWERING-X86_64-GNU-NEXT:     test_special_values_and_comparisons();
// LOWERING-X86_64-GNU-NEXT:     test_libc_and_libm_externs();
// LOWERING-X86_64-GNU-NEXT:     test_extern_function_abi();
// LOWERING-X86_64-GNU-NEXT:     test_struct_union_array_and_pointer_abi();
// LOWERING-X86_64-GNU-NEXT:     test_varargs();
// LOWERING-X86_64-GNU-NEXT:     test_extern_global();
// LOWERING-X86_64-GNU-NEXT:     test_control_flow_and_spills();
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: *mut i8 = b"long-double torture: %d/%d checks FAILED\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                 fprintf(
// LOWERING-X86_64-GNU-NEXT:                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                 )
// LOWERING-X86_64-GNU-NEXT:             };
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-NEXT:             __retval = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-X86_64-GNU-NEXT:             std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"long-double torture: PASS (%d checks)\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-X86_64-GNU-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn test_format_and_layout() {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:         b"long double: kind=%s sizeof=%zu align=%zu mant=%d max_exp=%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = ld_kind();
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = 64 as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = 16384 as i32;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 53;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 16384;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 1024;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = true;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 64;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 64;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 16384;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 16384;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                         } else {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = true;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 64;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 113;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 16384;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 16384;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                         } else {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 192;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"(LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024) || (LDBL_MANT_DIG == 64 && LDBL_MAX_EXP == 16384) || (LDBL_MANT_DIG == 113 && LDBL_MAX_EXP == 16384)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_sizeof_ld() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 194;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"sizeof(long double) == ext_sizeof_ld()\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_alignof_ld() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 195;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"(size_t)ALIGNOF(long double) == ext_alignof_ld()\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_box>() as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_sizeof_box() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 197;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"sizeof(struct ld_box) == ext_sizeof_box()\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_box>() as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_alignof_box() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 198;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"(size_t)ALIGNOF(struct ld_box) == ext_alignof_box()\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                 as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_offset_box_x() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 199;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"offsetof(struct ld_box, x) == ext_offset_box_x()\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_pair>() as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_offset_box_tail() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 200;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"offsetof(struct ld_box, tail) == ext_offset_box_tail()\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                 as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = 32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_sizeof_pair() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 202;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"sizeof(struct ld_pair) == ext_sizeof_pair()\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_pair>() as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_alignof_pair() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 203;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"(size_t)ALIGNOF(struct ld_pair) == ext_alignof_pair()\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                 as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_nested>() as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_sizeof_nested() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 205;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"sizeof(struct ld_nested) == ext_sizeof_nested()\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_nested>() as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_alignof_nested() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 206;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"(size_t)ALIGNOF(struct ld_nested) == ext_alignof_nested()\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                 as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_union>() as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_offset_nested_pair() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 207;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"offsetof(struct ld_nested, pair) == ext_offset_nested_pair()\0"
// LOWERING-X86_64-GNU-NEXT:                                 .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = 64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_offset_nested_z() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 208;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"offsetof(struct ld_nested, z) == ext_offset_nested_z()\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                 as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_union>() as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_sizeof_union() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 210;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"sizeof(union ld_union) == ext_sizeof_union()\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = (unsafe { ext_alignof_union() }) as u64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 211;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"(size_t)ALIGNOF(union ld_union) == ext_alignof_union()\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                 as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: /// 2^53 + 1
// LOWERING-X86_64-GNU-NEXT: fn test_literals_and_precision_boundaries() {
// LOWERING-X86_64-GNU-NEXT:     let mut one: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut eps: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut n1: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut n2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut via_parse_64: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut via_parse_113: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut end: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:     one = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 192, 63]);
// LOWERING-X86_64-GNU-NEXT:     eps = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 9007199254740993u64;
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = one;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = eps;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = one;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 224;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"one + eps != one\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = one;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = eps;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = one;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 check_got_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = eps;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 225;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"(one + eps) - one\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"eps\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = one;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_nextafterl__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = one;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 check_got_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = eps;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 226;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"nextafterl(one, 2.0L) - one\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"eps\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     n1 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 9007199254740992u64;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     n2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = n1;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = n2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 233;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"n1 != n2\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     end = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"0x1.0000000000000002p0\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:         __slate_strtold__rf80_pc_ppc(
// LOWERING-X86_64-GNU-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     via_parse_64 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: *mut i8 = end;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = end;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 238;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"end != NULL && *end == '\\0'\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = via_parse_64;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 240;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"via_parse_64 != 1.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = via_parse_64;
// LOWERING-X86_64-GNU-NEXT:                 check_got_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 241;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"via_parse_64\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"0x1.0000000000000002p0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     end = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"0x1.0000000000000000000000000001p0\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:         __slate_strtold__rf80_pc_ppc(
// LOWERING-X86_64-GNU-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     via_parse_113 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: *mut i8 = end;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = end;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 248;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"end != NULL && *end == '\\0'\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = via_parse_113;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 253;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"via_parse_113 == 1.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let mut wide: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let mut through_double: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:         wide = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f64 = 1.0;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:         through_double = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = wide;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = through_double;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 261;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"wide != through_double\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 fprintf(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let mut huge_but_finite: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 207, 71]);
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(huge_but_finite), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(huge_but_finite)) };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class({{_v[0-9]+}}, 504);
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 268;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"isfinite(huge_but_finite)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 fprintf(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(huge_but_finite)) };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: f64 = 179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 269;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"huge_but_finite > (long double)DBL_MAX\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 fprintf(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: /// Conditional operator and comma operator force long-double temporaries.
// LOWERING-X86_64-GNU-NEXT: /// Conditional operator and comma operator force long-double temporaries.
// LOWERING-X86_64-GNU-NEXT: fn test_arithmetic_and_temporaries() {
// LOWERING-X86_64-GNU-NEXT:     let mut a: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut b: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut c: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut x: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut y: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([128, 247, 230, 213, 196, 179, 162, 145, 19, 64]);
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 42, 59, 76, 93, 110, 127, 128, 248, 191]);
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(b), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 2, 64]);
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(c), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 check_got_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_expected_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 281;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"a + b\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_add(a, b)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 check_got_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_sub__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_expected_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 282;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"a - b\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_sub(a, b)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 check_got_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_mul__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_expected_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 283;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"a * b\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_mul(a, b)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_4: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_4: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 check_got_4 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_div__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_expected_4 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_4;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_4;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 284;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"a / c\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_div(a, c)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_4;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_4;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_5: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_5: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = -{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 check_got_5 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_neg__rf80_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_expected_5 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_5;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_5;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 285;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"-a\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_neg(a)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_5;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_5;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 252, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 128, 128, 244, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = -{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_fma({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     x = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_mul__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_div__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_sub__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 252, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_div__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 128, 128, 244, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_mul__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_sub__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     y = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_6: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_6: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = x;
// LOWERING-X86_64-GNU-NEXT:                 check_got_6 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = y;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_6 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_6;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_6;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 291;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"x\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"y\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_6;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_6;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let mut q: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 253, 63]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_7: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_7: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// LOWERING-X86_64-GNU-NEXT:                     check_got_7 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 136, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                     check_expected_7 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_7;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_7;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 299;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"q\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"2.125L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_7;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_7;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// LOWERING-X86_64-GNU-NEXT:         todo!("long double constant without Clang AST value");
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + _200;
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_8: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_8: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// LOWERING-X86_64-GNU-NEXT:                     check_got_8 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                     check_expected_8 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 301;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"q\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"3.125L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// LOWERING-X86_64-GNU-NEXT:         todo!("long double constant without Clang AST value");
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - _203;
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_9: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_9: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// LOWERING-X86_64-GNU-NEXT:                     check_got_9 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 136, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                     check_expected_9 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_9;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_9;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 303;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"q\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"2.125L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_9;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_9;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_10: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_10: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:                 check_got_10 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-X86_64-GNU-NEXT:                 check_expected_10 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_10;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_10;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 307;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"(1 ? a : (double)b)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"a\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_10;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_10;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_11: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_11: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-X86_64-GNU-NEXT:                 x = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = x;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 check_got_11 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 check_expected_11 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_11;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_11;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 308;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"(x = b, x + 1.0L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"b + 1.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_11;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_11;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn test_casts_and_usual_conversions() {
// LOWERING-X86_64-GNU-NEXT:     let mut si: aligned::Aligned<aligned::A16, [i64; 7]> = aligned::Aligned([0; 7]);
// LOWERING-X86_64-GNU-NEXT:     let mut ui: aligned::Aligned<aligned::A16, [u64; 6]> = aligned::Aligned([0; 6]);
// LOWERING-X86_64-GNU-NEXT:     let mut i: u64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: [i64; 7] = [
// LOWERING-X86_64-GNU-NEXT:         0,
// LOWERING-X86_64-GNU-NEXT:         1,
// LOWERING-X86_64-GNU-NEXT:         -1,
// LOWERING-X86_64-GNU-NEXT:         2147483647,
// LOWERING-X86_64-GNU-NEXT:         -2147483647,
// LOWERING-X86_64-GNU-NEXT:         9007199254740991,
// LOWERING-X86_64-GNU-NEXT:         -9007199254740991,
// LOWERING-X86_64-GNU-NEXT:     ];
// LOWERING-X86_64-GNU-NEXT:     *si = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: [u64; 6] = [
// LOWERING-X86_64-GNU-NEXT:         0,
// LOWERING-X86_64-GNU-NEXT:         1,
// LOWERING-X86_64-GNU-NEXT:         4294967295,
// LOWERING-X86_64-GNU-NEXT:         9007199254740991,
// LOWERING-X86_64-GNU-NEXT:         9007199254740993,
// LOWERING-X86_64-GNU-NEXT:         18446744073709551615,
// LOWERING-X86_64-GNU-NEXT:     ];
// LOWERING-X86_64-GNU-NEXT:     *ui = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-X86_64-GNU-NEXT:         i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = 56;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = 8;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 loop {
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let mut check_got_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                         let mut check_expected_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i64 = si[({{_v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         check_got_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i64 = si[({{_v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                             unsafe { __slate_ext_from_i64__rf80_i64({{_v[0-9]+}} as i64) };
// LOWERING-X86_64-GNU-NEXT:                         check_expected_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                                     failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                 }
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                     b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                         as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = 328;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: *mut i8 = b"(long double)si[i]\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: *mut i8 = b"ext_from_i64(si[i])\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                     __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     )
// LOWERING-X86_64-GNU-NEXT:                                 };
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                     if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         break;
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:             i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-X86_64-GNU-NEXT:         i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = 48;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = 8;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 loop {
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let mut check_got_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                         let mut check_expected_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: u64 = ui[({{_v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         check_got_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: u64 = ui[({{_v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                             unsafe { __slate_ext_from_u64__rf80_u64({{_v[0-9]+}} as u64) };
// LOWERING-X86_64-GNU-NEXT:                         check_expected_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                                     failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                 }
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                     b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                         as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = 331;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: *mut i8 = b"(long double)ui[i]\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: *mut i8 = b"ext_from_u64(ui[i])\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                     __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                         {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     )
// LOWERING-X86_64-GNU-NEXT:                                 };
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                     if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         break;
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:             i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 63, 180, 150, 19, 64]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i64 = __slate_f80_to_i64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 63, 180, 150, 19, 64]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i64 = unsafe { __slate_ext_to_i64__ri64_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 333;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"(int64_t)1234567.875L == ext_to_i64(1234567.875L)\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 63, 180, 150, 19, 192]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i64 = __slate_f80_to_i64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 63, 180, 150, 19, 192]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i64 = unsafe { __slate_ext_to_i64__ri64_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 334;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"(int64_t)-1234567.875L == ext_to_i64(-1234567.875L)\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 63, 180, 150, 19, 64]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = __slate_f80_to_u64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 63, 180, 150, 19, 64]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { __slate_ext_to_u64__ru64_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 335;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"(uint64_t)1234567.875L == ext_to_u64(1234567.875L)\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 254, 191]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i64 = __slate_f80_to_i64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 336;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"(int64_t)-0.875L == 0\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let mut d: f64 = 0.0;
// LOWERING-X86_64-GNU-NEXT:         let mut f: f32 = 0.0;
// LOWERING-X86_64-GNU-NEXT:         let mut ld1: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let mut ld2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f64 = 1048576.0000000002;
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f32 = 1024.00012;
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:         ld1 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:         ld2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = ld1;
// LOWERING-X86_64-GNU-NEXT:                     check_got_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { __slate_ext_from_double__rf80_f64({{_v[0-9]+}} as f64) };
// LOWERING-X86_64-GNU-NEXT:                     check_expected_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 343;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"ld1\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"ext_from_double(d)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_4: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_4: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = ld2;
// LOWERING-X86_64-GNU-NEXT:                     check_got_4 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { __slate_ext_from_float__rf80_f32({{_v[0-9]+}} as f32) };
// LOWERING-X86_64-GNU-NEXT:                     check_expected_4 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_4;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_4;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 344;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"ld2\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"ext_from_float(f)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_4;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_4;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = ld1;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: f64 = __slate_f80_to_f64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = ld1;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: f64 = unsafe { __slate_ext_to_double__rf64_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 345;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"(double)ld1 == ext_to_double(ld1)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 fprintf(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = ld2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: f32 = __slate_f80_to_f32({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = ld2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: f32 = unsafe { __slate_ext_to_float__rf32_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 346;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"(float)ld2 == ext_to_float(ld2)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 fprintf(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let mut i32: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:         let mut u64: u64 = 0;
// LOWERING-X86_64-GNU-NEXT:         let mut f2: f32 = 0.0;
// LOWERING-X86_64-GNU-NEXT:         let mut d2: f64 = 0.0;
// LOWERING-X86_64-GNU-NEXT:         let mut ld: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = -17;
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(i32), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 9007199254740993u64;
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(u64), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f32 = 0.25;
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f2), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f64 = -0.125;
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d2), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(ld), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_5: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_5: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i32)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     check_got_5 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i32)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     check_expected_5 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_5;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_5;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 357;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"ld + i32\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"ext_add(ld, (long double)i32)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_5;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_5;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_6: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_6: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(u64)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     check_got_6 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(u64)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     check_expected_6 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_6;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_6;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 358;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"ld + u64\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"ext_add(ld, (long double)u64)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_6;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_6;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_7: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_7: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f2)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     check_got_7 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f2)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     check_expected_7 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_7;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_7;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 359;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"ld + f\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"ext_add(ld, (long double)f)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_7;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_7;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_8: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_8: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d2)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     check_got_8 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d2)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     check_expected_8 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 360;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"ld + d\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"ext_add(ld, (long double)d)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn test_special_values_and_comparisons() {
// LOWERING-X86_64-GNU-NEXT:     let mut pz: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut nz: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut inf: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut ninf: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut nan: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:     pz = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// LOWERING-X86_64-GNU-NEXT:     nz = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: f32 = f32::from_bits(0x7f800000);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     inf = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: f32 = f32::from_bits(0xff800000);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     ninf = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_nanl__rf80_pc({{_v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-X86_64-GNU-NEXT:     nan = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = pz;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nz;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 371;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"pz == nz\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = pz;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 372;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"!signbit(pz)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nz;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 373;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"signbit(nz)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = pz;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_identity__rf80_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_got_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = pz;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"FAIL line %d: signed-zero mismatch\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 374;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = nz;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_identity__rf80_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_got_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = nz;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"FAIL line %d: signed-zero mismatch\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 375;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = pz;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_neg__rf80_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_got_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = nz;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"FAIL line %d: signed-zero mismatch\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 376;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_4: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_4: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = nz;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_neg__rf80_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_got_4 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = pz;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_4 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_4;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_4;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_4;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_4;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"FAIL line %d: signed-zero mismatch\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 377;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = inf;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class({{_v[0-9]+}}, 516);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = inf;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 379;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"isinf(inf) && inf > 0.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = ninf;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class({{_v[0-9]+}}, 516);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = ninf;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 380;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"isinf(ninf) && ninf < 0.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nan;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class({{_v[0-9]+}}, 3);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 381;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"isnan(nan)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nan;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nan;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 382;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"!(nan == nan)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nan;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 383;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"!(nan < 0.0L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nan;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 384;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"!(nan > 0.0L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nan;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} <= {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 385;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"!(nan <= 0.0L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nan;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} >= {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 386;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"!(nan >= 0.0L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nan;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nan;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 387;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"nan != nan\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = pz;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nz;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { __slate_ext_eq__ri32_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 389;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_eq(pz, nz)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { __slate_ext_lt__ri32_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 390;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_lt(-1.0L, 1.0L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { __slate_ext_le__ri32_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 391;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_le(1.0L, 1.0L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nan;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nan;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { __slate_ext_eq__ri32_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 392;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"!ext_eq(nan, nan)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nan;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { __slate_ext_lt__ri32_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 393;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"!ext_lt(nan, 0.0L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = nz;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = __slate_f80_copysign({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 395;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"signbit(copysignl(1.0L, nz))\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = pz;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = __slate_f80_copysign({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 396;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"!signbit(copysignl(1.0L, pz))\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn test_libc_and_libm_externs() {
// LOWERING-X86_64-GNU-NEXT:     let mut end: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut x: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut ip: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut fp: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut fr: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut e: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i8; 256]> = aligned::Aligned([0; 256]);
// LOWERING-X86_64-GNU-NEXT:     let mut end2: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut n: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"  -0x1.23456789abcdef0123456789p+17tail\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     end = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     e = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     end2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { __errno_location() };
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:         __slate_strtold__rf80_pc_ppc(
// LOWERING-X86_64-GNU-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     x = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: *mut i8 = end;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 413;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"end != NULL\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: *mut i8 = end;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b"tail\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                         strcmp(
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                         )
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 414;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"strcmp(end, \"tail\") == 0\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: *mut i32 = unsafe { __errno_location() };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 415;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"errno == 0\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = x;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 416;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"x < 0.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 256;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%La\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = x;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:         __slate_snprintf__ri32_pi8_u64_pi8_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as u64, {{_v[0-9]+}} as *mut i8, {{_v[0-9]+}})
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     n = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = n;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = n;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: u64 = 256;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 420;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"n > 0 && (size_t)n < sizeof(buf)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:                     __slate_strtold__rf80_pc_ppc(
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                         std::ptr::addr_of_mut!(end2) as *mut *mut core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                     )
// LOWERING-X86_64-GNU-NEXT:                 };
// LOWERING-X86_64-GNU-NEXT:                 check_got_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = x;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 421;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"strtold(buf, &end2)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"x\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: *mut i8 = end2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = end2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 422;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"end2 != NULL && *end2 == '\\0'\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 231, 192, 12, 192]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_fract({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_trunc({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     ip = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     fp = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = ip;
// LOWERING-X86_64-GNU-NEXT:                 check_got_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 228, 192, 12, 192]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 425;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ip\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"-12345.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = fp;
// LOWERING-X86_64-GNU-NEXT:                 check_got_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 254, 191]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 426;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"fp\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"-0.75L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 9, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:         unsafe { __slate_frexpl__rf80_f80_pi32({{_v[0-9]+}}, std::ptr::addr_of_mut!(e) as *mut i32) };
// LOWERING-X86_64-GNU-NEXT:     fr = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_4: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_4: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = fr;
// LOWERING-X86_64-GNU-NEXT:                 check_got_4 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 254, 63]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_4 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_4;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_4;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 429;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"fr\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"0.75L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_4;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_4;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = e;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 11;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 430;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"e == 11\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_5: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_5: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = fr;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = e;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                     unsafe { __slate_ldexpl__rf80_f80_i32({{_v[0-9]+}}, {{_v[0-9]+}} as i32) };
// LOWERING-X86_64-GNU-NEXT:                 check_got_5 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 9, 64]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_5 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_5;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_5;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 431;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ldexpl(fr, e)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"0x1.8p+10L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_5;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_5;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_6: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_6: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_sqrtl__rf80_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_got_6 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_6 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_6;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_6;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 433;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"sqrtl(4.0L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"2.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_6;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_6;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_7: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_7: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_powl__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_got_7 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 216, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_7 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_7;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_7;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 434;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"powl(1.5L, 3.0L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"3.375L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_7;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_7;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_8: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_8: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = __slate_f80_fma({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                 check_got_8 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_8 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_8;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_8;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 435;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"fmal(2.0L, 3.0L, 4.0L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"10.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_9: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_9: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 140, 3, 64]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_fmodl__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_got_9 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_9 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_9;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_9;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 436;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"fmodl(17.5L, 3.0L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"2.5L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_9;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_9;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_10: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_10: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 176, 1, 64]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_remainderl__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_got_10 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 191]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_10 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_10;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_10;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 437;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"remainderl(5.5L, 2.0L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"-0.5L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_10;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_10;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_11: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_11: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 255, 191]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = __slate_f80_floor({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                 check_got_11 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 192]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_11 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_11;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_11;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 438;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"floorl(-1.25L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"-2.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_11;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_11;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_12: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_12: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 255, 191]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = __slate_f80_ceil({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                 check_got_12 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_12 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_12;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_12;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 439;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ceill(-1.25L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"-1.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_12;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_12;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_13: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_13: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 255, 191]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = __slate_f80_trunc({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                 check_got_13 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_13 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_13;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_13;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 440;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"truncl(-1.75L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"-1.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_13;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_13;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let mut toward: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { __slate_nextafterl__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         toward = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = toward;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 444;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"toward > 1.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 fprintf(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_14: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_14: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = toward;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     check_got_14 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 192, 63]);
// LOWERING-X86_64-GNU-NEXT:                     check_expected_14 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_14;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_14;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 445;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"toward - 1.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"LDBL_EPSILON\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_14;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_14;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let mut tiny: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { __slate_nextafterl__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         tiny = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = tiny;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 450;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"tiny > 0.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 fprintf(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = tiny;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 0]);
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 451;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"tiny < LDBL_MIN\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 fprintf(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = tiny;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class({{_v[0-9]+}}, 96);
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 2;
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                         } else {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class({{_v[0-9]+}}, 3);
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                             } else {
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class({{_v[0-9]+}}, 516);
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                                     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                                 } else {
// LOWERING-X86_64-GNU-NEXT:                                     let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class({{_v[0-9]+}}, 264);
// LOWERING-X86_64-GNU-NEXT:                                     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-NEXT:                                     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-X86_64-GNU-NEXT:                                     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                                 };
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 452;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"fpclassify(tiny) == FP_SUBNORMAL\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 fprintf(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn test_extern_function_abi() {
// LOWERING-X86_64-GNU-NEXT:     let mut a0: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut a1: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut a2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut a3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut a4: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut a5: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut a6: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut a7: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut a8: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut a9: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut expected: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:     a0 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// LOWERING-X86_64-GNU-NEXT:     a1 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 253, 63]);
// LOWERING-X86_64-GNU-NEXT:     a2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 252, 63]);
// LOWERING-X86_64-GNU-NEXT:     a3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 251, 63]);
// LOWERING-X86_64-GNU-NEXT:     a4 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 250, 63]);
// LOWERING-X86_64-GNU-NEXT:     a5 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 249, 63]);
// LOWERING-X86_64-GNU-NEXT:     a6 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 248, 63]);
// LOWERING-X86_64-GNU-NEXT:     a7 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 247, 63]);
// LOWERING-X86_64-GNU-NEXT:     a8 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 246, 63]);
// LOWERING-X86_64-GNU-NEXT:     a9 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = a0;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = a1;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = a2;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = a3;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = a4;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = a5;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = a6;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = a7;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = a8;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = a9;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     expected = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([120, 111, 94, 77, 60, 43, 26, 137, 39, 64]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_identity__rf80_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                 check_got_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([120, 111, 94, 77, 60, 43, 26, 137, 39, 64]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 471;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_identity(0x1.123456789abcdef012345678p+40L)\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"0x1.123456789abcdef012345678p+40L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = a0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = a1;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = a2;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = a3;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = a4;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = a5;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = a6;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = a7;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = a8;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = a9;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:                     __slate_ext_sum10__rf80_f80_f80_f80_f80_f80_f80_f80_f80_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                     )
// LOWERING-X86_64-GNU-NEXT:                 };
// LOWERING-X86_64-GNU-NEXT:                 check_got_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = expected;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 472;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_sum10(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9)\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"expected\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let mut i0: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:         let mut d0: f64 = 0.0;
// LOWERING-X86_64-GNU-NEXT:         let mut a: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let mut u0: u32 = 0;
// LOWERING-X86_64-GNU-NEXT:         let mut b: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let mut f0: f32 = 0.0;
// LOWERING-X86_64-GNU-NEXT:         let mut c: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let mut i1: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:         let mut d1: f64 = 0.0;
// LOWERING-X86_64-GNU-NEXT:         let mut d: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let mut local: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i64 = -123456789;
// LOWERING-X86_64-GNU-NEXT:         i0 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f64 = 192.0;
// LOWERING-X86_64-GNU-NEXT:         d0 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([120, 111, 94, 77, 60, 43, 26, 137, 4, 64]);
// LOWERING-X86_64-GNU-NEXT:         a = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u32 = 12345;
// LOWERING-X86_64-GNU-NEXT:         u0 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 8, 25, 42, 59, 204, 252, 191]);
// LOWERING-X86_64-GNU-NEXT:         b = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f32 = 0.75;
// LOWERING-X86_64-GNU-NEXT:         f0 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 128, 128, 8, 64]);
// LOWERING-X86_64-GNU-NEXT:         c = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = -77;
// LOWERING-X86_64-GNU-NEXT:         i1 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f64 = -0.125;
// LOWERING-X86_64-GNU-NEXT:         d1 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 128, 255, 255, 244, 63]);
// LOWERING-X86_64-GNU-NEXT:         d = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = a;
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = b;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = c;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = d;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i64 = i0;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u32 = u0;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u32({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f64 = d0;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f32 = f0;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = i1;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f64 = d1;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i64 = i0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: f64 = d0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = a;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = u0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = b;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: f32 = f0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = c;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = i1;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: f64 = d1;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = d;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:                         __slate_ext_mix_abi__rf80_i64_f64_f80_u32_f80_f32_f80_i32_f64_f80(
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}} as i64,
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}} as f64,
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}} as u32,
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}} as f32,
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}} as f64,
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                         )
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     check_got_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                         unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// LOWERING-X86_64-GNU-NEXT:                     check_expected_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 497;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"ext_mix_abi(i0, d0, a, u0, b, f0, c, i1, d1, d)\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"local\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let mut fp: Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble> =
// LOWERING-X86_64-GNU-NEXT:             None;
// LOWERING-X86_64-GNU-NEXT:         unsafe {
// LOWERING-X86_64-GNU-NEXT:             std::ptr::write_volatile(std::ptr::addr_of_mut!(fp), unsafe {
// LOWERING-X86_64-GNU-NEXT:                 std::mem::transmute::<
// LOWERING-X86_64-GNU-NEXT:                     *const (),
// LOWERING-X86_64-GNU-NEXT:                     Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// LOWERING-X86_64-GNU-NEXT:                 >(__slate_ext_add__rf80_f80_f80 as *const ())
// LOWERING-X86_64-GNU-NEXT:             })
// LOWERING-X86_64-GNU-NEXT:         };
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_4: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_4: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: Option<
// LOWERING-X86_64-GNU-NEXT:                         unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble,
// LOWERING-X86_64-GNU-NEXT:                     > = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fp)) };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 128, 145, 3, 64]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 248, 191]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     check_got_4 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 128, 145, 3, 64]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 248, 191]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     check_expected_4 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_4;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_4;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 504;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"fp(0x1.23p+4L, -0x1.2p-7L)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"0x1.23p+4L + -0x1.2p-7L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_4;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_4;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let mut a10: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let mut b2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 128, 196, 179, 162, 145, 7, 64]);
// LOWERING-X86_64-GNU-NEXT:         a10 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 240, 230, 213, 251, 191]);
// LOWERING-X86_64-GNU-NEXT:         b2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_5: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_5: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = a10;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = b2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:                         __slate_ext_call_cb__rf80_x_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 std::mem::transmute::<
// LOWERING-X86_64-GNU-NEXT:                                     *const (),
// LOWERING-X86_64-GNU-NEXT:                                     Option<
// LOWERING-X86_64-GNU-NEXT:                                         unsafe extern "C-unwind" fn(
// LOWERING-X86_64-GNU-NEXT:                                             LongDouble,
// LOWERING-X86_64-GNU-NEXT:                                             LongDouble,
// LOWERING-X86_64-GNU-NEXT:                                         )
// LOWERING-X86_64-GNU-NEXT:                                             -> LongDouble,
// LOWERING-X86_64-GNU-NEXT:                                     >,
// LOWERING-X86_64-GNU-NEXT:                                 >(__slate_ld_local_cb as *const ())
// LOWERING-X86_64-GNU-NEXT:                             },
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                         )
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     check_got_5 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = a10;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = b2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = local_cb({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     check_expected_5 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_5;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_5;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 511;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"ext_call_cb(local_cb, a, b)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"local_cb(a, b)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_5;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_5;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn test_struct_union_array_and_pointer_abi() {
// LOWERING-X86_64-GNU-NEXT:     let mut b: ld_box = ld_box {
// LOWERING-X86_64-GNU-NEXT:         tag: 0,
// LOWERING-X86_64-GNU-NEXT:         x: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:         tail: 0,
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let mut r: ld_box = ld_box {
// LOWERING-X86_64-GNU-NEXT:         tag: 0,
// LOWERING-X86_64-GNU-NEXT:         x: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:         tail: 0,
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let mut p: ld_pair = ld_pair {
// LOWERING-X86_64-GNU-NEXT:         a: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:         b: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let mut n: ld_nested = ld_nested {
// LOWERING-X86_64-GNU-NEXT:         head: 0,
// LOWERING-X86_64-GNU-NEXT:         pair: ld_pair {
// LOWERING-X86_64-GNU-NEXT:             a: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:             b: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:         },
// LOWERING-X86_64-GNU-NEXT:         bytes: [0; 3],
// LOWERING-X86_64-GNU-NEXT:         z: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let mut nr: ld_nested = ld_nested {
// LOWERING-X86_64-GNU-NEXT:         head: 0,
// LOWERING-X86_64-GNU-NEXT:         pair: ld_pair {
// LOWERING-X86_64-GNU-NEXT:             a: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:             b: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:         },
// LOWERING-X86_64-GNU-NEXT:         bytes: [0; 3],
// LOWERING-X86_64-GNU-NEXT:         z: LongDouble([0; 10]),
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let mut arr: [LongDouble; 7] = [LongDouble([0; 10]); 7];
// LOWERING-X86_64-GNU-NEXT:     let mut copy: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut raw: aligned::Aligned<aligned::A16, [u8; 16]> = aligned::Aligned([0; 16]);
// LOWERING-X86_64-GNU-NEXT:     let mut i: u64 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u8 = 49;
// LOWERING-X86_64-GNU-NEXT:     b.tag = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([128, 247, 230, 213, 196, 179, 162, 145, 11, 64]);
// LOWERING-X86_64-GNU-NEXT:     b.x = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u32 = 305419896;
// LOWERING-X86_64-GNU-NEXT:     b.tail = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: ld_box = b;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: ld_box = unsafe { __slate_ext_box_roundtrip__rx_x({{_v[0-9]+}} as ld_box) };
// LOWERING-X86_64-GNU-NEXT:     r = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u8 = r.tag;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u8 = b.tag;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = 90;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} as u8;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 530;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"r.tag == (unsigned char)(b.tag ^ 0x5aU)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = r.x;
// LOWERING-X86_64-GNU-NEXT:                 check_got_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = b.x;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 246, 63]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 check_expected_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 531;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"r.x\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"b.x + 0x1p-9L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = r.tail;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = b.tail;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = 2779077210u32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 532;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"r.tail == (b.tail ^ UINT32_C(0xa5a55a5a))\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([136, 136, 136, 136, 136, 136, 136, 136, 2, 192]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([16, 17, 17, 17, 17, 17, 17, 145, 252, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: ld_pair = unsafe { __slate_ext_pair_make__rcf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     p = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = p.a;
// LOWERING-X86_64-GNU-NEXT:                 check_got_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([136, 136, 136, 136, 136, 136, 136, 136, 2, 192]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 535;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"p.a\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"-0x1.111111111111111p+3L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = p.b;
// LOWERING-X86_64-GNU-NEXT:                 check_got_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([16, 17, 17, 17, 17, 17, 17, 145, 252, 63]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 536;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"p.b\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"0x1.222222222222222p-3L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(n) as *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 80;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-X86_64-GNU-NEXT:         memset(
// LOWERING-X86_64-GNU-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-X86_64-GNU-NEXT:             {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-X86_64-GNU-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u16 = 4660;
// LOWERING-X86_64-GNU-NEXT:     n.head = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 2, 64]);
// LOWERING-X86_64-GNU-NEXT:     n.pair.a = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 176, 2, 192]);
// LOWERING-X86_64-GNU-NEXT:     n.pair.b = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u8 = 16;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:     n.bytes[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u8 = 32;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:     n.bytes[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u8 = 64;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-NEXT:     n.bytes[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 240, 1, 64]);
// LOWERING-X86_64-GNU-NEXT:     n.z = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: ld_nested = n;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: ld_nested = unsafe { __slate_ext_nested_roundtrip__rx_x({{_v[0-9]+}} as ld_nested) };
// LOWERING-X86_64-GNU-NEXT:     nr = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u16 = nr.head;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u16 = n.head;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 21930;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u16 = {{_v[0-9]+}} as u16;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 547;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"nr.head == (uint16_t)(n.head ^ UINT16_C(0x55aa))\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_4: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_4: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = nr.pair.a;
// LOWERING-X86_64-GNU-NEXT:                 check_got_4 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_4 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_4;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_4;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 548;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"nr.pair.a\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"10.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_4;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_4;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_5: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_5: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = nr.pair.b;
// LOWERING-X86_64-GNU-NEXT:                 check_got_5 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 208, 2, 192]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_5 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_5;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_5;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 549;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"nr.pair.b\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"-13.0L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_5;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_5;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u8 = nr.bytes[({{_v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u8 = n.bytes[({{_v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = 1;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} as u8;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 550;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"nr.bytes[0] == (unsigned char)(n.bytes[0] ^ 1U)\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u8 = nr.bytes[({{_v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u8 = n.bytes[({{_v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = 2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} as u8;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 551;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"nr.bytes[1] == (unsigned char)(n.bytes[1] ^ 2U)\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u8 = nr.bytes[({{_v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i64 = 2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u8 = n.bytes[({{_v[0-9]+}} as usize)];
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} as u32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = 4;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u32 = {{_v[0-9]+}} ^ {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: u8 = {{_v[0-9]+}} as u8;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 552;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"nr.bytes[2] == (unsigned char)(n.bytes[2] ^ 4U)\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_6: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_6: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = nr.z;
// LOWERING-X86_64-GNU-NEXT:                 check_got_6 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 240, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_6 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_6;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_6;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 553;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"nr.z\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"3.75L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_6;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_6;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 0;
// LOWERING-X86_64-GNU-NEXT:         i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = 112;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = 16;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = 1;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u64({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 2, 64]);
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:             arr[({{_v[0-9]+}} as usize)] = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:             i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_7: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_7: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: *mut LongDouble = arr.as_mut_ptr() as *mut LongDouble;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: u64 = 112;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: u64 = 16;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                     unsafe { __slate_ext_array_sum__rf80_pf80_usize({{_v[0-9]+}}, {{_v[0-9]+}} as usize) };
// LOWERING-X86_64-GNU-NEXT:                 check_got_7 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_7 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_7;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_7;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 557;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_array_sum(arr, sizeof(arr) / sizeof(arr[0]))\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"3.5L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_7;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_7;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:     copy = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([192, 179, 162, 145, 128, 247, 230, 213, 32, 192]);
// LOWERING-X86_64-GNU-NEXT:     unsafe { __slate_ext_store__rv_pf80_f80(std::ptr::addr_of_mut!(copy), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_8: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_8: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = copy;
// LOWERING-X86_64-GNU-NEXT:                 check_got_8 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                     LongDouble([192, 179, 162, 145, 128, 247, 230, 213, 32, 192]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_8 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_8;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_8;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 561;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"copy\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"-0x1.abcdef012345678p+33L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_9: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_9: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                     unsafe { __slate_ext_load__rf80_pf80(std::ptr::addr_of_mut!(copy)) };
// LOWERING-X86_64-GNU-NEXT:                 check_got_9 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = copy;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_9 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_9;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_9;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 562;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_load(&copy)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"copy\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_9;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_9;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([112, 86, 52, 146, 239, 205, 171, 137, 235, 63]);
// LOWERING-X86_64-GNU-NEXT:     copy = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut u8 = raw.as_mut_ptr() as *mut u8;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(copy) as *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-X86_64-GNU-NEXT:         memcpy(
// LOWERING-X86_64-GNU-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-X86_64-GNU-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-X86_64-GNU-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-X86_64-GNU-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:     copy = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(copy) as *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut u8 = raw.as_mut_ptr() as *mut u8;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 16;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-X86_64-GNU-NEXT:         memcpy(
// LOWERING-X86_64-GNU-NEXT:             {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-X86_64-GNU-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-X86_64-GNU-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-X86_64-GNU-NEXT:         )
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_10: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_10: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = copy;
// LOWERING-X86_64-GNU-NEXT:                 check_got_10 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([112, 86, 52, 146, 239, 205, 171, 137, 235, 63]);
// LOWERING-X86_64-GNU-NEXT:                 check_expected_10 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_10;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_10;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 570;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"copy\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"0x1.13579bdf2468acep-20L\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_10;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_10;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let mut u: ld_union = unsafe { std::mem::zeroed::<ld_union>() };
// LOWERING-X86_64-GNU-NEXT:         let mut v: ld_union = unsafe { std::mem::zeroed::<ld_union>() };
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([248, 222, 188, 154, 112, 86, 52, 146, 18, 192]);
// LOWERING-X86_64-GNU-NEXT:         unsafe {
// LOWERING-X86_64-GNU-NEXT:             u.ld = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(v) as *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(u) as *mut core::ffi::c_void;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe {
// LOWERING-X86_64-GNU-NEXT:             memcpy(
// LOWERING-X86_64-GNU-NEXT:                 {{_v[0-9]+}} as *mut core::ffi::c_void,
// LOWERING-X86_64-GNU-NEXT:                 {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-X86_64-GNU-NEXT:                 {{_v[0-9]+}} as usize,
// LOWERING-X86_64-GNU-NEXT:             )
// LOWERING-X86_64-GNU-NEXT:         };
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_11: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_11: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = unsafe { v.ld };
// LOWERING-X86_64-GNU-NEXT:                     check_got_11 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = unsafe { u.ld };
// LOWERING-X86_64-GNU-NEXT:                     check_expected_11 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_11;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_11;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 577;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"v.ld\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"u.ld\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_11;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_11;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: /// Translated va_arg(long double).
// LOWERING-X86_64-GNU-NEXT: /// Translated va_arg(long double).
// LOWERING-X86_64-GNU-NEXT: /// Translated caller -> native C variadic callee.
// LOWERING-X86_64-GNU-NEXT: /// Translated caller -> native C variadic callee.
// LOWERING-X86_64-GNU-NEXT: fn test_varargs() {
// LOWERING-X86_64-GNU-NEXT:     let mut a: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut b: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut c: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut d: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut expected: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i8; 128]> = aligned::Aligned([0; 128]);
// LOWERING-X86_64-GNU-NEXT:     let mut end: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     let mut n: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:     a = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 253, 63]);
// LOWERING-X86_64-GNU-NEXT:     b = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 251, 191]);
// LOWERING-X86_64-GNU-NEXT:     c = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 247, 63]);
// LOWERING-X86_64-GNU-NEXT:     d = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = a;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = b;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = c;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = d;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     expected = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = a;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = b;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = c;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = d;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:                     local_vsum(
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                         __SlateVaArgs::new(vec![
// LOWERING-X86_64-GNU-NEXT:                             __SlateVaArg::new({{_v[0-9]+}}),
// LOWERING-X86_64-GNU-NEXT:                             __SlateVaArg::new({{_v[0-9]+}}),
// LOWERING-X86_64-GNU-NEXT:                             __SlateVaArg::new({{_v[0-9]+}}),
// LOWERING-X86_64-GNU-NEXT:                             __SlateVaArg::new({{_v[0-9]+}}),
// LOWERING-X86_64-GNU-NEXT:                         ]),
// LOWERING-X86_64-GNU-NEXT:                     )
// LOWERING-X86_64-GNU-NEXT:                 };
// LOWERING-X86_64-GNU-NEXT:                 check_got_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = expected;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 592;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"local_vsum(4, a, b, c, d)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"expected\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = a;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = b;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = c;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = d;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:                     __slate_ext_vsum__rf80_i32_f80_f80_f80_f80({{_v[0-9]+}} as i32, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}})
// LOWERING-X86_64-GNU-NEXT:                 };
// LOWERING-X86_64-GNU-NEXT:                 check_got_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = expected;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 595;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_vsum(4, a, b, c, d)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"expected\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 128;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%.21La\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = expected;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:         __slate_snprintf__ri32_pi8_u64_pi8_f80({{_v[0-9]+}} as *mut i8, {{_v[0-9]+}} as u64, {{_v[0-9]+}} as *mut i8, {{_v[0-9]+}})
// LOWERING-X86_64-GNU-NEXT:     };
// LOWERING-X86_64-GNU-NEXT:     n = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = n;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = n;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: u64 = 128;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 599;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                             b"n > 0 && (size_t)n < sizeof(buf)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:     end = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:                     __slate_strtold__rf80_pc_ppc(
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                         std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                     )
// LOWERING-X86_64-GNU-NEXT:                 };
// LOWERING-X86_64-GNU-NEXT:                 check_got_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = expected;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 601;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"strtold(buf, &end)\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"expected\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: *mut i8 = end;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: *mut i8 = std::ptr::null_mut();
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = end;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i8 = unsafe { *{{_v[0-9]+}} };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = false;
// LOWERING-X86_64-GNU-NEXT:                         {{_v[0-9]+}}
// LOWERING-X86_64-GNU-NEXT:                     };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 602;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"end != NULL && *end == '\\0'\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             fprintf(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn test_extern_global() {
// LOWERING-X86_64-GNU-NEXT:     let mut old: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut a: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut b: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { ext_global_ld };
// LOWERING-X86_64-GNU-NEXT:     old = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([196, 179, 162, 145, 128, 247, 230, 213, 26, 192]);
// LOWERING-X86_64-GNU-NEXT:     a = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([132, 3, 131, 2, 130, 1, 129, 128, 224, 63]);
// LOWERING-X86_64-GNU-NEXT:     b = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = a;
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         ext_global_ld = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_global_get__rf80() };
// LOWERING-X86_64-GNU-NEXT:                 check_got_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = a;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 611;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_global_get()\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"a\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = b;
// LOWERING-X86_64-GNU-NEXT:     unsafe { __slate_ext_global_set__rv_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { ext_global_ld };
// LOWERING-X86_64-GNU-NEXT:                 check_got_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = b;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 614;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_global_ld\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"b\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_2;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = old;
// LOWERING-X86_64-GNU-NEXT:     unsafe { __slate_ext_global_set__rv_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut check_got_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let mut check_expected_3: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = unsafe { ext_global_ld };
// LOWERING-X86_64-GNU-NEXT:                 check_got_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = old;
// LOWERING-X86_64-GNU-NEXT:                 check_expected_3 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 unsafe {
// LOWERING-X86_64-GNU-NEXT:                     checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                         unsafe {
// LOWERING-X86_64-GNU-NEXT:                             failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0"
// LOWERING-X86_64-GNU-NEXT:                             .as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 617;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"ext_global_ld\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: *mut i8 = b"old\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                             __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                             )
// LOWERING-X86_64-GNU-NEXT:                         };
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn test_control_flow_and_spills() {
// LOWERING-X86_64-GNU-NEXT:     let mut seed: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut x: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut i: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([248, 230, 213, 196, 179, 162, 145, 128, 3, 64]);
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(seed), {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(seed)) };
// LOWERING-X86_64-GNU-NEXT:     x = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:         i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 200;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             {
// LOWERING-X86_64-GNU-NEXT:                 let mut t: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 17;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 8;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 243, 63]);
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 t = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = x;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = t;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         x = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     } else {
// LOWERING-X86_64-GNU-NEXT:                         {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 3;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: LongDouble = x;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: LongDouble = t;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                 x = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             } else {
// LOWERING-X86_64-GNU-NEXT:                                 {
// LOWERING-X86_64-GNU-NEXT:                                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                                     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-X86_64-GNU-NEXT:                                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-X86_64-GNU-NEXT:                                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: LongDouble = x;
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                                             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                                             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 235, 63]);
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                         x = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                     } else {
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: LongDouble = x;
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                                             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                                             LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 235, 63]);
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                         x = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                     }
// LOWERING-X86_64-GNU-NEXT:                                 }
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 19;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = x;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = local_identity({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                         x = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 31;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = x;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_identity__rf80_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                         x = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:             i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let mut y: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(seed)) };
// LOWERING-X86_64-GNU-NEXT:         y = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:             i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 200;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut t2: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 17;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 8;
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} - {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32({{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 243, 63]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_mul__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                     t2 = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = y;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = t2;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                                 unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                             y = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         } else {
// LOWERING-X86_64-GNU-NEXT:                             {
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = 3;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-NEXT:                                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                 if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                                     let {{_v[0-9]+}}: LongDouble = y;
// LOWERING-X86_64-GNU-NEXT:                                     let {{_v[0-9]+}}: LongDouble = t2;
// LOWERING-X86_64-GNU-NEXT:                                     let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                                         unsafe { __slate_ext_sub__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                                     y = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                 } else {
// LOWERING-X86_64-GNU-NEXT:                                     {
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} & {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: i32 = 2;
// LOWERING-X86_64-GNU-NEXT:                                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                                             let {{_v[0-9]+}}: LongDouble = y;
// LOWERING-X86_64-GNU-NEXT:                                             let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                                             let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 235, 63]);
// LOWERING-X86_64-GNU-NEXT:                                             let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                                 __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}})
// LOWERING-X86_64-GNU-NEXT:                                             };
// LOWERING-X86_64-GNU-NEXT:                                             let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                                 __slate_ext_mul__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}})
// LOWERING-X86_64-GNU-NEXT:                                             };
// LOWERING-X86_64-GNU-NEXT:                                             y = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                         } else {
// LOWERING-X86_64-GNU-NEXT:                                             let {{_v[0-9]+}}: LongDouble = y;
// LOWERING-X86_64-GNU-NEXT:                                             let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// LOWERING-X86_64-GNU-NEXT:                                             let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                                                 LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 235, 63]);
// LOWERING-X86_64-GNU-NEXT:                                             let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                                 __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}})
// LOWERING-X86_64-GNU-NEXT:                                             };
// LOWERING-X86_64-GNU-NEXT:                                             let {{_v[0-9]+}}: LongDouble = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                                 __slate_ext_div__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}})
// LOWERING-X86_64-GNU-NEXT:                                             };
// LOWERING-X86_64-GNU-NEXT:                                             y = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                                         }
// LOWERING-X86_64-GNU-NEXT:                                     }
// LOWERING-X86_64-GNU-NEXT:                                 }
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 19;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = y;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                                 unsafe { __slate_ext_identity__rf80_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                             y = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 31;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = y;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble =
// LOWERING-X86_64-GNU-NEXT:                                 unsafe { __slate_ext_identity__rf80_f80({{_v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:                             y = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                 i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:         {
// LOWERING-X86_64-GNU-NEXT:             loop {
// LOWERING-X86_64-GNU-NEXT:                 {
// LOWERING-X86_64-GNU-NEXT:                     let mut check_got_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let mut check_expected_: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = x;
// LOWERING-X86_64-GNU-NEXT:                     check_got_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = y;
// LOWERING-X86_64-GNU-NEXT:                     check_expected_ = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { checks };
// LOWERING-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                     unsafe {
// LOWERING-X86_64-GNU-NEXT:                         checks = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                     {
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe { failures };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:                             unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 failures = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:                             }
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut libc::FILE = unsafe { stderr };
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 =
// LOWERING-X86_64-GNU-NEXT:                                 b"FAIL line %d: %s != %s (got=%La expected=%La)\n\0".as_ptr()
// LOWERING-X86_64-GNU-NEXT:                                     as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = 664;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"x\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: *mut i8 = b"y\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_got_;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: LongDouble = check_expected_;
// LOWERING-X86_64-GNU-NEXT:                             let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-X86_64-GNU-NEXT:                                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as i32,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}} as *mut i8,
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                     {{_v[0-9]+}},
// LOWERING-X86_64-GNU-NEXT:                                 )
// LOWERING-X86_64-GNU-NEXT:                             };
// LOWERING-X86_64-GNU-NEXT:                         }
// LOWERING-X86_64-GNU-NEXT:                     }
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-X86_64-GNU-NEXT:                 if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                     break;
// LOWERING-X86_64-GNU-NEXT:                 }
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     return;
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn ld_kind() -> *mut i8 {
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"x87-extended\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[unsafe(no_mangle)]
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: extern "C-unwind" fn local_cb({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let mut x: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut y: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = -{{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_fma({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 238, 63]);
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: unsafe fn local_vsum({{arg[0-9]+}}: i32, mut __slate_va_args: __SlateVaArgs) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let mut n: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// LOWERING-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     let mut i: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     n = {{arg[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// LOWERING-X86_64-GNU-NEXT:     r = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     unsafe {
// LOWERING-X86_64-GNU-NEXT:         ap = __slate_va_args.clone();
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     {
// LOWERING-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:         i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         loop {
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = n;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-X86_64-GNU-NEXT:                 break;
// LOWERING-X86_64-GNU-NEXT:             }
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: LongDouble = unsafe { ap.next_arg::<LongDouble>() };
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: LongDouble = r;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             r = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-X86_64-GNU-NEXT:             i = {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:         }
// LOWERING-X86_64-GNU-NEXT:     }
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = r;
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: #[inline(never)]
// LOWERING-X86_64-GNU-NEXT: fn local_identity({{arg[0-9]+}}: LongDouble) -> LongDouble {
// LOWERING-X86_64-GNU-NEXT:     let mut y: LongDouble = LongDouble([0; 10]);
// LOWERING-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), {{arg[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// LOWERING-X86_64-GNU-NEXT:     return {{_v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT: }
// LOWERING-X86_64-GNU-EMPTY:
// LOWERING-X86_64-GNU-NEXT: unsafe extern "C" {
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// LOWERING-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// LOWERING-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// LOWERING-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_add__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_array_sum__rf80_pf80_usize(_0: *const LongDouble, _1: usize) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_box_roundtrip__rx_x(_0: ld_box) -> ld_box;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_call_cb__rf80_x_f80_f80(
// LOWERING-X86_64-GNU-NEXT:         _0: Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// LOWERING-X86_64-GNU-NEXT:         _1: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _2: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     ) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_div__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_eq__ri32_f80_f80(_0: LongDouble, _1: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_from_double__rf80_f64(_0: f64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_from_float__rf80_f32(_0: f32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_from_i64__rf80_i64(_0: i64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_from_u64__rf80_u64(_0: u64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_global_get__rf80() -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_global_set__rv_f80(_0: LongDouble);
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_identity__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_le__ri32_f80_f80(_0: LongDouble, _1: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_load__rf80_pf80(_0: *const LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_lt__ri32_f80_f80(_0: LongDouble, _1: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_mix_abi__rf80_i64_f64_f80_u32_f80_f32_f80_i32_f64_f80(
// LOWERING-X86_64-GNU-NEXT:         _0: i64,
// LOWERING-X86_64-GNU-NEXT:         _1: f64,
// LOWERING-X86_64-GNU-NEXT:         _2: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _3: u32,
// LOWERING-X86_64-GNU-NEXT:         _4: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _5: f32,
// LOWERING-X86_64-GNU-NEXT:         _6: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _7: i32,
// LOWERING-X86_64-GNU-NEXT:         _8: f64,
// LOWERING-X86_64-GNU-NEXT:         _9: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     ) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_mul__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_neg__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_nested_roundtrip__rx_x(_0: ld_nested) -> ld_nested;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_pair_make__rcf80_f80_f80(_0: LongDouble, _1: LongDouble) -> ld_pair;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_store__rv_pf80_f80(_0: *mut LongDouble, _1: LongDouble);
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_sub__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_sum10__rf80_f80_f80_f80_f80_f80_f80_f80_f80_f80_f80(
// LOWERING-X86_64-GNU-NEXT:         _0: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _1: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _2: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _3: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _4: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _5: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _6: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _7: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _8: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _9: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     ) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_to_double__rf64_f80(_0: LongDouble) -> f64;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_to_float__rf32_f80(_0: LongDouble) -> f32;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_to_i64__ri64_f80(_0: LongDouble) -> i64;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_to_u64__ru64_f80(_0: LongDouble) -> u64;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ext_vsum__rf80_i32_f80_f80_f80_f80(
// LOWERING-X86_64-GNU-NEXT:         _0: i32,
// LOWERING-X86_64-GNU-NEXT:         _1: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _2: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _3: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _4: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     ) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_fmodl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// LOWERING-X86_64-GNU-NEXT:         _0: *mut libc::FILE,
// LOWERING-X86_64-GNU-NEXT:         _1: *mut i8,
// LOWERING-X86_64-GNU-NEXT:         _2: i32,
// LOWERING-X86_64-GNU-NEXT:         _3: *mut i8,
// LOWERING-X86_64-GNU-NEXT:         _4: *mut i8,
// LOWERING-X86_64-GNU-NEXT:         _5: LongDouble,
// LOWERING-X86_64-GNU-NEXT:         _6: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     ) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_frexpl__rf80_f80_pi32(_0: LongDouble, _1: *mut i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     safe fn __slate_ld_local_cb(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_ldexpl__rf80_f80_i32(_0: LongDouble, _1: i32) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_nanl__rf80_pc(_0: *const core::ffi::c_char) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_nextafterl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_powl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_remainderl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_snprintf__ri32_pi8_u64_pi8_f80(
// LOWERING-X86_64-GNU-NEXT:         _0: *mut i8,
// LOWERING-X86_64-GNU-NEXT:         _1: u64,
// LOWERING-X86_64-GNU-NEXT:         _2: *mut i8,
// LOWERING-X86_64-GNU-NEXT:         _3: LongDouble,
// LOWERING-X86_64-GNU-NEXT:     ) -> i32;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_sqrtl__rf80_f80(_0: LongDouble) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT:     fn __slate_strtold__rf80_pc_ppc(
// LOWERING-X86_64-GNU-NEXT:         _0: *const core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:         _1: *mut *mut core::ffi::c_char,
// LOWERING-X86_64-GNU-NEXT:     ) -> LongDouble;
// LOWERING-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN rewrites-x86_64-gnu
// REWRITES-X86_64-GNU: #![feature(c_variadic)]
// REWRITES-X86_64-GNU-NEXT: #![allow(
// REWRITES-X86_64-GNU-NEXT:     dead_code,
// REWRITES-X86_64-GNU-NEXT:     unused,
// REWRITES-X86_64-GNU-NEXT:     non_camel_case_types,
// REWRITES-X86_64-GNU-NEXT:     non_snake_case,
// REWRITES-X86_64-GNU-NEXT:     non_upper_case_globals,
// REWRITES-X86_64-GNU-NEXT:     arithmetic_overflow,
// REWRITES-X86_64-GNU-NEXT:     unconditional_panic,
// REWRITES-X86_64-GNU-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-X86_64-GNU-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-X86_64-GNU-NEXT:     unused_comparisons
// REWRITES-X86_64-GNU-NEXT: )]
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C, align(16))]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct LongDouble([u8; 10]);
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Add for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn add(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_add(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Sub for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn sub(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_sub(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Mul for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn mul(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_mul(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Div for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn div(self, __o: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_div(self, __o)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::AddAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn add_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_add(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::SubAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn sub_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_sub(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::MulAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn mul_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_mul(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::DivAssign for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn div_assign(&mut self, __o: LongDouble) {
// REWRITES-X86_64-GNU-NEXT:         {
// REWRITES-X86_64-GNU-NEXT:             *self = __slate_f80_div(*self, __o);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::ops::Neg for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     type Output = LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn neg(self) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_neg(self)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialEq for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn eq(&self, __other: &LongDouble) -> bool {
// REWRITES-X86_64-GNU-NEXT:         __slate_f80_eq(*self, *__other)
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl core::cmp::PartialOrd for LongDouble {
// REWRITES-X86_64-GNU-NEXT:     fn partial_cmp(&self, __other: &LongDouble) -> Option<std::cmp::Ordering> {
// REWRITES-X86_64-GNU-NEXT:         if __slate_f80_lt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:             Some(std::cmp::Ordering::Less)
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             if __slate_f80_gt(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                 Some(std::cmp::Ordering::Greater)
// REWRITES-X86_64-GNU-NEXT:             } else {
// REWRITES-X86_64-GNU-NEXT:                 if __slate_f80_eq(*self, *__other) {
// REWRITES-X86_64-GNU-NEXT:                     Some(std::cmp::Ordering::Equal)
// REWRITES-X86_64-GNU-NEXT:                 } else {
// REWRITES-X86_64-GNU-NEXT:                     None
// REWRITES-X86_64-GNU-NEXT:                 }
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C)]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct ld_box {
// REWRITES-X86_64-GNU-NEXT:     tag: u8,
// REWRITES-X86_64-GNU-NEXT:     x: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     tail: u32,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C)]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct ld_nested {
// REWRITES-X86_64-GNU-NEXT:     head: u16,
// REWRITES-X86_64-GNU-NEXT:     pair: ld_pair,
// REWRITES-X86_64-GNU-NEXT:     bytes: [u8; 3],
// REWRITES-X86_64-GNU-NEXT:     z: LongDouble,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C)]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: struct ld_pair {
// REWRITES-X86_64-GNU-NEXT:     a: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     b: LongDouble,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[repr(C)]
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone, Copy)]
// REWRITES-X86_64-GNU-NEXT: union ld_union {
// REWRITES-X86_64-GNU-NEXT:     ld: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     bytes: [u8; 16],
// REWRITES-X86_64-GNU-NEXT:     u64: u64,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: static mut checks: i32 = 0;
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: static mut failures: i32 = 0;
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     static mut ext_global_ld: LongDouble;
// REWRITES-X86_64-GNU-NEXT:     static mut stderr: *mut libc::FILE;
// REWRITES-X86_64-GNU-NEXT:     fn fprintf(_0: *mut libc::FILE, _1: *const core::ffi::c_char, ...) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn ext_sizeof_ld() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn ext_alignof_ld() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn ext_sizeof_box() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn ext_alignof_box() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn ext_offset_box_x() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn ext_offset_box_tail() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn ext_sizeof_pair() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn ext_alignof_pair() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn ext_sizeof_nested() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn ext_alignof_nested() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn ext_offset_nested_pair() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn ext_offset_nested_z() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn ext_sizeof_union() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn ext_alignof_union() -> usize;
// REWRITES-X86_64-GNU-NEXT:     fn nextafterl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn strtold(_0: *const core::ffi::c_char, _1: *mut *mut core::ffi::c_char) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_add(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_sub(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_mul(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_div(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_neg(_0: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_from_i64(_0: i64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_from_u64(_0: u64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_to_i64(_0: LongDouble) -> i64;
// REWRITES-X86_64-GNU-NEXT:     fn ext_to_u64(_0: LongDouble) -> u64;
// REWRITES-X86_64-GNU-NEXT:     fn ext_from_double(_0: f64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_from_float(_0: f32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_to_double(_0: LongDouble) -> f64;
// REWRITES-X86_64-GNU-NEXT:     fn ext_to_float(_0: LongDouble) -> f32;
// REWRITES-X86_64-GNU-NEXT:     fn nanl(_0: *const core::ffi::c_char) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_identity(_0: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_eq(_0: LongDouble, _1: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn ext_lt(_0: LongDouble, _1: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn ext_le(_0: LongDouble, _1: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn __errno_location() -> *mut i32;
// REWRITES-X86_64-GNU-NEXT:     fn strcmp(_0: *const core::ffi::c_char, _1: *const core::ffi::c_char) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn snprintf(_0: *mut core::ffi::c_char, _1: usize, _2: *const core::ffi::c_char, ...) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn frexpl(_0: LongDouble, _1: *mut i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ldexpl(_0: LongDouble, _1: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn sqrtl(_0: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn powl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn fmodl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn remainderl(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_sum10(
// REWRITES-X86_64-GNU-NEXT:         _0: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _1: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _2: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _3: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _4: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _5: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _6: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _7: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _8: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _9: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     ) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_mix_abi(
// REWRITES-X86_64-GNU-NEXT:         _0: i64,
// REWRITES-X86_64-GNU-NEXT:         _1: f64,
// REWRITES-X86_64-GNU-NEXT:         _2: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _3: u32,
// REWRITES-X86_64-GNU-NEXT:         _4: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _5: f32,
// REWRITES-X86_64-GNU-NEXT:         _6: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _7: i32,
// REWRITES-X86_64-GNU-NEXT:         _8: f64,
// REWRITES-X86_64-GNU-NEXT:         _9: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     ) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_call_cb(
// REWRITES-X86_64-GNU-NEXT:         _0: Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         _1: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _2: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     ) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_box_roundtrip(_0: ld_box) -> ld_box;
// REWRITES-X86_64-GNU-NEXT:     fn ext_pair_make(_0: LongDouble, _1: LongDouble) -> ld_pair;
// REWRITES-X86_64-GNU-NEXT:     fn memset(_0: *mut core::ffi::c_void, _1: i32, _2: usize) -> *mut core::ffi::c_void;
// REWRITES-X86_64-GNU-NEXT:     fn ext_nested_roundtrip(_0: ld_nested) -> ld_nested;
// REWRITES-X86_64-GNU-NEXT:     fn ext_array_sum(_0: *const LongDouble, _1: usize) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_store(_0: *mut LongDouble, _1: LongDouble);
// REWRITES-X86_64-GNU-NEXT:     fn ext_load(_0: *const LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn memcpy(
// REWRITES-X86_64-GNU-NEXT:         _0: *mut core::ffi::c_void,
// REWRITES-X86_64-GNU-NEXT:         _1: *const core::ffi::c_void,
// REWRITES-X86_64-GNU-NEXT:         _2: usize,
// REWRITES-X86_64-GNU-NEXT:     ) -> *mut core::ffi::c_void;
// REWRITES-X86_64-GNU-NEXT:     fn ext_vsum(_0: i32, ...) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_global_get() -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn ext_global_set(_0: LongDouble);
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: struct __SlateVaArg {
// REWRITES-X86_64-GNU-NEXT:     value: Box<dyn std::any::Any>,
// REWRITES-X86_64-GNU-NEXT:     size: usize,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl __SlateVaArg {
// REWRITES-X86_64-GNU-NEXT:     fn new<T: 'static>(value: T) -> Self {
// REWRITES-X86_64-GNU-NEXT:         Self {
// REWRITES-X86_64-GNU-NEXT:             value: Box::new(value),
// REWRITES-X86_64-GNU-NEXT:             size: std::mem::size_of::<T>(),
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     fn read<T: Copy + 'static>(&self) -> T {
// REWRITES-X86_64-GNU-NEXT:         if let Some(value) = self.value.downcast_ref::<T>() {
// REWRITES-X86_64-GNU-NEXT:             return *value;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         assert!(self.size >= std::mem::size_of::<T>());
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             std::ptr::read_unaligned(
// REWRITES-X86_64-GNU-NEXT:                 (self.value.as_ref() as *const dyn std::any::Any) as *const () as *const T,
// REWRITES-X86_64-GNU-NEXT:             )
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[derive(Clone)]
// REWRITES-X86_64-GNU-NEXT: struct __SlateVaArgs {
// REWRITES-X86_64-GNU-NEXT:     args: Option<std::rc::Rc<Vec<__SlateVaArg>>>,
// REWRITES-X86_64-GNU-NEXT:     index: usize,
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: impl __SlateVaArgs {
// REWRITES-X86_64-GNU-NEXT:     fn new(args: Vec<__SlateVaArg>) -> Self {
// REWRITES-X86_64-GNU-NEXT:         Self {
// REWRITES-X86_64-GNU-NEXT:             args: Some(std::rc::Rc::new(args)),
// REWRITES-X86_64-GNU-NEXT:             index: 0,
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     const fn empty() -> Self {
// REWRITES-X86_64-GNU-NEXT:         Self {
// REWRITES-X86_64-GNU-NEXT:             args: None,
// REWRITES-X86_64-GNU-NEXT:             index: 0,
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT:     fn next_arg<T: Copy + 'static>(&mut self) -> T {
// REWRITES-X86_64-GNU-NEXT:         let index = self.index;
// REWRITES-X86_64-GNU-NEXT:         self.index += 1;
// REWRITES-X86_64-GNU-NEXT:         if std::mem::size_of::<T>() == 0 {
// REWRITES-X86_64-GNU-NEXT:             return unsafe { std::mem::zeroed() };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let args = self.args.as_ref().expect("va_arg with no arguments");
// REWRITES-X86_64-GNU-NEXT:         args[index].read::<T>()
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     let mut __retval: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:     test_format_and_layout();
// REWRITES-X86_64-GNU-NEXT:     test_literals_and_precision_boundaries();
// REWRITES-X86_64-GNU-NEXT:     test_arithmetic_and_temporaries();
// REWRITES-X86_64-GNU-NEXT:     test_casts_and_usual_conversions();
// REWRITES-X86_64-GNU-NEXT:     test_special_values_and_comparisons();
// REWRITES-X86_64-GNU-NEXT:     test_libc_and_libm_externs();
// REWRITES-X86_64-GNU-NEXT:     test_extern_function_abi();
// REWRITES-X86_64-GNU-NEXT:     test_struct_union_array_and_pointer_abi();
// REWRITES-X86_64-GNU-NEXT:     test_varargs();
// REWRITES-X86_64-GNU-NEXT:     test_extern_global();
// REWRITES-X86_64-GNU-NEXT:     test_control_flow_and_spills();
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: bool = (unsafe { failures }) != 0;
// REWRITES-X86_64-GNU-NEXT:     if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             fprintf(
// REWRITES-X86_64-GNU-NEXT:                 (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                 c"long-double torture: %d/%d checks FAILED\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                 unsafe { failures },
// REWRITES-X86_64-GNU-NEXT:                 unsafe { checks },
// REWRITES-X86_64-GNU-NEXT:             )
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         __retval = 1;
// REWRITES-X86_64-GNU-NEXT:         std::process::exit(__retval as i32);
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         printf(
// REWRITES-X86_64-GNU-NEXT:             c"long-double torture: PASS (%d checks)\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:             unsafe { checks },
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     __retval = 0;
// REWRITES-X86_64-GNU-NEXT:     std::process::exit(__retval as i32);
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn test_format_and_layout() {
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         printf(
// REWRITES-X86_64-GNU-NEXT:             c"long double: kind=%s sizeof=%zu align=%zu mant=%d max_exp=%d\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:             ld_kind(),
// REWRITES-X86_64-GNU-NEXT:             16 as u64,
// REWRITES-X86_64-GNU-NEXT:             16 as u64,
// REWRITES-X86_64-GNU-NEXT:             64 as i32,
// REWRITES-X86_64-GNU-NEXT:             16384 as i32,
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 53;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if 64 == {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 1024;
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = 16384 == {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = true;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 64;
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = if 64 == {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 16384;
// REWRITES-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = 16384 == {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:                 {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:             } else {
// REWRITES-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:                 {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = true;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 113;
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = if 64 == {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 16384;
// REWRITES-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = 16384 == {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:                 {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:             } else {
// REWRITES-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:                 {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf((unsafe { stderr }) as *mut libc::FILE, c"FAIL line %d: %s\n".as_ptr(), 192 as i32, c"(LDBL_MANT_DIG == 53 && LDBL_MAX_EXP == 1024) || (LDBL_MANT_DIG == 64 && LDBL_MAX_EXP == 16384) || (LDBL_MANT_DIG == 113 && LDBL_MAX_EXP == 16384)".as_ptr() as *mut i8)
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_sizeof_ld() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     194 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"sizeof(long double) == ext_sizeof_ld()".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_alignof_ld() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     195 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(size_t)ALIGNOF(long double) == ext_alignof_ld()".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_box>() as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_sizeof_box() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     197 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"sizeof(struct ld_box) == ext_sizeof_box()".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_box>() as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_alignof_box() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     198 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(size_t)ALIGNOF(struct ld_box) == ext_alignof_box()".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_offset_box_x() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     199 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"offsetof(struct ld_box, x) == ext_offset_box_x()".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_pair>() as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_offset_box_tail() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     200 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"offsetof(struct ld_box, tail) == ext_offset_box_tail()".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 32;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_sizeof_pair() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     202 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"sizeof(struct ld_pair) == ext_sizeof_pair()".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_pair>() as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_alignof_pair() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     203 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(size_t)ALIGNOF(struct ld_pair) == ext_alignof_pair()".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_nested>() as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_sizeof_nested() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     205 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"sizeof(struct ld_nested) == ext_sizeof_nested()".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_nested>() as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_alignof_nested() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     206 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(size_t)ALIGNOF(struct ld_nested) == ext_alignof_nested()".as_ptr()
// REWRITES-X86_64-GNU-NEXT:                         as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = std::mem::size_of::<ld_union>() as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_offset_nested_pair() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     207 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"offsetof(struct ld_nested, pair) == ext_offset_nested_pair()".as_ptr()
// REWRITES-X86_64-GNU-NEXT:                         as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_offset_nested_z() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     208 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"offsetof(struct ld_nested, z) == ext_offset_nested_z()".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = std::mem::align_of::<ld_union>() as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_sizeof_union() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     210 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"sizeof(union ld_union) == ext_sizeof_union()".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = (unsafe { ext_alignof_union() }) as u64;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     211 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(size_t)ALIGNOF(union ld_union) == ext_alignof_union()".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: /// 2^53 + 1
// REWRITES-X86_64-GNU-NEXT: fn test_literals_and_precision_boundaries() {
// REWRITES-X86_64-GNU-NEXT:     let mut one: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut eps: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 192, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut n1: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut n2: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut via_parse_64: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut via_parse_113: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut end: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 9007199254740993u64;
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(one + eps != one);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     224 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"one + eps != one".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_: LongDouble = one + eps - one;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_: LongDouble = eps;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_ == check_expected_);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     225 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(one + eps) - one".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"eps".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_2: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_2: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { __slate_nextafterl__rf80_f80_f80(one, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         check_got_2 = {{_v[0-9]+}} - one;
// REWRITES-X86_64-GNU-NEXT:         check_expected_2 = eps;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_2 == check_expected_2);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     226 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"nextafterl(one, 2.0L) - one".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"eps".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_2,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_2,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     n1 = __slate_f80_from_u64({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 9007199254740992u64;
// REWRITES-X86_64-GNU-NEXT:     n2 = __slate_f80_from_u64({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(n1 != n2);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     233 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"n1 != n2".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     via_parse_64 = unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_strtold__rf80_pc_ppc(
// REWRITES-X86_64-GNU-NEXT:             c"0x1.0000000000000002p0".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if end != std::ptr::null_mut() {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = ((unsafe { *end }) as i32) == 0;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     238 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"end != NULL && *end == '\\0'".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(via_parse_64 != LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]));
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     240 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"via_parse_64 != 1.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_3: LongDouble = via_parse_64;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_3: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_3 == check_expected_3);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     241 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"via_parse_64".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"0x1.0000000000000002p0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_3,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_3,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     end = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     via_parse_113 = unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_strtold__rf80_pc_ppc(
// REWRITES-X86_64-GNU-NEXT:             c"0x1.0000000000000000000000000001p0".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if end != std::ptr::null_mut() {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = ((unsafe { *end }) as i32) == 0;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     248 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"end != NULL && *end == '\\0'".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(via_parse_113 == LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]));
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     253 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"via_parse_113 == 1.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let mut wide: LongDouble = LongDouble([1, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut through_double: LongDouble = __slate_f80_from_f64(1.0 as f64);
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(wide != through_double);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     261 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"wide != through_double".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let mut huge_but_finite: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 207, 71]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(huge_but_finite), {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble =
// REWRITES-X86_64-GNU-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(huge_but_finite)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class({{_v[0-9]+}}, 504);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     268 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"isfinite(huge_but_finite)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble =
// REWRITES-X86_64-GNU-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(huge_but_finite)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64(179769313486231570814527423731704356798070567525844996598917476803157260780028538760589558632766878171540458953514382464234321326889464182768467546703537516986049910576551282076245490090389328944075868508455133942304583236903222948165808559332123348274797826204144723168738177180919299881250404026184124858368.0 as f64);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} > {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     269 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"huge_but_finite > (long double)DBL_MAX".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: /// Conditional operator and comma operator force long-double temporaries.
// REWRITES-X86_64-GNU-NEXT: /// Conditional operator and comma operator force long-double temporaries.
// REWRITES-X86_64-GNU-NEXT: fn test_arithmetic_and_temporaries() {
// REWRITES-X86_64-GNU-NEXT:     let mut a: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut b: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut c: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut x: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut y: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([128, 247, 230, 213, 196, 179, 162, 145, 19, 64]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 42, 59, 76, 93, 110, 127, 128, 248, 191]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(b), {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([255, 255, 255, 255, 255, 255, 255, 255, 2, 64]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(c), {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-X86_64-GNU-NEXT:         check_got_ = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_ = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_ == check_expected_);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     281 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"a + b".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_add(a, b)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_2: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_2: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-X86_64-GNU-NEXT:         check_got_2 = {{_v[0-9]+}} - {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_2 = unsafe { __slate_ext_sub__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_2 == check_expected_2);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     282 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"a - b".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_sub(a, b)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_2,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_2,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_3: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_3: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-X86_64-GNU-NEXT:         check_got_3 = {{_v[0-9]+}} * {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_3 = unsafe { __slate_ext_mul__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_3 == check_expected_3);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     283 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"a * b".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_mul(a, b)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_3,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_3,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_4: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_4: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-X86_64-GNU-NEXT:         check_got_4 = {{_v[0-9]+}} / {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_4 = unsafe { __slate_ext_div__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_4 == check_expected_4);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     284 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"a / c".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_div(a, c)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_4,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_4,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_5: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_5: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:         check_got_5 = -{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_5 = unsafe { __slate_ext_neg__rf80_f80({{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_5 == check_expected_5);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     285 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"-a".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_neg(a)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_5,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_5,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = {{_v[0-9]+}} * {{_v[0-9]+}} / LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 128, 128, 244, 63]);
// REWRITES-X86_64-GNU-NEXT:     x = __slate_f80_fma(
// REWRITES-X86_64-GNU-NEXT:         -(({{_v[0-9]+}} - {{_v[0-9]+}}) / LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 252, 63])),
// REWRITES-X86_64-GNU-NEXT:         {{_v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:         {{_v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:     );
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_mul__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 1, 64]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_div__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_sub__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 252, 63]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_div__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 128, 128, 244, 63]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_mul__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     y = unsafe { __slate_ext_sub__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_6: LongDouble = x;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_6: LongDouble = y;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_6 == check_expected_6);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     291 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"x".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"y".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_6,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_6,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let mut q: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}} * {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 253, 63]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}} - {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}} / {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_7: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_7: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 136, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_7 == check_expected_7);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     299 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"q".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"2.125L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_7,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_7,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// REWRITES-X86_64-GNU-NEXT:     todo!("long double constant without Clang AST value");
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}} + _200) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_8: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_8: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 200, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_8 == check_expected_8);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     301 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"q".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"3.125L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_8,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// REWRITES-X86_64-GNU-NEXT:     todo!("long double constant without Clang AST value");
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(q), {{_v[0-9]+}} - _203) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_9: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(q)) };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_9: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 136, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_9 == check_expected_9);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     303 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"q".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"2.125L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_9,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_9,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_10: LongDouble =
// REWRITES-X86_64-GNU-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_10: LongDouble =
// REWRITES-X86_64-GNU-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_10 == check_expected_10);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     307 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(1 ? a : (double)b)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"a".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_10,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_10,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_11: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_11: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         x = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-X86_64-GNU-NEXT:         check_got_11 = x + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_11 = {{_v[0-9]+}} + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_11 == check_expected_11);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     308 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(x = b, x + 1.0L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"b + 1.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_11,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_11,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn test_casts_and_usual_conversions() {
// REWRITES-X86_64-GNU-NEXT:     let mut si: aligned::Aligned<aligned::A16, [i64; 7]> = aligned::Aligned([0; 7]);
// REWRITES-X86_64-GNU-NEXT:     let mut ui: aligned::Aligned<aligned::A16, [u64; 6]> = aligned::Aligned([0; 6]);
// REWRITES-X86_64-GNU-NEXT:     let mut i: u64 = 0;
// REWRITES-X86_64-GNU-NEXT:     *si = [
// REWRITES-X86_64-GNU-NEXT:         0,
// REWRITES-X86_64-GNU-NEXT:         1,
// REWRITES-X86_64-GNU-NEXT:         -1,
// REWRITES-X86_64-GNU-NEXT:         2147483647,
// REWRITES-X86_64-GNU-NEXT:         -2147483647,
// REWRITES-X86_64-GNU-NEXT:         9007199254740991,
// REWRITES-X86_64-GNU-NEXT:         -9007199254740991,
// REWRITES-X86_64-GNU-NEXT:     ];
// REWRITES-X86_64-GNU-NEXT:     *ui = [
// REWRITES-X86_64-GNU-NEXT:         0,
// REWRITES-X86_64-GNU-NEXT:         1,
// REWRITES-X86_64-GNU-NEXT:         4294967295,
// REWRITES-X86_64-GNU-NEXT:         9007199254740991,
// REWRITES-X86_64-GNU-NEXT:         9007199254740993,
// REWRITES-X86_64-GNU-NEXT:         18446744073709551615,
// REWRITES-X86_64-GNU-NEXT:     ];
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 8;
// REWRITES-X86_64-GNU-NEXT:         if !(i < 56 / {{_v[0-9]+}}) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         loop {
// REWRITES-X86_64-GNU-NEXT:             let mut check_got_: LongDouble = __slate_f80_from_i64(si[(i as usize)]);
// REWRITES-X86_64-GNU-NEXT:             let mut check_expected_: LongDouble =
// REWRITES-X86_64-GNU-NEXT:                 unsafe { __slate_ext_from_i64__rf80_i64(si[(i as usize)] as i64) };
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = !(check_got_ == check_expected_);
// REWRITES-X86_64-GNU-NEXT:             if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:                 unsafe {
// REWRITES-X86_64-GNU-NEXT:                     failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:                 }
// REWRITES-X86_64-GNU-NEXT:                 unsafe {
// REWRITES-X86_64-GNU-NEXT:                     __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                         (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                         c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                         328 as i32,
// REWRITES-X86_64-GNU-NEXT:                         c"(long double)si[i]".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                         c"ext_from_i64(si[i])".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                         check_got_,
// REWRITES-X86_64-GNU-NEXT:                         check_expected_,
// REWRITES-X86_64-GNU-NEXT:                     )
// REWRITES-X86_64-GNU-NEXT:                 };
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:             if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:                 break;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         i += 1;
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     i = 0;
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 8;
// REWRITES-X86_64-GNU-NEXT:         if !(i < 48 / {{_v[0-9]+}}) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         loop {
// REWRITES-X86_64-GNU-NEXT:             let mut check_got_2: LongDouble = __slate_f80_from_u64(ui[(i as usize)]);
// REWRITES-X86_64-GNU-NEXT:             let mut check_expected_2: LongDouble =
// REWRITES-X86_64-GNU-NEXT:                 unsafe { __slate_ext_from_u64__rf80_u64(ui[(i as usize)] as u64) };
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = !(check_got_2 == check_expected_2);
// REWRITES-X86_64-GNU-NEXT:             if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:                 unsafe {
// REWRITES-X86_64-GNU-NEXT:                     failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:                 }
// REWRITES-X86_64-GNU-NEXT:                 unsafe {
// REWRITES-X86_64-GNU-NEXT:                     __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                         (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                         c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                         331 as i32,
// REWRITES-X86_64-GNU-NEXT:                         c"(long double)ui[i]".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                         c"ext_from_u64(ui[i])".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                         check_got_2,
// REWRITES-X86_64-GNU-NEXT:                         check_expected_2,
// REWRITES-X86_64-GNU-NEXT:                     )
// REWRITES-X86_64-GNU-NEXT:                 };
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:             if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:                 break;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         i += 1;
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 63, 180, 150, 19, 64]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i64 = __slate_f80_to_i64({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 63, 180, 150, 19, 64]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i64 = unsafe { __slate_ext_to_i64__ri64_f80({{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     333 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(int64_t)1234567.875L == ext_to_i64(1234567.875L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 63, 180, 150, 19, 192]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i64 = __slate_f80_to_i64({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 63, 180, 150, 19, 192]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i64 = unsafe { __slate_ext_to_i64__ri64_f80({{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     334 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(int64_t)-1234567.875L == ext_to_i64(-1234567.875L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 63, 180, 150, 19, 64]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = __slate_f80_to_u64({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 63, 180, 150, 19, 64]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = unsafe { __slate_ext_to_u64__ru64_f80({{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     335 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(uint64_t)1234567.875L == ext_to_u64(1234567.875L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 254, 191]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i64 = __slate_f80_to_i64({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == 0);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     336 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(int64_t)-0.875L == 0".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let mut d: f64 = 0.0;
// REWRITES-X86_64-GNU-NEXT:     let mut f: f32 = 0.0;
// REWRITES-X86_64-GNU-NEXT:     let mut ld1: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut ld2: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), 1048576.0000000002 as f64) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), 1024.00012 as f32) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// REWRITES-X86_64-GNU-NEXT:     ld1 = __slate_f80_from_f64({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-X86_64-GNU-NEXT:     ld2 = __slate_f80_from_f32({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_3: LongDouble = ld1;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_3: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_3 = unsafe { __slate_ext_from_double__rf80_f64({{_v[0-9]+}} as f64) };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_3 == check_expected_3);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     343 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ld1".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_from_double(d)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_3,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_3,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_4: LongDouble = ld2;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_4: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_4 = unsafe { __slate_ext_from_float__rf80_f32({{_v[0-9]+}} as f32) };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_4 == check_expected_4);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     344 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ld2".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_from_float(f)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_4,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_4,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f64 = __slate_f80_to_f64(ld1);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { __slate_ext_to_double__rf64_f80(ld1) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     345 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(double)ld1 == ext_to_double(ld1)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f32 = __slate_f80_to_f32(ld2);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f32 = unsafe { __slate_ext_to_float__rf32_f80(ld2) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     346 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"(float)ld2 == ext_to_float(ld2)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let mut i32: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:     let mut u64: u64 = 0;
// REWRITES-X86_64-GNU-NEXT:     let mut f2: f32 = 0.0;
// REWRITES-X86_64-GNU-NEXT:     let mut d2: f64 = 0.0;
// REWRITES-X86_64-GNU-NEXT:     let mut ld: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(i32), -17 as i32) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: u64 = 9007199254740993u64;
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(u64), {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f2), 0.25 as f32) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d2), -0.125 as f64) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(ld), {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_5: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_5: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i32)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         check_got_5 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i32)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         check_expected_5 = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_5 == check_expected_5);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     357 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ld + i32".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_add(ld, (long double)i32)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_5,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_5,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_6: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_6: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(u64)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u64({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         check_got_6 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(u64)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u64({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         check_expected_6 = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_6 == check_expected_6);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     358 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ld + u64".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_add(ld, (long double)u64)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_6,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_6,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_7: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_7: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f2)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         check_got_7 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f2)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         check_expected_7 = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_7 == check_expected_7);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     359 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ld + f".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_add(ld, (long double)f)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_7,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_7,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_8: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_8: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d2)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         check_got_8 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d2)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         check_expected_8 = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_8 == check_expected_8);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     360 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ld + d".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_add(ld, (long double)d)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_8,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn test_special_values_and_comparisons() {
// REWRITES-X86_64-GNU-NEXT:     let mut pz: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:     let mut nz: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 128]);
// REWRITES-X86_64-GNU-NEXT:     let mut inf: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut ninf: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut nan: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: f32 = f32::from_bits(0x7f800000);
// REWRITES-X86_64-GNU-NEXT:     inf = __slate_f80_from_f32({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: f32 = f32::from_bits(0xff800000);
// REWRITES-X86_64-GNU-NEXT:     ninf = __slate_f80_from_f32({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     nan = unsafe { __slate_nanl__rf80_pc(c"".as_ptr()) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(pz == nz);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     371 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"pz == nz".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = __slate_f80_signbit(pz);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     372 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"!signbit(pz)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = __slate_f80_signbit(nz);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     373 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"signbit(nz)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_: LongDouble = unsafe { __slate_ext_identity__rf80_f80(pz) };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_: LongDouble = pz;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = check_got_ == check_expected_;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = __slate_f80_signbit(check_got_);
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = __slate_f80_signbit(check_expected_);
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == ({{_v[0-9]+}} as i32);
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: signed-zero mismatch\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     374 as i32,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_2: LongDouble = unsafe { __slate_ext_identity__rf80_f80(nz) };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_2: LongDouble = nz;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = check_got_2 == check_expected_2;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = __slate_f80_signbit(check_got_2);
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = __slate_f80_signbit(check_expected_2);
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == ({{_v[0-9]+}} as i32);
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: signed-zero mismatch\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     375 as i32,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_3: LongDouble = unsafe { __slate_ext_neg__rf80_f80(pz) };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_3: LongDouble = nz;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = check_got_3 == check_expected_3;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = __slate_f80_signbit(check_got_3);
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = __slate_f80_signbit(check_expected_3);
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == ({{_v[0-9]+}} as i32);
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: signed-zero mismatch\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     376 as i32,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_4: LongDouble = unsafe { __slate_ext_neg__rf80_f80(nz) };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_4: LongDouble = pz;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = check_got_4 == check_expected_4;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = __slate_f80_signbit(check_got_4);
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = __slate_f80_signbit(check_expected_4);
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == ({{_v[0-9]+}} as i32);
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: signed-zero mismatch\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     377 as i32,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class(inf, 516);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = inf > LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     379 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"isinf(inf) && inf > 0.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class(ninf, 516);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = ninf < LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     380 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"isinf(ninf) && ninf < 0.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class(nan, 3);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     381 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"isnan(nan)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = nan == nan;
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     382 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"!(nan == nan)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = nan < LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     383 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"!(nan < 0.0L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = nan > LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     384 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"!(nan > 0.0L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = nan <= LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     385 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"!(nan <= 0.0L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = nan >= LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     386 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"!(nan >= 0.0L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(nan != nan);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     387 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"nan != nan".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { __slate_ext_eq__ri32_f80_f80(pz, nz) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} != 0);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     389 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_eq(pz, nz)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { __slate_ext_lt__ri32_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} != 0);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     390 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_lt(-1.0L, 1.0L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { __slate_ext_le__ri32_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} != 0);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     391 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_le(1.0L, 1.0L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { __slate_ext_eq__ri32_f80_f80(nan, nan) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     392 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"!ext_eq(nan, nan)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { __slate_ext_lt__ri32_f80_f80(nan, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     393 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"!ext_lt(nan, 0.0L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_copysign({{_v[0-9]+}}, nz);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     395 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"signbit(copysignl(1.0L, nz))".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_copysign({{_v[0-9]+}}, pz);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = __slate_f80_signbit({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     396 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"!signbit(copysignl(1.0L, pz))".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn test_libc_and_libm_externs() {
// REWRITES-X86_64-GNU-NEXT:     let mut end: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let mut x: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut ip: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut fp: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut fr: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut e: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i8; 256]> = aligned::Aligned([0; 256]);
// REWRITES-X86_64-GNU-NEXT:     let mut end2: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let mut n: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"  -0x1.23456789abcdef0123456789p+17tail".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { __errno_location() };
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     x = unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_strtold__rf80_pc_ppc(
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-X86_64-GNU-NEXT:             std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(end != std::ptr::null_mut());
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     413 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"end != NULL".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = unsafe { strcmp(end as *const core::ffi::c_char, c"tail".as_ptr()) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == 0);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     414 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"strcmp(end, \"tail\") == 0".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: *mut i32 = unsafe { __errno_location() };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !((unsafe { *{{_v[0-9]+}} }) == 0);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     415 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"errno == 0".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(x < LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]));
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     416 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"x < 0.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     n = unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_snprintf__ri32_pi8_u64_pi8_f80(
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}} as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             256 as u64,
// REWRITES-X86_64-GNU-NEXT:             c"%La".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             x,
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if n > 0 {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = (n as u64) < 256;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     420 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"n > 0 && (size_t)n < sizeof(buf)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         check_got_ = unsafe {
// REWRITES-X86_64-GNU-NEXT:             __slate_strtold__rf80_pc_ppc(
// REWRITES-X86_64-GNU-NEXT:                 {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-X86_64-GNU-NEXT:                 std::ptr::addr_of_mut!(end2) as *mut *mut core::ffi::c_char,
// REWRITES-X86_64-GNU-NEXT:             )
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         check_expected_ = x;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_ == check_expected_);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     421 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"strtold(buf, &end2)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"x".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if end2 != std::ptr::null_mut() {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = ((unsafe { *end2 }) as i32) == 0;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     422 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"end2 != NULL && *end2 == '\\0'".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 231, 192, 12, 192]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_fract({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     ip = __slate_f80_trunc({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:     fp = {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_2: LongDouble = ip;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_2: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 228, 192, 12, 192]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_2 == check_expected_2);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     425 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ip".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"-12345.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_2,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_2,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_3: LongDouble = fp;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_3: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 254, 191]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_3 == check_expected_3);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     426 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"fp".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"-0.75L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_3,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_3,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 9, 64]);
// REWRITES-X86_64-GNU-NEXT:     fr = unsafe { __slate_frexpl__rf80_f80_pi32({{_v[0-9]+}}, std::ptr::addr_of_mut!(e) as *mut i32) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_4: LongDouble = fr;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_4: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 254, 63]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_4 == check_expected_4);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     429 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"fr".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"0.75L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_4,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_4,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(e == 11);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     430 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"e == 11".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_5: LongDouble = unsafe { __slate_ldexpl__rf80_f80_i32(fr, e as i32) };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_5: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 9, 64]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_5 == check_expected_5);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     431 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ldexpl(fr, e)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"0x1.8p+10L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_5,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_5,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_6: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_6: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// REWRITES-X86_64-GNU-NEXT:         check_got_6 = unsafe { __slate_sqrtl__rf80_f80({{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_6 = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_6 == check_expected_6);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     433 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"sqrtl(4.0L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"2.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_6,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_6,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_7: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_7: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         check_got_7 = unsafe { __slate_powl__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_7 = LongDouble([0, 0, 0, 0, 0, 0, 0, 216, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_7 == check_expected_7);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     434 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"powl(1.5L, 3.0L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"3.375L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_7,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_7,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_8: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_8: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 64]);
// REWRITES-X86_64-GNU-NEXT:         check_got_8 = __slate_f80_fma({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         check_expected_8 = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_8 == check_expected_8);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     435 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"fmal(2.0L, 3.0L, 4.0L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"10.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_8,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_9: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_9: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 140, 3, 64]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         check_got_9 = unsafe { __slate_fmodl__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_9 = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_9 == check_expected_9);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     436 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"fmodl(17.5L, 3.0L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"2.5L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_9,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_9,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_10: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_10: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 176, 1, 64]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         check_got_10 = unsafe { __slate_remainderl__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_10 = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 191]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_10 == check_expected_10);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     437 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"remainderl(5.5L, 2.0L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"-0.5L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_10,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_10,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_11: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_11: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 255, 191]);
// REWRITES-X86_64-GNU-NEXT:         check_got_11 = __slate_f80_floor({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         check_expected_11 = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 192]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_11 == check_expected_11);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     438 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"floorl(-1.25L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"-2.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_11,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_11,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_12: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_12: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 255, 191]);
// REWRITES-X86_64-GNU-NEXT:         check_got_12 = __slate_f80_ceil({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         check_expected_12 = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_12 == check_expected_12);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     439 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ceill(-1.25L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"-1.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_12,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_12,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_13: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_13: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 255, 191]);
// REWRITES-X86_64-GNU-NEXT:         check_got_13 = __slate_f80_trunc({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         check_expected_13 = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 191]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_13 == check_expected_13);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     440 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"truncl(-1.75L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"-1.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_13,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_13,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let mut toward: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     toward = unsafe { __slate_nextafterl__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(toward > LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]));
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     444 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"toward > 1.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_14: LongDouble = toward - LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_14: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 192, 63]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_14 == check_expected_14);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     445 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"toward - 1.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"LDBL_EPSILON".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_14,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_14,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let mut tiny: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:     tiny = unsafe { __slate_nextafterl__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(tiny > LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]));
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     450 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"tiny > 0.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(tiny < LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 1, 0]));
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     451 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"tiny < LDBL_MIN".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = tiny;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class({{_v[0-9]+}}, 96);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = 2;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class({{_v[0-9]+}}, 3);
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:                 {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:             } else {
// REWRITES-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class({{_v[0-9]+}}, 516);
// REWRITES-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-X86_64-GNU-NEXT:                     {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:                 } else {
// REWRITES-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: bool = __slate_f80_is_fp_class({{_v[0-9]+}}, 264);
// REWRITES-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 4;
// REWRITES-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = 3;
// REWRITES-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: i32 = if {{_v[0-9]+}} { {{_v[0-9]+}} } else { {{_v[0-9]+}} };
// REWRITES-X86_64-GNU-NEXT:                     {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:                 };
// REWRITES-X86_64-GNU-NEXT:                 {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} == 3);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     452 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"fpclassify(tiny) == FP_SUBNORMAL".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn test_extern_function_abi() {
// REWRITES-X86_64-GNU-NEXT:     let mut a0: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut a1: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 254, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut a2: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 253, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut a3: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 252, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut a4: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 251, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut a5: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 250, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut a6: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 249, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut a7: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 248, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut a8: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 247, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut a9: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 246, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut expected: LongDouble = a0 + a1 + a2 + a3 + a4 + (a5 + a6 + a7 + a8 + a9);
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([120, 111, 94, 77, 60, 43, 26, 137, 39, 64]);
// REWRITES-X86_64-GNU-NEXT:         check_got_ = unsafe { __slate_ext_identity__rf80_f80({{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_ = LongDouble([120, 111, 94, 77, 60, 43, 26, 137, 39, 64]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_ == check_expected_);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     471 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_identity(0x1.123456789abcdef012345678p+40L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"0x1.123456789abcdef012345678p+40L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_2: LongDouble = unsafe {
// REWRITES-X86_64-GNU-NEXT:             __slate_ext_sum10__rf80_f80_f80_f80_f80_f80_f80_f80_f80_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                 a0, a1, a2, a3, a4, a5, a6, a7, a8, a9,
// REWRITES-X86_64-GNU-NEXT:             )
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_2: LongDouble = expected;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_2 == check_expected_2);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     472 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_sum10(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"expected".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_2,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_2,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let mut i0: i64 = -123456789;
// REWRITES-X86_64-GNU-NEXT:     let mut d0: f64 = 0.0;
// REWRITES-X86_64-GNU-NEXT:     let mut a: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut u0: u32 = 12345;
// REWRITES-X86_64-GNU-NEXT:     let mut b: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut f0: f32 = 0.0;
// REWRITES-X86_64-GNU-NEXT:     let mut c: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut i1: i32 = -77;
// REWRITES-X86_64-GNU-NEXT:     let mut d1: f64 = 0.0;
// REWRITES-X86_64-GNU-NEXT:     let mut d: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut local: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     d0 = 192.0;
// REWRITES-X86_64-GNU-NEXT:     a = LongDouble([120, 111, 94, 77, 60, 43, 26, 137, 4, 64]);
// REWRITES-X86_64-GNU-NEXT:     b = LongDouble([0, 0, 0, 8, 25, 42, 59, 204, 252, 191]);
// REWRITES-X86_64-GNU-NEXT:     f0 = 0.75;
// REWRITES-X86_64-GNU-NEXT:     c = LongDouble([0, 0, 0, 0, 0, 0, 128, 128, 8, 64]);
// REWRITES-X86_64-GNU-NEXT:     d1 = -0.125;
// REWRITES-X86_64-GNU-NEXT:     d = LongDouble([0, 0, 0, 0, 0, 128, 255, 255, 244, 63]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), a) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}} + b) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}} - c) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}} + d) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i64(i0);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u32(u0);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}} - {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64(d0);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f32(f0);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}} - {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32(i1);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_from_f64(d1);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(local), {{_v[0-9]+}} - {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_3: LongDouble = unsafe {
// REWRITES-X86_64-GNU-NEXT:             __slate_ext_mix_abi__rf80_i64_f64_f80_u32_f80_f32_f80_i32_f64_f80(
// REWRITES-X86_64-GNU-NEXT:                 i0 as i64, d0 as f64, a, u0 as u32, b, f0 as f32, c, i1 as i32, d1 as f64, d,
// REWRITES-X86_64-GNU-NEXT:             )
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_3: LongDouble =
// REWRITES-X86_64-GNU-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(local)) };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_3 == check_expected_3);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     497 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_mix_abi(i0, d0, a, u0, b, f0, c, i1, d1, d)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"local".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_3,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_3,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let mut fp: Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble> = None;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         std::ptr::write_volatile(std::ptr::addr_of_mut!(fp), unsafe {
// REWRITES-X86_64-GNU-NEXT:             std::mem::transmute::<
// REWRITES-X86_64-GNU-NEXT:                 *const (),
// REWRITES-X86_64-GNU-NEXT:                 Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// REWRITES-X86_64-GNU-NEXT:             >(__slate_ext_add__rf80_f80_f80 as *const ())
// REWRITES-X86_64-GNU-NEXT:         })
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_4: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_4: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble> =
// REWRITES-X86_64-GNU-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(fp)) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 128, 145, 3, 64]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 248, 191]);
// REWRITES-X86_64-GNU-NEXT:         check_got_4 = unsafe { {{_v[0-9]+}}.unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_4 = LongDouble([0, 0, 0, 0, 0, 0, 128, 145, 3, 64])
// REWRITES-X86_64-GNU-NEXT:             + LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 248, 191]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_4 == check_expected_4);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     504 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"fp(0x1.23p+4L, -0x1.2p-7L)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"0x1.23p+4L + -0x1.2p-7L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_4,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_4,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let mut a10: LongDouble = LongDouble([0, 0, 0, 128, 196, 179, 162, 145, 7, 64]);
// REWRITES-X86_64-GNU-NEXT:     let mut b2: LongDouble = LongDouble([0, 0, 0, 0, 0, 240, 230, 213, 251, 191]);
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_5: LongDouble = unsafe {
// REWRITES-X86_64-GNU-NEXT:             __slate_ext_call_cb__rf80_x_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                 unsafe {
// REWRITES-X86_64-GNU-NEXT:                     std::mem::transmute::<
// REWRITES-X86_64-GNU-NEXT:                         *const (),
// REWRITES-X86_64-GNU-NEXT:                         Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// REWRITES-X86_64-GNU-NEXT:                     >(__slate_ld_local_cb as *const ())
// REWRITES-X86_64-GNU-NEXT:                 },
// REWRITES-X86_64-GNU-NEXT:                 a10,
// REWRITES-X86_64-GNU-NEXT:                 b2,
// REWRITES-X86_64-GNU-NEXT:             )
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_5: LongDouble = local_cb(a10, b2);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_5 == check_expected_5);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     511 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_call_cb(local_cb, a, b)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"local_cb(a, b)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_5,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_5,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn test_struct_union_array_and_pointer_abi() {
// REWRITES-X86_64-GNU-NEXT:     let mut b: ld_box = ld_box {
// REWRITES-X86_64-GNU-NEXT:         tag: 0,
// REWRITES-X86_64-GNU-NEXT:         x: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:         tail: 0,
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let mut r: ld_box = ld_box {
// REWRITES-X86_64-GNU-NEXT:         tag: 0,
// REWRITES-X86_64-GNU-NEXT:         x: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:         tail: 0,
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let mut p: ld_pair = ld_pair {
// REWRITES-X86_64-GNU-NEXT:         a: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:         b: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let mut n: ld_nested = ld_nested {
// REWRITES-X86_64-GNU-NEXT:         head: 0,
// REWRITES-X86_64-GNU-NEXT:         pair: ld_pair {
// REWRITES-X86_64-GNU-NEXT:             a: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:             b: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:         },
// REWRITES-X86_64-GNU-NEXT:         bytes: [0; 3],
// REWRITES-X86_64-GNU-NEXT:         z: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let mut nr: ld_nested = ld_nested {
// REWRITES-X86_64-GNU-NEXT:         head: 0,
// REWRITES-X86_64-GNU-NEXT:         pair: ld_pair {
// REWRITES-X86_64-GNU-NEXT:             a: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:             b: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:         },
// REWRITES-X86_64-GNU-NEXT:         bytes: [0; 3],
// REWRITES-X86_64-GNU-NEXT:         z: LongDouble([0; 10]),
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     let mut arr: [LongDouble; 7] = [LongDouble([0; 10]); 7];
// REWRITES-X86_64-GNU-NEXT:     let mut copy: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut raw: aligned::Aligned<aligned::A16, [u8; 16]> = aligned::Aligned([0; 16]);
// REWRITES-X86_64-GNU-NEXT:     b.tag = 49;
// REWRITES-X86_64-GNU-NEXT:     b.x = LongDouble([128, 247, 230, 213, 196, 179, 162, 145, 11, 64]);
// REWRITES-X86_64-GNU-NEXT:     b.tail = 305419896;
// REWRITES-X86_64-GNU-NEXT:     r = unsafe { __slate_ext_box_roundtrip__rx_x(b as ld_box) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !((r.tag as i32) == ((((b.tag as u32) ^ 90) as u8) as i32));
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     530 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"r.tag == (unsigned char)(b.tag ^ 0x5aU)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_: LongDouble = r.x;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_: LongDouble = b.x + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 246, 63]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_ == check_expected_);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     531 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"r.x".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"b.x + 0x1p-9L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(r.tail == b.tail ^ 2779077210u32);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     532 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"r.tail == (b.tail ^ UINT32_C(0xa5a55a5a))".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([136, 136, 136, 136, 136, 136, 136, 136, 2, 192]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([16, 17, 17, 17, 17, 17, 17, 145, 252, 63]);
// REWRITES-X86_64-GNU-NEXT:     p = unsafe { __slate_ext_pair_make__rcf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_2: LongDouble = p.a;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_2: LongDouble =
// REWRITES-X86_64-GNU-NEXT:             LongDouble([136, 136, 136, 136, 136, 136, 136, 136, 2, 192]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_2 == check_expected_2);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     535 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"p.a".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"-0x1.111111111111111p+3L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_2,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_2,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_3: LongDouble = p.b;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_3: LongDouble =
// REWRITES-X86_64-GNU-NEXT:             LongDouble([16, 17, 17, 17, 17, 17, 17, 145, 252, 63]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_3 == check_expected_3);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     536 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"p.b".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"0x1.222222222222222p-3L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_3,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_3,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(n) as *mut core::ffi::c_void;
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_bytes({{_v[0-9]+}} as *mut u8, (0 as i32) as u8, (80 as u64) as usize) };
// REWRITES-X86_64-GNU-NEXT:     n.head = 4660;
// REWRITES-X86_64-GNU-NEXT:     n.pair.a = LongDouble([0, 0, 0, 0, 0, 0, 0, 144, 2, 64]);
// REWRITES-X86_64-GNU-NEXT:     n.pair.b = LongDouble([0, 0, 0, 0, 0, 0, 0, 176, 2, 192]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// REWRITES-X86_64-GNU-NEXT:     n.bytes[({{_v[0-9]+}} as usize)] = 16;
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// REWRITES-X86_64-GNU-NEXT:     n.bytes[({{_v[0-9]+}} as usize)] = 32;
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: i64 = 2;
// REWRITES-X86_64-GNU-NEXT:     n.bytes[({{_v[0-9]+}} as usize)] = 64;
// REWRITES-X86_64-GNU-NEXT:     n.z = LongDouble([0, 0, 0, 0, 0, 0, 0, 240, 1, 64]);
// REWRITES-X86_64-GNU-NEXT:     nr = unsafe { __slate_ext_nested_roundtrip__rx_x(n as ld_nested) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !((nr.head as i32) == ((((n.head as i32) ^ 21930) as u16) as i32));
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     547 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"nr.head == (uint16_t)(n.head ^ UINT16_C(0x55aa))".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_4: LongDouble = nr.pair.a;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_4: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 160, 2, 64]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_4 == check_expected_4);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     548 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"nr.pair.a".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"10.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_4,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_4,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_5: LongDouble = nr.pair.b;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_5: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 208, 2, 192]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_5 == check_expected_5);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     549 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"nr.pair.b".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"-13.0L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_5,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_5,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !((nr.bytes[0] as i32) == ((((n.bytes[0] as u32) ^ 1) as u8) as i32));
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     550 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"nr.bytes[0] == (unsigned char)(n.bytes[0] ^ 1U)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !((nr.bytes[1] as i32) == ((((n.bytes[1] as u32) ^ 2) as u8) as i32));
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     551 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"nr.bytes[1] == (unsigned char)(n.bytes[1] ^ 2U)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !((nr.bytes[2] as i32) == ((((n.bytes[2] as u32) ^ 4) as u8) as i32));
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     552 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"nr.bytes[2] == (unsigned char)(n.bytes[2] ^ 4U)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_6: LongDouble = nr.z;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_6: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 240, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_6 == check_expected_6);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     553 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"nr.z".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"3.75L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_6,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_6,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     for i in 0..(112 / 16) {
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = i + 1;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_u64({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         arr[(i as usize)] = {{_v[0-9]+}} / LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 2, 64]);
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_7: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_7: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: *mut LongDouble = arr.as_mut_ptr() as *mut LongDouble;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 16;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: u64 = 112 / {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         check_got_7 = unsafe { __slate_ext_array_sum__rf80_pf80_usize({{_v[0-9]+}}, {{_v[0-9]+}} as usize) };
// REWRITES-X86_64-GNU-NEXT:         check_expected_7 = LongDouble([0, 0, 0, 0, 0, 0, 0, 224, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_7 == check_expected_7);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     557 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_array_sum(arr, sizeof(arr) / sizeof(arr[0]))".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"3.5L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_7,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_7,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     copy = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([192, 179, 162, 145, 128, 247, 230, 213, 32, 192]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { __slate_ext_store__rv_pf80_f80(std::ptr::addr_of_mut!(copy), {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_8: LongDouble = copy;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_8: LongDouble =
// REWRITES-X86_64-GNU-NEXT:             LongDouble([192, 179, 162, 145, 128, 247, 230, 213, 32, 192]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_8 == check_expected_8);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     561 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"copy".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"-0x1.abcdef012345678p+33L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_8,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_9: LongDouble =
// REWRITES-X86_64-GNU-NEXT:             unsafe { __slate_ext_load__rf80_pf80(std::ptr::addr_of_mut!(copy)) };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_9: LongDouble = copy;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_9 == check_expected_9);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     562 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_load(&copy)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"copy".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_9,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_9,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     copy = LongDouble([112, 86, 52, 146, 239, 205, 171, 137, 235, 63]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut u8 = raw.as_mut_ptr() as *mut u8;
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(copy) as *mut core::ffi::c_void;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         std::ptr::copy_nonoverlapping({{_v[0-9]+}} as *const u8, {{_v[0-9]+}} as *mut u8, (16 as u64) as usize)
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     copy = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(copy) as *mut core::ffi::c_void;
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut u8 = raw.as_mut_ptr() as *mut u8;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         std::ptr::copy_nonoverlapping({{_v[0-9]+}} as *const u8, {{_v[0-9]+}} as *mut u8, (16 as u64) as usize)
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_10: LongDouble = copy;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_10: LongDouble =
// REWRITES-X86_64-GNU-NEXT:             LongDouble([112, 86, 52, 146, 239, 205, 171, 137, 235, 63]);
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_10 == check_expected_10);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     570 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"copy".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"0x1.13579bdf2468acep-20L".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_10,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_10,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let mut u: ld_union = unsafe { std::mem::zeroed::<ld_union>() };
// REWRITES-X86_64-GNU-NEXT:     let mut v: ld_union = unsafe { std::mem::zeroed::<ld_union>() };
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         u.ld = LongDouble([248, 222, 188, 154, 112, 86, 52, 146, 18, 192]);
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(v) as *mut core::ffi::c_void;
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = std::ptr::addr_of_mut!(u) as *mut core::ffi::c_void;
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         std::ptr::copy_nonoverlapping({{_v[0-9]+}} as *const u8, {{_v[0-9]+}} as *mut u8, (16 as u64) as usize)
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_11: LongDouble = unsafe { v.ld };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_11: LongDouble = unsafe { u.ld };
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_11 == check_expected_11);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     577 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"v.ld".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"u.ld".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_11,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_11,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: /// Translated va_arg(long double).
// REWRITES-X86_64-GNU-NEXT: /// Translated va_arg(long double).
// REWRITES-X86_64-GNU-NEXT: /// Translated caller -> native C variadic callee.
// REWRITES-X86_64-GNU-NEXT: /// Translated caller -> native C variadic callee.
// REWRITES-X86_64-GNU-NEXT: fn test_varargs() {
// REWRITES-X86_64-GNU-NEXT:     let mut a: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut b: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 253, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut c: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 251, 191]);
// REWRITES-X86_64-GNU-NEXT:     let mut d: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 247, 63]);
// REWRITES-X86_64-GNU-NEXT:     let mut expected: LongDouble = a + b + c + d;
// REWRITES-X86_64-GNU-NEXT:     let mut buf: aligned::Aligned<aligned::A16, [i8; 128]> = aligned::Aligned([0; 128]);
// REWRITES-X86_64-GNU-NEXT:     let mut end: *mut i8 = std::ptr::null_mut();
// REWRITES-X86_64-GNU-NEXT:     let mut n: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = a;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = b;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = c;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = d;
// REWRITES-X86_64-GNU-NEXT:         check_got_ = unsafe {
// REWRITES-X86_64-GNU-NEXT:             local_vsum(
// REWRITES-X86_64-GNU-NEXT:                 4 as i32,
// REWRITES-X86_64-GNU-NEXT:                 __SlateVaArgs::new(vec![
// REWRITES-X86_64-GNU-NEXT:                     __SlateVaArg::new({{_v[0-9]+}}),
// REWRITES-X86_64-GNU-NEXT:                     __SlateVaArg::new({{_v[0-9]+}}),
// REWRITES-X86_64-GNU-NEXT:                     __SlateVaArg::new({{_v[0-9]+}}),
// REWRITES-X86_64-GNU-NEXT:                     __SlateVaArg::new({{_v[0-9]+}}),
// REWRITES-X86_64-GNU-NEXT:                 ]),
// REWRITES-X86_64-GNU-NEXT:             )
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         check_expected_ = expected;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_ == check_expected_);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     592 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"local_vsum(4, a, b, c, d)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"expected".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_2: LongDouble =
// REWRITES-X86_64-GNU-NEXT:             unsafe { __slate_ext_vsum__rf80_i32_f80_f80_f80_f80(4 as i32, a, b, c, d) };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_2: LongDouble = expected;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_2 == check_expected_2);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     595 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_vsum(4, a, b, c, d)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"expected".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_2,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_2,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     n = unsafe {
// REWRITES-X86_64-GNU-NEXT:         __slate_snprintf__ri32_pi8_u64_pi8_f80(
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}} as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             128 as u64,
// REWRITES-X86_64-GNU-NEXT:             c"%.21La".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:             expected,
// REWRITES-X86_64-GNU-NEXT:         )
// REWRITES-X86_64-GNU-NEXT:     };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if n > 0 {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = (n as u64) < 128;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     599 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"n > 0 && (size_t)n < sizeof(buf)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_3: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_3: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: *mut i8 = buf.as_mut_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:         check_got_3 = unsafe {
// REWRITES-X86_64-GNU-NEXT:             __slate_strtold__rf80_pc_ppc(
// REWRITES-X86_64-GNU-NEXT:                 {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-X86_64-GNU-NEXT:                 std::ptr::addr_of_mut!(end) as *mut *mut core::ffi::c_char,
// REWRITES-X86_64-GNU-NEXT:             )
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         check_expected_3 = expected;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_3 == check_expected_3);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     601 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"strtold(buf, &end)".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"expected".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_3,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_3,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = if end != std::ptr::null_mut() {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = ((unsafe { *end }) as i32) == 0;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = false;
// REWRITES-X86_64-GNU-NEXT:             {{_v[0-9]+}}
// REWRITES-X86_64-GNU-NEXT:         };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 fprintf(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s\n".as_ptr(),
// REWRITES-X86_64-GNU-NEXT:                     602 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"end != NULL && *end == '\\0'".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn test_extern_global() {
// REWRITES-X86_64-GNU-NEXT:     let mut old: LongDouble = unsafe { ext_global_ld };
// REWRITES-X86_64-GNU-NEXT:     let mut a: LongDouble = LongDouble([196, 179, 162, 145, 128, 247, 230, 213, 26, 192]);
// REWRITES-X86_64-GNU-NEXT:     let mut b: LongDouble = LongDouble([132, 3, 131, 2, 130, 1, 129, 128, 224, 63]);
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         ext_global_ld = a;
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_: LongDouble = unsafe { __slate_ext_global_get__rf80() };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_: LongDouble = a;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_ == check_expected_);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     611 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_global_get()".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"a".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     unsafe { __slate_ext_global_set__rv_f80(b) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_2: LongDouble = unsafe { ext_global_ld };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_2: LongDouble = b;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_2 == check_expected_2);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     614 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_global_ld".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"b".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_2,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_2,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     unsafe { __slate_ext_global_set__rv_f80(old) };
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_3: LongDouble = unsafe { ext_global_ld };
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_3: LongDouble = old;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_3 == check_expected_3);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     617 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"ext_global_ld".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"old".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_3,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_3,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn test_control_flow_and_spills() {
// REWRITES-X86_64-GNU-NEXT:     let mut seed: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut x: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut i: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([248, 230, 213, 196, 179, 162, 145, 128, 3, 64]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(seed), {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     x = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(seed)) };
// REWRITES-X86_64-GNU-NEXT:     while i < 200 {
// REWRITES-X86_64-GNU-NEXT:         let mut t: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = i % 17 - 8;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         t = {{_v[0-9]+}} * LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 243, 63]);
// REWRITES-X86_64-GNU-NEXT:         if i & 3 == 0 {
// REWRITES-X86_64-GNU-NEXT:             x += t;
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             if i & 3 == 1 {
// REWRITES-X86_64-GNU-NEXT:                 x -= t;
// REWRITES-X86_64-GNU-NEXT:             } else {
// REWRITES-X86_64-GNU-NEXT:                 if i & 3 == 2 {
// REWRITES-X86_64-GNU-NEXT:                     x *= LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63])
// REWRITES-X86_64-GNU-NEXT:                         + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 235, 63]);
// REWRITES-X86_64-GNU-NEXT:                 } else {
// REWRITES-X86_64-GNU-NEXT:                     x /= LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63])
// REWRITES-X86_64-GNU-NEXT:                         + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 235, 63]);
// REWRITES-X86_64-GNU-NEXT:                 }
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = i % 19 == 0;
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             x = local_identity(x);
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = i % 31 == 0;
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             x = unsafe { __slate_ext_identity__rf80_f80(x) };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         i += 1;
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     let mut y: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(seed)) };
// REWRITES-X86_64-GNU-NEXT:     i = 0;
// REWRITES-X86_64-GNU-NEXT:     while i < 200 {
// REWRITES-X86_64-GNU-NEXT:         let mut t2: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = i % 17 - 8;
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = __slate_f80_from_i32({{_v[0-9]+}});
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 243, 63]);
// REWRITES-X86_64-GNU-NEXT:         t2 = unsafe { __slate_ext_mul__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = i & 3 == 0;
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             y = unsafe { __slate_ext_add__rf80_f80_f80(y, t2) };
// REWRITES-X86_64-GNU-NEXT:         } else {
// REWRITES-X86_64-GNU-NEXT:             let {{_v[0-9]+}}: bool = i & 3 == 1;
// REWRITES-X86_64-GNU-NEXT:             if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:                 y = unsafe { __slate_ext_sub__rf80_f80_f80(y, t2) };
// REWRITES-X86_64-GNU-NEXT:             } else {
// REWRITES-X86_64-GNU-NEXT:                 let {{_v[0-9]+}}: bool = i & 3 == 2;
// REWRITES-X86_64-GNU-NEXT:                 if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = y;
// REWRITES-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 235, 63]);
// REWRITES-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:                     y = unsafe { __slate_ext_mul__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:                 } else {
// REWRITES-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = y;
// REWRITES-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 255, 63]);
// REWRITES-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 235, 63]);
// REWRITES-X86_64-GNU-NEXT:                     let {{_v[0-9]+}}: LongDouble = unsafe { __slate_ext_add__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:                     y = unsafe { __slate_ext_div__rf80_f80_f80({{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:                 }
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = i % 19 == 0;
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             y = unsafe { __slate_ext_identity__rf80_f80(y) };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = i % 31 == 0;
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             y = unsafe { __slate_ext_identity__rf80_f80(y) };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         i += 1;
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     loop {
// REWRITES-X86_64-GNU-NEXT:         let mut check_got_: LongDouble = x;
// REWRITES-X86_64-GNU-NEXT:         let mut check_expected_: LongDouble = y;
// REWRITES-X86_64-GNU-NEXT:         unsafe {
// REWRITES-X86_64-GNU-NEXT:             checks = (unsafe { checks }) + 1;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: bool = !(check_got_ == check_expected_);
// REWRITES-X86_64-GNU-NEXT:         if {{_v[0-9]+}} {
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 failures = (unsafe { failures }) + 1;
// REWRITES-X86_64-GNU-NEXT:             }
// REWRITES-X86_64-GNU-NEXT:             unsafe {
// REWRITES-X86_64-GNU-NEXT:                 __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:                     (unsafe { stderr }) as *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:                     c"FAIL line %d: %s != %s (got=%La expected=%La)\n".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     664 as i32,
// REWRITES-X86_64-GNU-NEXT:                     c"x".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     c"y".as_ptr() as *mut i8,
// REWRITES-X86_64-GNU-NEXT:                     check_got_,
// REWRITES-X86_64-GNU-NEXT:                     check_expected_,
// REWRITES-X86_64-GNU-NEXT:                 )
// REWRITES-X86_64-GNU-NEXT:             };
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-X86_64-GNU-NEXT:         if !({{_v[0-9]+}} != 0) {
// REWRITES-X86_64-GNU-NEXT:             break;
// REWRITES-X86_64-GNU-NEXT:         }
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     return;
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn ld_kind() -> *mut i8 {
// REWRITES-X86_64-GNU-NEXT:     c"x87-extended".as_ptr() as *mut i8
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[unsafe(no_mangle)]
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: extern "C-unwind" fn local_cb({{arg[0-9]+}}: LongDouble, {{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     let mut x: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     let mut y: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(x), {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 192, 0, 64]);
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// REWRITES-X86_64-GNU-NEXT:     let {{_v[0-9]+}}: LongDouble = __slate_f80_fma(
// REWRITES-X86_64-GNU-NEXT:         {{_v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:         {{_v[0-9]+}},
// REWRITES-X86_64-GNU-NEXT:         -({{_v[0-9]+}} / LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 0, 64])),
// REWRITES-X86_64-GNU-NEXT:     );
// REWRITES-X86_64-GNU-NEXT:     {{_v[0-9]+}} + LongDouble([0, 0, 0, 0, 0, 0, 0, 128, 238, 63])
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: unsafe fn local_vsum(mut n: i32, mut __slate_va_args: __SlateVaArgs) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     let mut ap: __SlateVaArgs = __SlateVaArgs::empty();
// REWRITES-X86_64-GNU-NEXT:     let mut r: LongDouble = LongDouble([0, 0, 0, 0, 0, 0, 0, 0, 0, 0]);
// REWRITES-X86_64-GNU-NEXT:     unsafe {
// REWRITES-X86_64-GNU-NEXT:         ap = __slate_va_args.clone();
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     for i in 0..n {
// REWRITES-X86_64-GNU-NEXT:         let {{_v[0-9]+}}: LongDouble = unsafe { ap.next_arg::<LongDouble>() };
// REWRITES-X86_64-GNU-NEXT:         r += {{_v[0-9]+}};
// REWRITES-X86_64-GNU-NEXT:     }
// REWRITES-X86_64-GNU-NEXT:     r
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: #[inline(never)]
// REWRITES-X86_64-GNU-NEXT: fn local_identity({{arg[0-9]+}}: LongDouble) -> LongDouble {
// REWRITES-X86_64-GNU-NEXT:     let mut y: LongDouble = LongDouble([0; 10]);
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(y), {{arg[0-9]+}}) };
// REWRITES-X86_64-GNU-NEXT:     unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) }
// REWRITES-X86_64-GNU-NEXT: }
// REWRITES-X86_64-GNU-EMPTY:
// REWRITES-X86_64-GNU-NEXT: unsafe extern "C" {
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_div(
// REWRITES-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_cf80_mul(
// REWRITES-X86_64-GNU-NEXT:         __a: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         __b: num_complex::Complex<LongDouble>,
// REWRITES-X86_64-GNU-NEXT:     ) -> num_complex::Complex<LongDouble>;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_add__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_array_sum__rf80_pf80_usize(_0: *const LongDouble, _1: usize) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_box_roundtrip__rx_x(_0: ld_box) -> ld_box;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_call_cb__rf80_x_f80_f80(
// REWRITES-X86_64-GNU-NEXT:         _0: Option<unsafe extern "C-unwind" fn(LongDouble, LongDouble) -> LongDouble>,
// REWRITES-X86_64-GNU-NEXT:         _1: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _2: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     ) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_div__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_eq__ri32_f80_f80(_0: LongDouble, _1: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_from_double__rf80_f64(_0: f64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_from_float__rf80_f32(_0: f32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_from_i64__rf80_i64(_0: i64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_from_u64__rf80_u64(_0: u64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_global_get__rf80() -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_global_set__rv_f80(_0: LongDouble);
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_identity__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_le__ri32_f80_f80(_0: LongDouble, _1: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_load__rf80_pf80(_0: *const LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_lt__ri32_f80_f80(_0: LongDouble, _1: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_mix_abi__rf80_i64_f64_f80_u32_f80_f32_f80_i32_f64_f80(
// REWRITES-X86_64-GNU-NEXT:         _0: i64,
// REWRITES-X86_64-GNU-NEXT:         _1: f64,
// REWRITES-X86_64-GNU-NEXT:         _2: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _3: u32,
// REWRITES-X86_64-GNU-NEXT:         _4: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _5: f32,
// REWRITES-X86_64-GNU-NEXT:         _6: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _7: i32,
// REWRITES-X86_64-GNU-NEXT:         _8: f64,
// REWRITES-X86_64-GNU-NEXT:         _9: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     ) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_mul__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_neg__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_nested_roundtrip__rx_x(_0: ld_nested) -> ld_nested;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_pair_make__rcf80_f80_f80(_0: LongDouble, _1: LongDouble) -> ld_pair;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_store__rv_pf80_f80(_0: *mut LongDouble, _1: LongDouble);
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_sub__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_sum10__rf80_f80_f80_f80_f80_f80_f80_f80_f80_f80_f80(
// REWRITES-X86_64-GNU-NEXT:         _0: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _1: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _2: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _3: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _4: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _5: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _6: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _7: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _8: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _9: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     ) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_to_double__rf64_f80(_0: LongDouble) -> f64;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_to_float__rf32_f80(_0: LongDouble) -> f32;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_to_i64__ri64_f80(_0: LongDouble) -> i64;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_to_u64__ru64_f80(_0: LongDouble) -> u64;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ext_vsum__rf80_i32_f80_f80_f80_f80(
// REWRITES-X86_64-GNU-NEXT:         _0: i32,
// REWRITES-X86_64-GNU-NEXT:         _1: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _2: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _3: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _4: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     ) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_abs(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_add(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ceil(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_copysign(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_div(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_eq(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_floor(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fma(__a: LongDouble, __b: LongDouble, __c: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmax(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fmin(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_fract(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_bool(__a: bool) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f32(__a: f32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_f64(__a: f64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i128(__a: i128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i16(__a: i16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i32(__a: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i64(__a: i64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_i8(__a: i8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u128(__a: u128) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u16(__a: u16) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u32(__a: u32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u64(__a: u64) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_from_u8(__a: u8) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ge(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_gt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_is_fp_class(__a: LongDouble, __flags: i32) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_le(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_lt(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_mul(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_ne(__a: LongDouble, __b: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_neg(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_rint(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_round(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_signbit(__a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_sub(__a: LongDouble, __b: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_bool(__a: LongDouble) -> bool;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f32(__a: LongDouble) -> f32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_f64(__a: LongDouble) -> f64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i128(__a: LongDouble) -> i128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i16(__a: LongDouble) -> i16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i32(__a: LongDouble) -> i32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i64(__a: LongDouble) -> i64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_i8(__a: LongDouble) -> i8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u128(__a: LongDouble) -> u128;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u16(__a: LongDouble) -> u16;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u32(__a: LongDouble) -> u32;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u64(__a: LongDouble) -> u64;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_to_u8(__a: LongDouble) -> u8;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_f80_trunc(__a: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_fmodl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_fprintf__ri32_px_pi8_i32_pi8_pi8_f80_f80(
// REWRITES-X86_64-GNU-NEXT:         _0: *mut libc::FILE,
// REWRITES-X86_64-GNU-NEXT:         _1: *mut i8,
// REWRITES-X86_64-GNU-NEXT:         _2: i32,
// REWRITES-X86_64-GNU-NEXT:         _3: *mut i8,
// REWRITES-X86_64-GNU-NEXT:         _4: *mut i8,
// REWRITES-X86_64-GNU-NEXT:         _5: LongDouble,
// REWRITES-X86_64-GNU-NEXT:         _6: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     ) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_frexpl__rf80_f80_pi32(_0: LongDouble, _1: *mut i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     safe fn __slate_ld_local_cb(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_ldexpl__rf80_f80_i32(_0: LongDouble, _1: i32) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_nanl__rf80_pc(_0: *const core::ffi::c_char) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_nextafterl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_powl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_remainderl__rf80_f80_f80(_0: LongDouble, _1: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_snprintf__ri32_pi8_u64_pi8_f80(
// REWRITES-X86_64-GNU-NEXT:         _0: *mut i8,
// REWRITES-X86_64-GNU-NEXT:         _1: u64,
// REWRITES-X86_64-GNU-NEXT:         _2: *mut i8,
// REWRITES-X86_64-GNU-NEXT:         _3: LongDouble,
// REWRITES-X86_64-GNU-NEXT:     ) -> i32;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_sqrtl__rf80_f80(_0: LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT:     fn __slate_strtold__rf80_pc_ppc(
// REWRITES-X86_64-GNU-NEXT:         _0: *const core::ffi::c_char,
// REWRITES-X86_64-GNU-NEXT:         _1: *mut *mut core::ffi::c_char,
// REWRITES-X86_64-GNU-NEXT:     ) -> LongDouble;
// REWRITES-X86_64-GNU-NEXT: }
// SLATE-FILECHECK-END rewrites-x86_64-gnu
