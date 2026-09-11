#define _GNU_SOURCE
#include <complex.h>
#include <fenv.h>
#include <math.h>
#include <stdio.h>

static int gnu_sincos_extensions(void) {
  double      sine     = 0.0;
  double      cosine   = 0.0;
  float       sine_f   = 0.0f;
  float       cosine_f = 0.0f;
  long double sine_l   = 0.0L;
  long double cosine_l = 0.0L;
  int         total    = 0;

  sincos(0.5, &sine, &cosine);
  sincosf(0.5f, &sine_f, &cosine_f);
  sincosl(0.5L, &sine_l, &cosine_l);

  total += fabs(sine - sin(0.5)) == 0.0;
  total += fabs(cosine - cos(0.5)) == 0.0;
  total += fabsf(sine_f - sinf(0.5f)) == 0.0f;
  total += fabsf(cosine_f - cosf(0.5f)) == 0.0f;
  total += fabsl(sine_l - sinl(0.5L)) == 0.0L;
  total += fabsl(cosine_l - cosl(0.5L)) == 0.0L;
  return total;
}

static int gnu_exponential_extensions(void) {
  int sign  = 0;
  int total = 0;

  total += exp10(2.0) == 100.0;
  total += exp10f(2.0f) == 100.0f;
  total += exp10l(2.0L) == 100.0L;
  total += lgamma_r(0.5, &sign) == lgamma(0.5);
  total += sign == 1;
  total += significand(12.0) == 1.5;
  total += significandf(12.0f) == 1.5f;
  total += drem(7.0, 3.0) == 1.0;
  total += dremf(7.0f, 3.0f) == 1.0f;
  total += scalb(1.5, 3.0) == 12.0;
  total += scalbf(1.5f, 3.0f) == 12.0f;
  return total;
}

static int gnu_bessel_extensions(void) {
  int total = 0;

  total += j0(1.0) == j0(1.0) && j0(1.0) > 0.7;
  total += j1(1.0) > 0.4;
  total += jn(2, 1.0) > 0.1;
  total += y0(1.0) > 0.08;
  total += y1(1.0) < 0.0;
  total += yn(2, 1.0) < 0.0;
  total += j0f(1.0f) > 0.7f;
  total += y0f(1.0f) > 0.08f;
  return total;
}

static int gnu_fenv_extensions(void) {
  int total = 0;

  total += feenableexcept(FE_DIVBYZERO) != -1;
  total += (fegetexcept() & FE_DIVBYZERO) != 0;
  total += fedisableexcept(FE_DIVBYZERO) != -1;
  total += (fegetexcept() & FE_DIVBYZERO) == 0;
  return total;
}

static int gnu_constant_extensions(void) {
  int total = 0;

  total += (double)M_PIl == M_PI;
  total += (double)M_El == M_E;
  total += (double)M_SQRT2l == M_SQRT2;
  total += (double)M_LN2l == M_LN2;
  return total;
}

int main(void) {
  printf("%d %d %d %d %d\n", gnu_sincos_extensions(),
         gnu_exponential_extensions(), gnu_bessel_extensions(),
         gnu_fenv_extensions(), gnu_constant_extensions());
  return 0;
}

// LOWERING: #![feature(f128)]

// REWRITES: #![feature(f128)]
