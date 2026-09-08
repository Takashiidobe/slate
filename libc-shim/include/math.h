#ifndef _SLATE_MATH_H
#define _SLATE_MATH_H

#include <features.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
#define __STDC_VERSION_MATH_H__ 202311L
#endif

#if defined(__SLATE_LIBC_MSVC)

#include <bits/msvc/math.h>

#else

#define __NEED_float_t
#define __NEED_double_t
#include <bits/types.h>

#if 100 * __GNUC__ + __GNUC_MINOR__ >= 303
#define NAN      __builtin_nanf("")
#define INFINITY __builtin_inff()
#else
#define NAN      (0.0f / 0.0f)
#define INFINITY 1e5000f
#endif

#define HUGE_VAL  (__builtin_huge_val())
#define HUGE_VALF (__builtin_huge_valf())
#define HUGE_VALL (__builtin_huge_vall())

#define MATH_ERRNO       1
#define MATH_ERREXCEPT   2
#define math_errhandling (MATH_ERRNO | MATH_ERREXCEPT)

#define FP_ILOGBNAN (-1 - 0x7fffffff)
#define FP_ILOGB0   FP_ILOGBNAN

#define FP_INT_DOWNWARD 0
#define FP_INT_TONEAREST 1
#define FP_INT_TONEARESTFROMZERO 4
#define FP_INT_TOWARDZERO 3
#define FP_INT_UPWARD 2
#define FP_LLOGB0 FP_ILOGB0
#define FP_LLOGBNAN FP_ILOGBNAN

#ifdef __FP_FAST_FMA
#define FP_FAST_FMA 1
#endif

#ifdef __FP_FAST_FMAF
#define FP_FAST_FMAF 1
#endif

#ifdef __FP_FAST_FMAL
#define FP_FAST_FMAL 1
#endif

#define FP_NAN 0
#define FP_INFINITE 1
#define FP_ZERO 2
#define FP_SUBNORMAL 3
#define FP_NORMAL 4

#define fpclassify(x)                                                          \
  __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)
#define isfinite(x)          __builtin_isfinite(x)
#define isinf(x)             __builtin_isinf(x)
#define isnan(x)             __builtin_isnan(x)
#define isnormal(x)          __builtin_isnormal(x)
#define signbit(x)           __builtin_signbit(x)
#define isless(x, y)         __builtin_isless(x, y)
#define islessequal(x, y)    __builtin_islessequal(x, y)
#define islessgreater(x, y)  __builtin_islessgreater(x, y)
#define isgreater(x, y)      __builtin_isgreater(x, y)
#define isgreaterequal(x, y) __builtin_isgreaterequal(x, y)
#define isunordered(x, y)    __builtin_isunordered(x, y)
#define iscanonical(x)       1
#define iseqsig(x, y)        ((x) == (y))
#define issignaling(x)       0
#define issubnormal(x)       0
#define iszero(x)            ((x) == 0)

#if defined(__SLATE_LIBC_GLIBC)
typedef long double long_double_t;
#endif

double      acos(double);
float       acosf(float);
long double acosl(long double);

int (isinf)(double);
int (isnan)(double);

double      acosh(double);
float       acoshf(float);
long double acoshl(long double);

double      asin(double);
float       asinf(float);
long double asinl(long double);

double      asinh(double);
float       asinhf(float);
long double asinhl(long double);

double      atan(double);
float       atanf(float);
long double atanl(long double);

double      atan2(double, double);
float       atan2f(float, float);
long double atan2l(long double, long double);

double      atanh(double);
float       atanhf(float);
long double atanhl(long double);

int canonicalize(double *, const double *);
int canonicalizef(float *, const float *);
int canonicalizel(long double *, const long double *);

double      cbrt(double);
float       cbrtf(float);
long double cbrtl(long double);

double      ceil(double);
float       ceilf(float);
long double ceill(long double);

double      copysign(double, double);
float       copysignf(float, float);
long double copysignl(long double, long double);

double      cos(double);
float       cosf(float);
long double cosl(long double);

double      cosh(double);
float       coshf(float);
long double coshl(long double);

double      erf(double);
float       erff(float);
long double erfl(long double);

double      erfc(double);
float       erfcf(float);
long double erfcl(long double);

double      exp(double);
float       expf(float);
long double expl(long double);

double      exp2(double);
float       exp2f(float);
long double exp2l(long double);

double      expm1(double);
float       expm1f(float);
long double expm1l(long double);

double      fabs(double);
float       fabsf(float);
long double fabsl(long double);

double      fdim(double, double);
float       fdimf(float, float);
long double fdiml(long double, long double);

double      floor(double);
float       floorf(float);
long double floorl(long double);

double      fma(double, double, double);
float       fmaf(float, float, float);
long double fmal(long double, long double, long double);

double      fmax(double, double);
float       fmaxf(float, float);
long double fmaxl(long double, long double);

double      fmin(double, double);
float       fminf(float, float);
long double fminl(long double, long double);

double      fmod(double, double);
float       fmodf(float, float);
long double fmodl(long double, long double);

double      frexp(double, int *);
float       frexpf(float, int *);
long double frexpl(long double, int *);

double      hypot(double, double);
float       hypotf(float, float);
long double hypotl(long double, long double);

int ilogb(double);
int ilogbf(float);
int ilogbl(long double);

double      ldexp(double, int);
float       ldexpf(float, int);
long double ldexpl(long double, int);

double      lgamma(double);
float       lgammaf(float);
long double lgammal(long double);

long long llrint(double);
long long llrintf(float);
long long llrintl(long double);

long long llround(double);
long long llroundf(float);
long long llroundl(long double);

double      log(double);
float       logf(float);
long double logl(long double);

double      log10(double);
float       log10f(float);
long double log10l(long double);

double      log1p(double);
float       log1pf(float);
long double log1pl(long double);

double      log2(double);
float       log2f(float);
long double log2l(long double);

double      logb(double);
float       logbf(float);
long double logbl(long double);

long lrint(double);
long lrintf(float);
long lrintl(long double);

long lround(double);
long lroundf(float);
long lroundl(long double);

double      modf(double, double *);
float       modff(float, float *);
long double modfl(long double, long double *);

double      nan(const char *);
float       nanf(const char *);
long double nanl(const char *);

double      nearbyint(double);
float       nearbyintf(float);
long double nearbyintl(long double);

double      nextafter(double, double);
float       nextafterf(float, float);
long double nextafterl(long double, long double);

double      nexttoward(double, long double);
float       nexttowardf(float, long double);
long double nexttowardl(long double, long double);

double      pow(double, double);
float       powf(float, float);
long double powl(long double, long double);

double      remainder(double, double);
float       remainderf(float, float);
long double remainderl(long double, long double);

double      remquo(double, double, int *);
float       remquof(float, float, int *);
long double remquol(long double, long double, int *);

double      rint(double);
float       rintf(float);
long double rintl(long double);

double      round(double);
float       roundf(float);
long double roundl(long double);

double      scalbln(double, long);
float       scalblnf(float, long);
long double scalblnl(long double, long);

double      scalbn(double, int);
float       scalbnf(float, int);
long double scalbnl(long double, int);

double      sin(double);
float       sinf(float);
long double sinl(long double);

double      sinh(double);
float       sinhf(float);
long double sinhl(long double);

double      sqrt(double);
float       sqrtf(float);
long double sqrtl(long double);

double      tan(double);
float       tanf(float);
long double tanl(long double);

double      tanh(double);
float       tanhf(float);
long double tanhl(long double);

double      tgamma(double);
float       tgammaf(float);
long double tgammal(long double);

double      trunc(double);
float       truncf(float);
long double truncl(long double);

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#undef MAXFLOAT
#define MAXFLOAT 3.40282346638528859812e+38F
#endif

#if defined(_XOPEN_SOURCE) || defined(_GNU_SOURCE) || defined(_BSD_SOURCE)
#define M_E        2.7182818284590452354  /* e */
#define M_LOG2E    1.4426950408889634074  /* log_2 e */
#define M_LOG10E   0.43429448190325182765 /* log_10 e */
#define M_LN2      0.69314718055994530942 /* log_e 2 */
#define M_LN10     2.30258509299404568402 /* log_e 10 */
#define M_PI       3.14159265358979323846 /* pi */
#define M_PI_2     1.57079632679489661923 /* pi/2 */
#define M_PI_4     0.78539816339744830962 /* pi/4 */
#define M_1_PI     0.31830988618379067154 /* 1/pi */
#define M_2_PI     0.63661977236758134308 /* 2/pi */
#define M_2_SQRTPI 1.12837916709551257390 /* 2/sqrt(pi) */
#define M_SQRT2    1.41421356237309504880 /* sqrt(2) */
#define M_SQRT1_2  0.70710678118654752440 /* 1/sqrt(2) */

#ifdef _GNU_SOURCE
#define M_El        2.718281828459045235360287471352662498L /* e */
#define M_LOG2El    1.442695040888963407359924681001892137L /* log_2 e */
#define M_LOG10El   0.434294481903251827651128918916605082L /* log_10 e */
#define M_LN2l      0.693147180559945309417232121458176568L /* log_e 2 */
#define M_LN10l     2.302585092994045684017991454684364208L /* log_e 10 */
#define M_PIl       3.141592653589793238462643383279502884L /* pi */
#define M_PI_2l     1.570796326794896619231321691639751442L /* pi/2 */
#define M_PI_4l     0.785398163397448309615660845819875721L /* pi/4 */
#define M_1_PIl     0.318309886183790671537767526745028724L /* 1/pi */
#define M_2_PIl     0.636619772367581343075535053490057448L /* 2/pi */
#define M_2_SQRTPIl 1.128379167095512573896158903121545172L /* 2/sqrt(pi) */
#define M_SQRT2l    1.414213562373095048801688724209698079L /* sqrt(2) */
#define M_SQRT1_2l  0.707106781186547524400844362104849039L /* 1/sqrt(2) */
#endif

#if defined(__SLATE_LIBC_GLIBC)
#define M_Ef M_E
#define M_Ef32 M_E
#define M_Ef32x M_E
#define M_Ef64 M_E
#define M_Ef64x M_E
#define M_Ef128 M_E
#define M_LOG2Ef M_LOG2E
#define M_LOG2Ef32 M_LOG2E
#define M_LOG2Ef32x M_LOG2E
#define M_LOG2Ef64 M_LOG2E
#define M_LOG2Ef64x M_LOG2E
#define M_LOG2Ef128 M_LOG2E
#define M_LOG10Ef M_LOG10E
#define M_LOG10Ef32 M_LOG10E
#define M_LOG10Ef32x M_LOG10E
#define M_LOG10Ef64 M_LOG10E
#define M_LOG10Ef64x M_LOG10E
#define M_LOG10Ef128 M_LOG10E
#define M_LN2f M_LN2
#define M_LN2f32 M_LN2
#define M_LN2f32x M_LN2
#define M_LN2f64 M_LN2
#define M_LN2f64x M_LN2
#define M_LN2f128 M_LN2
#define M_LN10f M_LN10
#define M_LN10f32 M_LN10
#define M_LN10f32x M_LN10
#define M_LN10f64 M_LN10
#define M_LN10f64x M_LN10
#define M_LN10f128 M_LN10
#define M_PIf M_PI
#define M_PIf32 M_PI
#define M_PIf32x M_PI
#define M_PIf64 M_PI
#define M_PIf64x M_PI
#define M_PIf128 M_PI
#define M_PI_2f M_PI_2
#define M_PI_2f32 M_PI_2
#define M_PI_2f32x M_PI_2
#define M_PI_2f64 M_PI_2
#define M_PI_2f64x M_PI_2
#define M_PI_2f128 M_PI_2
#define M_PI_4f M_PI_4
#define M_PI_4f32 M_PI_4
#define M_PI_4f32x M_PI_4
#define M_PI_4f64 M_PI_4
#define M_PI_4f64x M_PI_4
#define M_PI_4f128 M_PI_4
#define M_1_PIf M_1_PI
#define M_1_PIf32 M_1_PI
#define M_1_PIf32x M_1_PI
#define M_1_PIf64 M_1_PI
#define M_1_PIf64x M_1_PI
#define M_1_PIf128 M_1_PI
#define M_2_PIf M_2_PI
#define M_2_PIf32 M_2_PI
#define M_2_PIf32x M_2_PI
#define M_2_PIf64 M_2_PI
#define M_2_PIf64x M_2_PI
#define M_2_PIf128 M_2_PI
#define M_2_SQRTPIf M_2_SQRTPI
#define M_2_SQRTPIf32 M_2_SQRTPI
#define M_2_SQRTPIf32x M_2_SQRTPI
#define M_2_SQRTPIf64 M_2_SQRTPI
#define M_2_SQRTPIf64x M_2_SQRTPI
#define M_2_SQRTPIf128 M_2_SQRTPI
#define M_SQRT2f M_SQRT2
#define M_SQRT2f32 M_SQRT2
#define M_SQRT2f32x M_SQRT2
#define M_SQRT2f64 M_SQRT2
#define M_SQRT2f64x M_SQRT2
#define M_SQRT2f128 M_SQRT2
#define M_SQRT1_2f M_SQRT1_2
#define M_SQRT1_2f32 M_SQRT1_2
#define M_SQRT1_2f32x M_SQRT1_2
#define M_SQRT1_2f64 M_SQRT1_2
#define M_SQRT1_2f64x M_SQRT1_2
#define M_SQRT1_2f128 M_SQRT1_2
#define HUGE_VAL_F32 HUGE_VALF
#define HUGE_VAL_F64 HUGE_VAL
#define HUGE_VAL_F32X HUGE_VAL
#define HUGE_VAL_F64X HUGE_VAL
#define HUGE_VAL_F128 HUGE_VALL
#define SNAN 0.0
#define SNANF 0.0F
#define SNANL 0.0L
#define SNANF32 0.0F
#define SNANF32X 0.0F
#define SNANF64 0.0
#define SNANF64X 0.0
#define SNANF128 0.0L
#endif

extern int signgam;

double j0(double);
double j1(double);
double jn(int, double);

double y0(double);
double y1(double);
double yn(int, double);
#endif

#if defined(_GNU_SOURCE) || defined(_XOPEN_SOURCE) || defined(_BSD_SOURCE)
#define HUGE 3.40282346638528859812e+38F

double drem(double, double);
float  dremf(float, float);

int finite(double);
int finitef(float);

double scalb(double, double);
float  scalbf(float, float);

double significand(double);
float  significandf(float);

double lgamma_r(double, int *);
float  lgammaf_r(float, int *);

float j0f(float);
float j1f(float);
float jnf(int, float);

float y0f(float);
float y1f(float);
float ynf(int, float);
#endif

#ifdef _GNU_SOURCE
long double lgammal_r(long double, int *);

void sincos(double, double *, double *);
void sincosf(float, float *, float *);
void sincosl(long double, long double *, long double *);

double      exp10(double);
float       exp10f(float);
long double exp10l(long double);

// glibc 2.27 obsoleted pow10/pow10f/pow10l, every other profile uses it
#if !defined(__SLATE_LIBC_GLIBC) || __SLATE_GLIBC_MINOR__ < 27
double      pow10(double);
float       pow10f(float);
long double pow10l(long double);
#endif
#endif

#if defined(__SLATE_LIBC_GLIBC)
double acospi(double);
double asinpi(double);
double atan2pi(double, double);
double atanpi(double);
double compoundn(double, long long);
double cospi(double);
double exp10m1(double);
double exp2m1(double);
double fmaximum(double, double);
double fmaximum_mag(double, double);
double fmaximum_mag_num(double, double);
double fmaximum_num(double, double);
double fmaxmag(double, double);
double fminimum(double, double);
double fminimum_mag(double, double);
double fminimum_mag_num(double, double);
double fminimum_num(double, double);
double fminmag(double, double);
#if defined(__SLATE_ARCH_ARM)
long long fromfp(double, int, unsigned int);
long long fromfpx(double, int, unsigned int);
unsigned long long ufromfp(double, int, unsigned int);
unsigned long long ufromfpx(double, int, unsigned int);
#else
double fromfp(double, int, unsigned int);
double fromfpx(double, int, unsigned int);
double ufromfp(double, int, unsigned int);
double ufromfpx(double, int, unsigned int);
#endif
double gamma(double);
double getpayload(const double *);
long llogb(double);
double log10p1(double);
double log2p1(double);
double logp1(double);
double nextdown(double);
double nextup(double);
double pown(double, long long);
double powr(double, double);
double rootn(double, long long);
double roundeven(double);
double rsqrt(double);
int setpayload(double *, double);
int setpayloadsig(double *, double);
double sinpi(double);
double tanpi(double);
int totalorder(const double *, const double *);
int totalordermag(const double *, const double *);
#endif

#endif

#endif
