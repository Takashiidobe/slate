#include <stdio.h>

static long double root(long double x) {
  long double r;
  __asm__("fsqrt" : "=t"(r) : "0"(x));
  return r;
}

static long double arctan2(long double y, long double x) {
  long double r;
  __asm__ __volatile__("fpatan" : "=t"(r) : "0"(x), "u"(y) : "st(1)");
  return r;
}

static void sincos(double x, double *s, double *c) {
  __asm__("fsincos" : "=t"(*c), "=u"(*s) : "0"(x));
}

static int below(long double a, long double b) {
  int flag;
  __asm__("fucomi %%st(1), %%st" : "=@ccb"(flag) : "t"(a), "u"(b));
  return flag;
}

static float square(float x) {
  float r;
  __asm__("fld %1\n\tfmul %%st(0), %%st" : "=&t"(r) : "f"(x));
  return r;
}

static long double sum3(long double a, long double b, long double c) {
  long double r;
  __asm__("fadd %2, %%st\n\tfadd %3, %%st" : "=t"(r) : "0"(a), "f"(c), "u"(b));
  return r;
}

static double from_int(int i) {
  double r;
  __asm__("fildl %1" : "=t"(r) : "m"(i));
  return r;
}

static long double scale(long double x) {
  __asm__("fadd %%st(0), %%st" : "+t"(x));
  return x;
}

static unsigned long long add_words(unsigned long long a, unsigned long long b) {
  __asm__("paddw %1, %0" : "=y"(a) : "y"(b), "0"(a));
  __asm__ __volatile__("emms");
  return a;
}

static int add_dwords(int a, int b) {
  __asm__("paddd %1, %0" : "+y"(a) : "y"(b));
  __asm__ __volatile__("emms");
  return a;
}

int main(void) {
  double s;
  double c;
  sincos(0.5, &s, &c);
  printf("%.12Lf\n", root(2.0L));
  printf("%.12Lf\n", arctan2(1.0L, 1.0L));
  printf("%.12f %.12f\n", s, c);
  printf("%d %d\n", below(1.0L, 2.0L), below(3.0L, 2.0L));
  printf("%.3f\n", square(1.5f));
  printf("%.3Lf\n", sum3(1.0L, 2.0L, 3.0L));
  printf("%.1f\n", from_int(-42));
  printf("%.3Lf\n", scale(2.25L));
  printf("%llx\n", add_words(0x0001000200030004ULL, 0x001000200030ffffULL));
  printf("%d\n", add_dwords(5, -7));
  return 0;
}
