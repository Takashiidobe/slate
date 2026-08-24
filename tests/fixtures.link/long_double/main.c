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
// REWRITES-X86_64-GNU-DAG: fn ext_store(_0: *mut LongDouble, _1: LongDouble);
// REWRITES-X86_64-GNU-DAG: fn ext_load(_0: *const LongDouble) -> LongDouble;
// REWRITES-X86_64-GNU-DAG: fn __slate_ext_store__rv_pf80_f80(_0: *mut LongDouble
// REWRITES-X86_64-GNU-DAG: fn __slate_ext_load__rf80_pf80(_0: *const LongDouble
