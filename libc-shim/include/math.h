#ifndef _SLATE_MATH_H
#define _SLATE_MATH_H

#define HUGE_VAL (__builtin_huge_val())
#define HUGE_VALF (__builtin_huge_valf())
#define HUGE_VALL (__builtin_huge_vall())
#define INFINITY (__builtin_inff())
#define NAN (__builtin_nanf(""))

#define MATH_ERRNO 1
#define MATH_ERREXCEPT 2
#define math_errhandling (MATH_ERRNO | MATH_ERREXCEPT)

#define FP_NAN 0
#define FP_INFINITE 1
#define FP_ZERO 2
#define FP_SUBNORMAL 3
#define FP_NORMAL 4

#define fpclassify(x)                                                          \
  __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)
#define isfinite(x) __builtin_isfinite(x)
#define isinf(x) __builtin_isinf(x)
#define isnan(x) __builtin_isnan(x)
#define signbit(x) __builtin_signbit(x)

double acos(double x);
double asin(double x);
double atan(double x);
double atan2(double y, double x);
double cbrt(double x);
double ceil(double x);
long double ceill(long double x);
double copysign(double x, double y);
double cos(double x);
float cosf(float x);
double cosh(double x);
double exp(double x);
double exp2(double x);
double fabs(double x);
long double fabsl(long double x);
double fdim(double x, double y);
double floor(double x);
long double floorl(long double x);
double fma(double x, double y, double z);
double fmax(double x, double y);
double fmin(double x, double y);
double fmod(double x, double y);
float fmodf(float x, float y);
double frexp(double x, int *exp);
float frexpf(float x, int *exp);
double hypot(double x, double y);
int ilogb(double x);
double ldexp(double x, int exp);
long long llround(double x);
double log(double x);
double log2(double x);
double logb(double x);
long lrint(double x);
long lround(double x);
double modf(double x, double *iptr);
float modff(float x, float *iptr);
double nearbyint(double x);
double pow(double base, double exp);
double remainder(double x, double y);
double remquo(double x, double y, int *quo);
double rint(double x);
double round(double x);
long double roundl(long double x);
double scalbn(double x, int exp);
double sin(double x);
float sinf(float x);
double sinh(double x);
double sqrt(double x);
float sqrtf(float x);
double tan(double x);
double tanh(double x);
double trunc(double x);
long double truncl(long double x);

#endif
