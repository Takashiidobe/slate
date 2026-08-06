#include <complex.h>
#include <stdio.h>

_Static_assert(__STDC_VERSION_COMPLEX_H__ == 202311L,
               "C23 complex header version");
_Static_assert(__STDC_IEC_559_COMPLEX__ == 1, "IEC 60559 complex arithmetic");

static void print_float_complex(const char *name, float complex value) {
  printf("%s %.9f %.9f\n", name, crealf(value), cimagf(value));
}

static void print_double_complex(const char *name, double complex value) {
  printf("%s %.12f %.12f\n", name, creal(value), cimag(value));
}

static void print_long_double_complex(const char         *name,
                                      long double complex value) {
  printf("%s %.12Lf %.12Lf\n", name, creall(value), cimagl(value));
}

static void print_float(const char *name, float value) {
  printf("%s %.9f\n", name, value);
}

static void print_double(const char *name, double value) {
  printf("%s %.12f\n", name, value);
}

static void print_long_double(const char *name, long double value) {
  printf("%s %.12Lf\n", name, value);
}

#define PRINT_COMPLEX_UNARY(name)                                              \
  print_float_complex(#name "f", name##f(zf));                                 \
  print_double_complex(#name, name(zd));                                       \
  print_long_double_complex(#name "l", name##l(zl))

#define PRINT_COMPLEX_BINARY(name)                                             \
  print_float_complex(#name "f", name##f(zf, ef));                             \
  print_double_complex(#name, name(zd, ed));                                   \
  print_long_double_complex(#name "l", name##l(zl, el))

#define PRINT_SCALAR_UNARY(name)                                               \
  print_float(#name "f", name##f(zf));                                         \
  print_double(#name, name(zd));                                               \
  print_long_double(#name "l", name##l(zl))

int main(void) {
  volatile float       rf  = 0.5f;
  volatile float       if_ = -0.25f;
  volatile double      rd  = 0.5;
  volatile double      id  = -0.25;
  volatile long double rl  = 0.5L;
  volatile long double il  = -0.25L;
  volatile float       ref = 1.25f;
  volatile float       ief = 0.5f;
  volatile double      red = 1.25;
  volatile double      ied = 0.5;
  volatile long double rel = 1.25L;
  volatile long double iel = 0.5L;

  float complex       zf = rf + if_ * I;
  double complex      zd = rd + id * _Complex_I;
  long double complex zl = rl + il * I;
  float complex       ef = ref + ief * I;
  double complex      ed = red + ied * I;
  long double complex el = rel + iel * I;

  print_float_complex("CMPLXF", CMPLXF(rf, if_));
  print_double_complex("CMPLX", CMPLX(rd, id));
  print_long_double_complex("CMPLXL", CMPLXL(rl, il));

  PRINT_COMPLEX_UNARY(cacos);
  PRINT_COMPLEX_UNARY(casin);
  PRINT_COMPLEX_UNARY(catan);
  PRINT_COMPLEX_UNARY(ccos);
  PRINT_COMPLEX_UNARY(csin);
  PRINT_COMPLEX_UNARY(ctan);
  PRINT_COMPLEX_UNARY(cacosh);
  PRINT_COMPLEX_UNARY(casinh);
  PRINT_COMPLEX_UNARY(catanh);
  PRINT_COMPLEX_UNARY(ccosh);
  PRINT_COMPLEX_UNARY(csinh);
  PRINT_COMPLEX_UNARY(ctanh);
  PRINT_COMPLEX_UNARY(cexp);
  PRINT_COMPLEX_UNARY(clog);
  PRINT_COMPLEX_BINARY(cpow);
  PRINT_COMPLEX_UNARY(csqrt);
  PRINT_SCALAR_UNARY(cabs);
  PRINT_SCALAR_UNARY(carg);
  PRINT_SCALAR_UNARY(cimag);
  PRINT_COMPLEX_UNARY(conj);
  PRINT_COMPLEX_UNARY(cproj);
  PRINT_SCALAR_UNARY(creal);
  return 0;
}
