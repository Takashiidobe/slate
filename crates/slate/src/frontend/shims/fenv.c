#define _GNU_SOURCE
#include <math.h>
#include <fenv.h>

#if defined(__clang__)
#pragma STDC FENV_ACCESS ON
#endif
#if defined(_MSC_VER) && !defined(__clang__)
#define SLATE_FENV __declspec(noinline)
#else
#define SLATE_FENV __attribute__((noinline))
#endif

SLATE_FENV int __slate_fenv_flt_rounds(void) {
  switch (fegetround()) {
  case FE_TOWARDZERO: return 0;
  case FE_TONEAREST: return 1;
  case FE_UPWARD: return 2;
  case FE_DOWNWARD: return 3;
  default: return -1;
  }
}

SLATE_FENV double __slate_fenv_add_f64(double a, double b) { return a + b; }
SLATE_FENV float __slate_fenv_add_f32(float a, float b) { return a + b; }

SLATE_FENV double __slate_fenv_sub_f64(double a, double b) { return a - b; }
SLATE_FENV float __slate_fenv_sub_f32(float a, float b) { return a - b; }

SLATE_FENV double __slate_fenv_mul_f64(double a, double b) { return a * b; }
SLATE_FENV float __slate_fenv_mul_f32(float a, float b) { return a * b; }

SLATE_FENV double __slate_fenv_div_f64(double a, double b) { return a / b; }
SLATE_FENV float __slate_fenv_div_f32(float a, float b) { return a / b; }

SLATE_FENV double __slate_fenv_rem_f64(double a, double b) { return fmod(a, b); }
SLATE_FENV float __slate_fenv_rem_f32(float a, float b) { return fmodf(a, b); }

SLATE_FENV _Bool __slate_fenv_lt_f64(double a, double b) { return a < b; }
SLATE_FENV _Bool __slate_fenv_lt_f32(float a, float b) { return a < b; }

SLATE_FENV _Bool __slate_fenv_le_f64(double a, double b) { return a <= b; }
SLATE_FENV _Bool __slate_fenv_le_f32(float a, float b) { return a <= b; }

SLATE_FENV _Bool __slate_fenv_gt_f64(double a, double b) { return a > b; }
SLATE_FENV _Bool __slate_fenv_gt_f32(float a, float b) { return a > b; }

SLATE_FENV _Bool __slate_fenv_ge_f64(double a, double b) { return a >= b; }
SLATE_FENV _Bool __slate_fenv_ge_f32(float a, float b) { return a >= b; }

SLATE_FENV _Bool __slate_fenv_eq_f64(double a, double b) { return a == b; }
SLATE_FENV _Bool __slate_fenv_eq_f32(float a, float b) { return a == b; }

SLATE_FENV _Bool __slate_fenv_ne_f64(double a, double b) { return a != b; }
SLATE_FENV _Bool __slate_fenv_ne_f32(float a, float b) { return a != b; }

SLATE_FENV double __slate_fenv_i64_to_f64(long long a) { return (double)a; }
SLATE_FENV float __slate_fenv_i64_to_f32(long long a) { return (float)a; }

SLATE_FENV double __slate_fenv_u64_to_f64(unsigned long long a) {
  if ((long long)a >= 0) return (double)(long long)a;
  return (double)(long long)((a >> 1) | (a & 1)) * 2.0;
}
SLATE_FENV float __slate_fenv_u64_to_f32(unsigned long long a) {
  if ((long long)a >= 0) return (float)(long long)a;
  return (float)(long long)((a >> 1) | (a & 1)) * 2.0f;
}

SLATE_FENV long long __slate_fenv_f64_to_i64(double a) { return (long long)a; }
SLATE_FENV long long __slate_fenv_f32_to_i64(float a) { return (long long)a; }

SLATE_FENV unsigned long long __slate_fenv_f64_to_u64(double a) { return (unsigned long long)a; }
SLATE_FENV unsigned long long __slate_fenv_f32_to_u64(float a) { return (unsigned long long)a; }

SLATE_FENV _Bool __slate_fenv_f64_to_bool(double a) { return a != 0.0; }
SLATE_FENV _Bool __slate_fenv_f32_to_bool(float a) { return a != 0.0f; }

SLATE_FENV double __slate_fenv_f32_to_f64(float a) { return (double)a; }
SLATE_FENV float __slate_fenv_f64_to_f32(double a) { return (float)a; }

SLATE_FENV double __slate_fenv_sin_f64(double a) { return sin(a); }
SLATE_FENV float __slate_fenv_sin_f32(float a) { return sinf(a); }

SLATE_FENV double __slate_fenv_cos_f64(double a) { return cos(a); }
SLATE_FENV float __slate_fenv_cos_f32(float a) { return cosf(a); }

SLATE_FENV double __slate_fenv_exp_f64(double a) { return exp(a); }
SLATE_FENV float __slate_fenv_exp_f32(float a) { return expf(a); }

SLATE_FENV double __slate_fenv_exp2_f64(double a) { return exp2(a); }
SLATE_FENV float __slate_fenv_exp2_f32(float a) { return exp2f(a); }

SLATE_FENV double __slate_fenv_log_f64(double a) { return log(a); }
SLATE_FENV float __slate_fenv_log_f32(float a) { return logf(a); }

SLATE_FENV double __slate_fenv_log2_f64(double a) { return log2(a); }
SLATE_FENV float __slate_fenv_log2_f32(float a) { return log2f(a); }

SLATE_FENV double __slate_fenv_log10_f64(double a) { return log10(a); }
SLATE_FENV float __slate_fenv_log10_f32(float a) { return log10f(a); }

SLATE_FENV double __slate_fenv_pow_f64(double a, double b) { return pow(a, b); }
SLATE_FENV float __slate_fenv_pow_f32(float a, float b) { return powf(a, b); }

SLATE_FENV double __slate_fenv_ceil_f64(double a) { return ceil(a); }
SLATE_FENV float __slate_fenv_ceil_f32(float a) { return ceilf(a); }

SLATE_FENV double __slate_fenv_floor_f64(double a) { return floor(a); }
SLATE_FENV float __slate_fenv_floor_f32(float a) { return floorf(a); }

SLATE_FENV double __slate_fenv_round_f64(double a) { return round(a); }
SLATE_FENV float __slate_fenv_round_f32(float a) { return roundf(a); }

SLATE_FENV double __slate_fenv_rint_f64(double a) { return rint(a); }
SLATE_FENV float __slate_fenv_rint_f32(float a) { return rintf(a); }

SLATE_FENV double __slate_fenv_nearbyint_f64(double a) { return nearbyint(a); }
SLATE_FENV float __slate_fenv_nearbyint_f32(float a) { return nearbyintf(a); }

SLATE_FENV double __slate_fenv_roundeven_f64(double a) { return roundeven(a); }
SLATE_FENV float __slate_fenv_roundeven_f32(float a) { return roundevenf(a); }

SLATE_FENV double __slate_fenv_trunc_f64(double a) { return trunc(a); }
SLATE_FENV float __slate_fenv_trunc_f32(float a) { return truncf(a); }

SLATE_FENV double __slate_fenv_sqrt_f64(double a) { return sqrt(a); }
SLATE_FENV float __slate_fenv_sqrt_f32(float a) { return sqrtf(a); }

SLATE_FENV double __slate_fenv_fabs_f64(double a) { return fabs(a); }
SLATE_FENV float __slate_fenv_fabs_f32(float a) { return fabsf(a); }

SLATE_FENV double __slate_fenv_fmax_f64(double a, double b) { return fmax(a, b); }
SLATE_FENV float __slate_fenv_fmax_f32(float a, float b) { return fmaxf(a, b); }

SLATE_FENV double __slate_fenv_fmin_f64(double a, double b) { return fmin(a, b); }
SLATE_FENV float __slate_fenv_fmin_f32(float a, float b) { return fminf(a, b); }

SLATE_FENV double __slate_fenv_copysign_f64(double a, double b) { return copysign(a, b); }
SLATE_FENV float __slate_fenv_copysign_f32(float a, float b) { return copysignf(a, b); }

SLATE_FENV double __slate_fenv_fma_f64(double a, double b, double c) { return fma(a, b, c); }
SLATE_FENV float __slate_fenv_fma_f32(float a, float b, float c) { return fmaf(a, b, c); }

#if defined(__clang__)
#pragma STDC FENV_ACCESS DEFAULT
#endif
