#define _GNU_SOURCE
#include <math.h>

double __slate_fenv_add_f64(double a, double b) { return a + b; }
float __slate_fenv_add_f32(float a, float b) { return a + b; }

double __slate_fenv_sub_f64(double a, double b) { return a - b; }
float __slate_fenv_sub_f32(float a, float b) { return a - b; }

double __slate_fenv_mul_f64(double a, double b) { return a * b; }
float __slate_fenv_mul_f32(float a, float b) { return a * b; }

double __slate_fenv_div_f64(double a, double b) { return a / b; }
float __slate_fenv_div_f32(float a, float b) { return a / b; }

double __slate_fenv_rem_f64(double a, double b) { return fmod(a, b); }
float __slate_fenv_rem_f32(float a, float b) { return fmodf(a, b); }

_Bool __slate_fenv_lt_f64(double a, double b) { return a < b; }
_Bool __slate_fenv_lt_f32(float a, float b) { return a < b; }

_Bool __slate_fenv_le_f64(double a, double b) { return a <= b; }
_Bool __slate_fenv_le_f32(float a, float b) { return a <= b; }

_Bool __slate_fenv_gt_f64(double a, double b) { return a > b; }
_Bool __slate_fenv_gt_f32(float a, float b) { return a > b; }

_Bool __slate_fenv_ge_f64(double a, double b) { return a >= b; }
_Bool __slate_fenv_ge_f32(float a, float b) { return a >= b; }

_Bool __slate_fenv_eq_f64(double a, double b) { return a == b; }
_Bool __slate_fenv_eq_f32(float a, float b) { return a == b; }

_Bool __slate_fenv_ne_f64(double a, double b) { return a != b; }
_Bool __slate_fenv_ne_f32(float a, float b) { return a != b; }

double __slate_fenv_i64_to_f64(long long a) { return (double)a; }
float __slate_fenv_i64_to_f32(long long a) { return (float)a; }

double __slate_fenv_u64_to_f64(unsigned long long a) { return (double)a; }
float __slate_fenv_u64_to_f32(unsigned long long a) { return (float)a; }

long long __slate_fenv_f64_to_i64(double a) { return (long long)a; }
long long __slate_fenv_f32_to_i64(float a) { return (long long)a; }

unsigned long long __slate_fenv_f64_to_u64(double a) { return (unsigned long long)a; }
unsigned long long __slate_fenv_f32_to_u64(float a) { return (unsigned long long)a; }

_Bool __slate_fenv_f64_to_bool(double a) { return a != 0.0; }
_Bool __slate_fenv_f32_to_bool(float a) { return a != 0.0f; }

double __slate_fenv_f32_to_f64(float a) { return (double)a; }
float __slate_fenv_f64_to_f32(double a) { return (float)a; }

double __slate_fenv_sin_f64(double a) { return sin(a); }
float __slate_fenv_sin_f32(float a) { return sinf(a); }

double __slate_fenv_cos_f64(double a) { return cos(a); }
float __slate_fenv_cos_f32(float a) { return cosf(a); }

double __slate_fenv_exp_f64(double a) { return exp(a); }
float __slate_fenv_exp_f32(float a) { return expf(a); }

double __slate_fenv_exp2_f64(double a) { return exp2(a); }
float __slate_fenv_exp2_f32(float a) { return exp2f(a); }

double __slate_fenv_log_f64(double a) { return log(a); }
float __slate_fenv_log_f32(float a) { return logf(a); }

double __slate_fenv_log2_f64(double a) { return log2(a); }
float __slate_fenv_log2_f32(float a) { return log2f(a); }

double __slate_fenv_log10_f64(double a) { return log10(a); }
float __slate_fenv_log10_f32(float a) { return log10f(a); }

double __slate_fenv_pow_f64(double a, double b) { return pow(a, b); }
float __slate_fenv_pow_f32(float a, float b) { return powf(a, b); }

double __slate_fenv_ceil_f64(double a) { return ceil(a); }
float __slate_fenv_ceil_f32(float a) { return ceilf(a); }

double __slate_fenv_floor_f64(double a) { return floor(a); }
float __slate_fenv_floor_f32(float a) { return floorf(a); }

double __slate_fenv_round_f64(double a) { return round(a); }
float __slate_fenv_round_f32(float a) { return roundf(a); }

double __slate_fenv_rint_f64(double a) { return rint(a); }
float __slate_fenv_rint_f32(float a) { return rintf(a); }

double __slate_fenv_nearbyint_f64(double a) { return nearbyint(a); }
float __slate_fenv_nearbyint_f32(float a) { return nearbyintf(a); }

double __slate_fenv_roundeven_f64(double a) { return roundeven(a); }
float __slate_fenv_roundeven_f32(float a) { return roundevenf(a); }

double __slate_fenv_trunc_f64(double a) { return trunc(a); }
float __slate_fenv_trunc_f32(float a) { return truncf(a); }

double __slate_fenv_sqrt_f64(double a) { return sqrt(a); }
float __slate_fenv_sqrt_f32(float a) { return sqrtf(a); }

double __slate_fenv_fabs_f64(double a) { return fabs(a); }
float __slate_fenv_fabs_f32(float a) { return fabsf(a); }

double __slate_fenv_fmax_f64(double a, double b) { return fmax(a, b); }
float __slate_fenv_fmax_f32(float a, float b) { return fmaxf(a, b); }

double __slate_fenv_fmin_f64(double a, double b) { return fmin(a, b); }
float __slate_fenv_fmin_f32(float a, float b) { return fminf(a, b); }

double __slate_fenv_copysign_f64(double a, double b) { return copysign(a, b); }
float __slate_fenv_copysign_f32(float a, float b) { return copysignf(a, b); }

double __slate_fenv_fma_f64(double a, double b, double c) { return fma(a, b, c); }
float __slate_fenv_fma_f32(float a, float b, float c) { return fmaf(a, b, c); }
