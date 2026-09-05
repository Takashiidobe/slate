#include <complex.h>
#include <float.h>
#include <math.h>
#include <stdint.h>
#include <string.h>

typedef struct {
  unsigned char bytes[10];
} __attribute__((aligned(16))) __slate_f80;

static long double __slate_f80_load(__slate_f80 value) {
  long double out = 0.0L;
  memcpy(&out, value.bytes, sizeof(value.bytes));
  return out;
}

static __slate_f80 __slate_f80_store(long double value) {
  __slate_f80 out;
  memcpy(out.bytes, &value, sizeof(out.bytes));
  return out;
}

typedef struct {
  __slate_f80 re;
  __slate_f80 im;
} __attribute__((aligned(16))) __slate_cf80;

static long double _Complex __slate_cf80_load(__slate_cf80 value) {
  return CMPLXL(__slate_f80_load(value.re), __slate_f80_load(value.im));
}

static __slate_cf80 __slate_cf80_store(long double _Complex value) {
  __slate_cf80 out = {
      __slate_f80_store(creall(value)),
      __slate_f80_store(cimagl(value)),
  };
  return out;
}

__slate_cf80 __slate_cf80_make(__slate_f80 re, __slate_f80 im) {
  __slate_cf80 out = {re, im};
  return out;
}

__slate_cf80 __slate_cf80_pos(__slate_cf80 a) { return a; }

__slate_cf80 __slate_cf80_neg(__slate_cf80 a) {
  return __slate_cf80_store(-__slate_cf80_load(a));
}

__slate_cf80 __slate_cf80_add(__slate_cf80 a, __slate_cf80 b) {
  return __slate_cf80_store(__slate_cf80_load(a) + __slate_cf80_load(b));
}

__slate_cf80 __slate_cf80_sub(__slate_cf80 a, __slate_cf80 b) {
  return __slate_cf80_store(__slate_cf80_load(a) - __slate_cf80_load(b));
}

__slate_cf80 __slate_cf80_mul(__slate_cf80 a, __slate_cf80 b) {
  return __slate_cf80_store(__slate_cf80_load(a) * __slate_cf80_load(b));
}

__slate_cf80 __slate_cf80_div(__slate_cf80 a, __slate_cf80 b) {
  return __slate_cf80_store(__slate_cf80_load(a) / __slate_cf80_load(b));
}

__slate_cf80 __slate_cf80_add_assign(__slate_cf80 *a, __slate_cf80 b) {
  *a = __slate_cf80_add(*a, b);
  return *a;
}

__slate_cf80 __slate_cf80_sub_assign(__slate_cf80 *a, __slate_cf80 b) {
  *a = __slate_cf80_sub(*a, b);
  return *a;
}

__slate_cf80 __slate_cf80_mul_assign(__slate_cf80 *a, __slate_cf80 b) {
  *a = __slate_cf80_mul(*a, b);
  return *a;
}

__slate_cf80 __slate_cf80_div_assign(__slate_cf80 *a, __slate_cf80 b) {
  *a = __slate_cf80_div(*a, b);
  return *a;
}

_Bool __slate_cf80_eq(__slate_cf80 a, __slate_cf80 b) {
  return __slate_cf80_load(a) == __slate_cf80_load(b);
}

_Bool __slate_cf80_ne(__slate_cf80 a, __slate_cf80 b) {
  return __slate_cf80_load(a) != __slate_cf80_load(b);
}

_Bool __slate_cf80_to_bool(__slate_cf80 a) {
  return __slate_cf80_load(a) != 0.0L;
}

#define SLATE_CF80_UNARY(name, fn)                                             \
  __slate_cf80 __slate_cf80_##name(__slate_cf80 a) {                           \
    return __slate_cf80_store(fn(__slate_cf80_load(a)));                       \
  }

#define SLATE_CF80_BINARY(name, fn)                                            \
  __slate_cf80 __slate_cf80_##name(__slate_cf80 a, __slate_cf80 b) {           \
    return __slate_cf80_store(fn(__slate_cf80_load(a), __slate_cf80_load(b))); \
  }

#define SLATE_CF80_REAL(name, fn)                                              \
  __slate_f80 __slate_cf80_##name(__slate_cf80 a) {                            \
    return __slate_f80_store(fn(__slate_cf80_load(a)));                        \
  }

#define SLATE_CF80_UNARY(name, fn)                                             \
  __slate_cf80 __slate_cf80_##name(__slate_cf80 a) {                           \
    return __slate_cf80_store(fn(__slate_cf80_load(a)));                       \
  }

#define SLATE_CF80_BINARY(name, fn)                                            \
  __slate_cf80 __slate_cf80_##name(__slate_cf80 a, __slate_cf80 b) {           \
    return __slate_cf80_store(fn(__slate_cf80_load(a), __slate_cf80_load(b))); \
  }

#define SLATE_CF80_REAL(name, fn)                                              \
  __slate_f80 __slate_cf80_##name(__slate_cf80 a) {                            \
    return __slate_f80_store(fn(__slate_cf80_load(a)));                        \
  }

/* Trigonometric */

SLATE_CF80_UNARY(acos, cacosl)
SLATE_CF80_UNARY(asin, casinl)
SLATE_CF80_UNARY(atan, catanl)
SLATE_CF80_UNARY(cos, ccosl)
SLATE_CF80_UNARY(sin, csinl)
SLATE_CF80_UNARY(tan, ctanl)

/* Hyperbolic */

SLATE_CF80_UNARY(acosh, cacoshl)
SLATE_CF80_UNARY(asinh, casinhl)
SLATE_CF80_UNARY(atanh, catanhl)
SLATE_CF80_UNARY(cosh, ccoshl)
SLATE_CF80_UNARY(sinh, csinhl)
SLATE_CF80_UNARY(tanh, ctanhl)

/* Exponential / logarithmic */

SLATE_CF80_UNARY(exp, cexpl)
SLATE_CF80_UNARY(log, clogl)

/* Power */

SLATE_CF80_BINARY(pow, cpowl)
SLATE_CF80_UNARY(sqrt, csqrtl)

/* Real-valued */

SLATE_CF80_REAL(abs, cabsl)
SLATE_CF80_REAL(arg, cargl)

/* Manipulation */

SLATE_CF80_UNARY(conj, conjl)
SLATE_CF80_UNARY(proj, cprojl)

#undef SLATE_CF80_UNARY
#undef SLATE_CF80_BINARY
#undef SLATE_CF80_REAL

__slate_f80 __slate_f80_add(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_store(__slate_f80_load(a) + __slate_f80_load(b));
}

__slate_f80 __slate_f80_sub(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_store(__slate_f80_load(a) - __slate_f80_load(b));
}

__slate_f80 __slate_f80_mul(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_store(__slate_f80_load(a) * __slate_f80_load(b));
}

__slate_f80 __slate_f80_div(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_store(__slate_f80_load(a) / __slate_f80_load(b));
}

__slate_f80 __slate_f80_powi(__slate_f80 a, int n) {
  return __slate_f80_store(powl(__slate_f80_load(a), n));
}

__slate_f80 __slate_f80_add_assign(__slate_f80 *a, __slate_f80 b) {
  *a = __slate_f80_add(*a, b);
  return *a;
}

__slate_f80 __slate_f80_sub_assign(__slate_f80 *a, __slate_f80 b) {
  *a = __slate_f80_sub(*a, b);
  return *a;
}

__slate_f80 __slate_f80_mul_assign(__slate_f80 *a, __slate_f80 b) {
  *a = __slate_f80_mul(*a, b);
  return *a;
}

__slate_f80 __slate_f80_div_assign(__slate_f80 *a, __slate_f80 b) {
  *a = __slate_f80_div(*a, b);
  return *a;
}

__slate_f80 __slate_f80_inc(__slate_f80 a) {
  return __slate_f80_store(__slate_f80_load(a) + 1.0L);
}

__slate_f80 __slate_f80_dec(__slate_f80 a) {
  return __slate_f80_store(__slate_f80_load(a) - 1.0L);
}

__slate_f80 __slate_f80_neg(__slate_f80 a) {
  return __slate_f80_store(-__slate_f80_load(a));
}

__slate_f80 __slate_f80_abs(__slate_f80 a) {
  return __slate_f80_store(fabsl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_ceil(__slate_f80 a) {
  return __slate_f80_store(ceill(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_floor(__slate_f80 a) {
  return __slate_f80_store(floorl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_round(__slate_f80 a) {
  return __slate_f80_store(roundl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_trunc(__slate_f80 a) {
  return __slate_f80_store(truncl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_fract(__slate_f80 a) {
  long double integral;
  return __slate_f80_store(modfl(__slate_f80_load(a), &integral));
}

__slate_f80 __slate_f80_fmod(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_store(fmodl(__slate_f80_load(a), __slate_f80_load(b)));
}

__slate_f80 __slate_f80_remainder(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_store(
      remainderl(__slate_f80_load(a), __slate_f80_load(b)));
}

__slate_f80 __slate_f80_remquo(__slate_f80 a, __slate_f80 b, int *quo) {
  return __slate_f80_store(
      remquol(__slate_f80_load(a), __slate_f80_load(b), quo));
}

__slate_f80 __slate_f80_copysign(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_store(copysignl(__slate_f80_load(a), __slate_f80_load(b)));
}

__slate_f80 __slate_f80_fma(__slate_f80 a, __slate_f80 b, __slate_f80 c) {
  return __slate_f80_store(
      fmal(__slate_f80_load(a), __slate_f80_load(b), __slate_f80_load(c)));
}

__slate_f80 __slate_f80_sqrt(__slate_f80 a) {
  return __slate_f80_store(sqrtl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_cbrt(__slate_f80 a) {
  return __slate_f80_store(cbrtl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_pow(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_store(powl(__slate_f80_load(a), __slate_f80_load(b)));
}

__slate_f80 __slate_f80_exp(__slate_f80 a) {
  return __slate_f80_store(expl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_exp2(__slate_f80 a) {
  return __slate_f80_store(exp2l(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_expm1(__slate_f80 a) {
  return __slate_f80_store(expm1l(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_log(__slate_f80 a) {
  return __slate_f80_store(logl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_log2(__slate_f80 a) {
  return __slate_f80_store(log2l(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_log10(__slate_f80 a) {
  return __slate_f80_store(log10l(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_log1p(__slate_f80 a) {
  return __slate_f80_store(log1pl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_sin(__slate_f80 a) {
  return __slate_f80_store(sinl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_cos(__slate_f80 a) {
  return __slate_f80_store(cosl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_tan(__slate_f80 a) {
  return __slate_f80_store(tanl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_asin(__slate_f80 a) {
  return __slate_f80_store(asinl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_acos(__slate_f80 a) {
  return __slate_f80_store(acosl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_atan(__slate_f80 a) {
  return __slate_f80_store(atanl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_atan2(__slate_f80 y, __slate_f80 x) {
  return __slate_f80_store(atan2l(__slate_f80_load(y), __slate_f80_load(x)));
}

_Bool __slate_f80_signbit(__slate_f80 a) {
  return signbit(__slate_f80_load(a));
}

__slate_f80 __slate_f80_rint(__slate_f80 a) {
  return __slate_f80_store(rintl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_nearbyint(__slate_f80 a) {
  return __slate_f80_store(nearbyintl(__slate_f80_load(a)));
}

long __slate_f80_lrint(__slate_f80 a) { return lrintl(__slate_f80_load(a)); }

long long __slate_f80_llrint(__slate_f80 a) {
  return llrintl(__slate_f80_load(a));
}

__slate_f80 __slate_f80_sinh(__slate_f80 a) {
  return __slate_f80_store(sinhl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_cosh(__slate_f80 a) {
  return __slate_f80_store(coshl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_tanh(__slate_f80 a) {
  return __slate_f80_store(tanhl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_asinh(__slate_f80 a) {
  return __slate_f80_store(asinhl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_acosh(__slate_f80 a) {
  return __slate_f80_store(acoshl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_atanh(__slate_f80 a) {
  return __slate_f80_store(atanhl(__slate_f80_load(a)));
}

long __slate_f80_lround(__slate_f80 a) { return lroundl(__slate_f80_load(a)); }

long long __slate_f80_llround(__slate_f80 a) {
  return llroundl(__slate_f80_load(a));
}

__slate_f80 __slate_f80_fdim(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_store(fdiml(__slate_f80_load(a), __slate_f80_load(b)));
}

__slate_f80 __slate_f80_fmax(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_store(fmaxl(__slate_f80_load(a), __slate_f80_load(b)));
}

__slate_f80 __slate_f80_fmin(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_store(fminl(__slate_f80_load(a), __slate_f80_load(b)));
}

__slate_f80 __slate_f80_hypot(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_store(hypotl(__slate_f80_load(a), __slate_f80_load(b)));
}

__slate_f80 __slate_f80_frexp(__slate_f80 a, int *exp) {
  return __slate_f80_store(frexpl(__slate_f80_load(a), exp));
}

__slate_f80 __slate_f80_ldexp(__slate_f80 a, int exp) {
  return __slate_f80_store(ldexpl(__slate_f80_load(a), exp));
}

__slate_f80 __slate_f80_scalbn(__slate_f80 a, int exp) {
  return __slate_f80_store(scalbnl(__slate_f80_load(a), exp));
}

__slate_f80 __slate_f80_scalbln(__slate_f80 a, long exp) {
  return __slate_f80_store(scalblnl(__slate_f80_load(a), exp));
}

int __slate_f80_ilogb(__slate_f80 a) { return ilogbl(__slate_f80_load(a)); }

__slate_f80 __slate_f80_logb(__slate_f80 a) {
  return __slate_f80_store(logbl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_nextafter(__slate_f80 from, __slate_f80 to) {
  return __slate_f80_store(
      nextafterl(__slate_f80_load(from), __slate_f80_load(to)));
}

__slate_f80 __slate_f80_nexttoward(__slate_f80 from, __slate_f80 to) {
  return __slate_f80_store(
      nexttowardl(__slate_f80_load(from), __slate_f80_load(to)));
}

__slate_f80 __slate_f80_modf(__slate_f80 a, __slate_f80 *integral) {
  long double ipart;
  long double frac = modfl(__slate_f80_load(a), &ipart);

  *integral = __slate_f80_store(ipart);
  return __slate_f80_store(frac);
}

__slate_f80 __slate_f80_erf(__slate_f80 a) {
  return __slate_f80_store(erfl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_erfc(__slate_f80 a) {
  return __slate_f80_store(erfcl(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_tgamma(__slate_f80 a) {
  return __slate_f80_store(tgammal(__slate_f80_load(a)));
}

__slate_f80 __slate_f80_lgamma(__slate_f80 a) {
  return __slate_f80_store(lgammal(__slate_f80_load(a)));
}

int __slate_f80_fpclassify(__slate_f80 a) {
  return fpclassify(__slate_f80_load(a));
}

_Bool __slate_f80_isfinite(__slate_f80 a) {
  return isfinite(__slate_f80_load(a));
}

_Bool __slate_f80_isinf(__slate_f80 a) { return isinf(__slate_f80_load(a)); }

_Bool __slate_f80_isnan(__slate_f80 a) { return isnan(__slate_f80_load(a)); }

_Bool __slate_f80_isnormal(__slate_f80 a) {
  return isnormal(__slate_f80_load(a));
}

_Bool __slate_f80_is_fp_class(__slate_f80 a, int flags) {
  long double value    = __slate_f80_load(a);
  int         category = fpclassify(value);
  int         negative = signbit(value);
  return ((flags & 0x3) && category == FP_NAN) ||
         ((flags & 0x4) && category == FP_INFINITE && negative) ||
         ((flags & 0x8) && category == FP_NORMAL && negative) ||
         ((flags & 0x10) && category == FP_SUBNORMAL && negative) ||
         ((flags & 0x20) && category == FP_ZERO && negative) ||
         ((flags & 0x40) && category == FP_ZERO && !negative) ||
         ((flags & 0x80) && category == FP_SUBNORMAL && !negative) ||
         ((flags & 0x100) && category == FP_NORMAL && !negative) ||
         ((flags & 0x200) && category == FP_INFINITE && !negative);
}

_Bool __slate_f80_lt(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_load(a) < __slate_f80_load(b);
}

_Bool __slate_f80_le(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_load(a) <= __slate_f80_load(b);
}

_Bool __slate_f80_gt(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_load(a) > __slate_f80_load(b);
}

_Bool __slate_f80_ge(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_load(a) >= __slate_f80_load(b);
}

_Bool __slate_f80_eq(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_load(a) == __slate_f80_load(b);
}

_Bool __slate_f80_ne(__slate_f80 a, __slate_f80 b) {
  return __slate_f80_load(a) != __slate_f80_load(b);
}

_Bool __slate_f80_isunordered(__slate_f80 a, __slate_f80 b) {
  return isunordered(__slate_f80_load(a), __slate_f80_load(b));
}

int __slate_f80_radix(void) { return FLT_RADIX; }

int __slate_f80_mant_dig(void) { return LDBL_MANT_DIG; }

int __slate_f80_dig(void) { return LDBL_DIG; }

int __slate_f80_min_exp(void) { return LDBL_MIN_EXP; }

int __slate_f80_max_exp(void) { return LDBL_MAX_EXP; }

int __slate_f80_min_10_exp(void) { return LDBL_MIN_10_EXP; }

int __slate_f80_max_10_exp(void) { return LDBL_MAX_10_EXP; }

int __slate_f80_decimal_dig(void) {
#ifdef LDBL_DECIMAL_DIG
  return LDBL_DECIMAL_DIG;
#else
  return DECIMAL_DIG;
#endif
}

__slate_f80 __slate_f80_epsilon(void) {
  return __slate_f80_store(LDBL_EPSILON);
}

__slate_f80 __slate_f80_min(void) { return __slate_f80_store(LDBL_MIN); }

__slate_f80 __slate_f80_max(void) { return __slate_f80_store(LDBL_MAX); }

__slate_f80 __slate_f80_true_min(void) {
#ifdef LDBL_TRUE_MIN
  return __slate_f80_store(LDBL_TRUE_MIN);
#else
  return __slate_f80_store(ldexpl(1.0L, -16445));
#endif
}

int __slate_float_rounds(void) { return FLT_ROUNDS; }

int __slate_float_eval_method(void) { return FLT_EVAL_METHOD; }

__slate_f80 __slate_f80_infinity(void) { return __slate_f80_store(INFINITY); }

__slate_f80 __slate_f80_nan(const char *tag) {
  return __slate_f80_store(nanl(tag));
}

__slate_f80 __slate_f80_from_i8(int8_t a) {
  return __slate_f80_store((long double)a);
}
__slate_f80 __slate_f80_from_u8(uint8_t a) {
  return __slate_f80_store((long double)a);
}
__slate_f80 __slate_f80_from_i16(int16_t a) {
  return __slate_f80_store((long double)a);
}
__slate_f80 __slate_f80_from_u16(uint16_t a) {
  return __slate_f80_store((long double)a);
}
__slate_f80 __slate_f80_from_i32(int32_t a) {
  return __slate_f80_store((long double)a);
}
__slate_f80 __slate_f80_from_u32(uint32_t a) {
  return __slate_f80_store((long double)a);
}
__slate_f80 __slate_f80_from_i64(int64_t a) {
  return __slate_f80_store((long double)a);
}
__slate_f80 __slate_f80_from_u64(uint64_t a) {
  return __slate_f80_store((long double)a);
}
__slate_f80 __slate_f80_from_f32(float a) {
  return __slate_f80_store((long double)a);
}
__slate_f80 __slate_f80_from_f64(double a) {
  return __slate_f80_store((long double)a);
}
__slate_f80 __slate_f80_from_bool(_Bool a) {
  return __slate_f80_store((long double)a);
}

int8_t  __slate_f80_to_i8(__slate_f80 a) { return (int8_t)__slate_f80_load(a); }
uint8_t __slate_f80_to_u8(__slate_f80 a) {
  return (uint8_t)__slate_f80_load(a);
}
int16_t __slate_f80_to_i16(__slate_f80 a) {
  return (int16_t)__slate_f80_load(a);
}

uint16_t __slate_f80_to_u16(__slate_f80 a) {
  return (uint16_t)__slate_f80_load(a);
}

int32_t __slate_f80_to_i32(__slate_f80 a) {
  return (int32_t)__slate_f80_load(a);
}

uint32_t __slate_f80_to_u32(__slate_f80 a) {
  return (uint32_t)__slate_f80_load(a);
}

int64_t __slate_f80_to_i64(__slate_f80 a) {
  return (int64_t)__slate_f80_load(a);
}

uint64_t __slate_f80_to_u64(__slate_f80 a) {
  return (uint64_t)__slate_f80_load(a);
}

float __slate_f80_to_f32(__slate_f80 a) { return (float)__slate_f80_load(a); }

double __slate_f80_to_f64(__slate_f80 a) { return (double)__slate_f80_load(a); }

_Bool __slate_f80_to_bool(__slate_f80 a) { return __slate_f80_load(a) != 0.0L; }

_Bool __slate_f80_logical_not(__slate_f80 a) { return !__slate_f80_to_bool(a); }

_Bool __slate_cf80_logical_not(__slate_cf80 a) { return !__slate_cf80_load(a); }

__slate_f80 __slate_cf80_real(__slate_cf80 a) {
  return __slate_f80_store(creall(__slate_cf80_load(a)));
}

__slate_f80 __slate_cf80_imag(__slate_cf80 a) {
  return __slate_f80_store(cimagl(__slate_cf80_load(a)));
}

__slate_cf80 __slate_cf80_from_f80(__slate_f80 a) {
  return __slate_cf80_store(CMPLXL(__slate_f80_load(a), 0.0L));
}

__slate_f80 __slate_cf80_to_f80(__slate_cf80 a) {
  return __slate_f80_store(creall(__slate_cf80_load(a)));
}

float __slate_cf80_to_f32(__slate_cf80 a) { return __slate_f80_to_f32(a.re); }

double __slate_cf80_to_f64(__slate_cf80 a) { return __slate_f80_to_f64(a.re); }

int8_t __slate_cf80_to_i8(__slate_cf80 a) { return __slate_f80_to_i8(a.re); }

uint8_t __slate_cf80_to_u8(__slate_cf80 a) { return __slate_f80_to_u8(a.re); }

int16_t __slate_cf80_to_i16(__slate_cf80 a) { return __slate_f80_to_i16(a.re); }

uint16_t __slate_cf80_to_u16(__slate_cf80 a) {
  return __slate_f80_to_u16(a.re);
}

int32_t __slate_cf80_to_i32(__slate_cf80 a) { return __slate_f80_to_i32(a.re); }

uint32_t __slate_cf80_to_u32(__slate_cf80 a) {
  return __slate_f80_to_u32(a.re);
}

int64_t __slate_cf80_to_i64(__slate_cf80 a) { return __slate_f80_to_i64(a.re); }

uint64_t __slate_cf80_to_u64(__slate_cf80 a) {
  return __slate_f80_to_u64(a.re);
}

__slate_cf80 __slate_cf80_from_f32(float a) {
  return __slate_cf80_from_f80(__slate_f80_from_f32(a));
}

__slate_cf80 __slate_cf80_from_f64(double a) {
  return __slate_cf80_from_f80(__slate_f80_from_f64(a));
}
__slate_cf80 __slate_cf80_from_i8(int8_t a) {
  return __slate_cf80_from_f80(__slate_f80_from_i8(a));
}

__slate_cf80 __slate_cf80_from_u8(uint8_t a) {
  return __slate_cf80_from_f80(__slate_f80_from_u8(a));
}

__slate_cf80 __slate_cf80_from_i16(int16_t a) {
  return __slate_cf80_from_f80(__slate_f80_from_i16(a));
}

__slate_cf80 __slate_cf80_from_u16(uint16_t a) {
  return __slate_cf80_from_f80(__slate_f80_from_u16(a));
}

__slate_cf80 __slate_cf80_from_i32(int32_t a) {
  return __slate_cf80_from_f80(__slate_f80_from_i32(a));
}

__slate_cf80 __slate_cf80_from_u32(uint32_t a) {
  return __slate_cf80_from_f80(__slate_f80_from_u32(a));
}

__slate_cf80 __slate_cf80_from_i64(int64_t a) {
  return __slate_cf80_from_f80(__slate_f80_from_i64(a));
}

__slate_cf80 __slate_cf80_from_u64(uint64_t a) {
  return __slate_cf80_from_f80(__slate_f80_from_u64(a));
}

__slate_cf80 __slate_cf80_from_bool(_Bool a) {
  return __slate_cf80_from_f80(__slate_f80_from_bool(a));
}

__int128 __slate_f80_to_i128(__slate_f80 a) {
  return (__int128)__slate_f80_load(a);
}

unsigned __int128 __slate_f80_to_u128(__slate_f80 a) {
  return (unsigned __int128)__slate_f80_load(a);
}

__int128 __slate_cf80_to_i128(__slate_cf80 a) {
  return __slate_f80_to_i128(a.re);
}

unsigned __int128 __slate_cf80_to_u128(__slate_cf80 a) {
  return __slate_f80_to_u128(a.re);
}

__slate_f80 __slate_f80_from_i128(__int128 a) {
  return __slate_f80_store((long double)a);
}

__slate_f80 __slate_f80_from_u128(unsigned __int128 a) {
  return __slate_f80_store((long double)a);
}
