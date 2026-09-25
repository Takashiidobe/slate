/* PR target/92904 */

#include <stdarg.h>

struct S {
  long long a, b;
};
struct __attribute__((aligned(16))) T {
  long long a, b;
};
struct U {
  double a, b, c, d;
};
struct __attribute__((aligned(32))) V {
  double a, b, c, d;
};
struct W {
  double    a;
  long long b;
};
struct __attribute__((aligned(16))) X {
  double    a;
  long long b;
};
#if __SIZEOF_INT128__ == 2 * __SIZEOF_LONG_LONG__
__int128 b;
#endif
struct S c;
struct T d;
struct U e;
struct V f;
struct W g;
struct X h;

#if __SIZEOF_INT128__ == 2 * __SIZEOF_LONG_LONG__
__attribute__((noipa)) __int128 f1(int x, ...) {
  __int128 r;
  va_list  ap;
  va_start(ap, x);
  while (x--)
    va_arg(ap, int);
  r = va_arg(ap, __int128);
  va_end(ap);
  return r;
}
#endif

__attribute__((noipa)) struct S f2(int x, ...) {
  struct S r;
  va_list  ap;
  va_start(ap, x);
  while (x--)
    va_arg(ap, int);
  r = va_arg(ap, struct S);
  va_end(ap);
  return r;
}

__attribute__((noipa)) struct T f3(int x, ...) {
  struct T r;
  va_list  ap;
  va_start(ap, x);
  while (x--)
    va_arg(ap, int);
  r = va_arg(ap, struct T);
  va_end(ap);
  return r;
}

#if __SIZEOF_INT128__ == 2 * __SIZEOF_LONG_LONG__
__attribute__((noipa)) void f4(int x, ...) {
  va_list ap;
  va_start(ap, x);
  while (x--)
    va_arg(ap, int);
  b = va_arg(ap, __int128);
  va_end(ap);
}
#endif

__attribute__((noipa)) void f5(int x, ...) {
  va_list ap;
  va_start(ap, x);
  while (x--)
    va_arg(ap, int);
  c = va_arg(ap, struct S);
  va_end(ap);
}

__attribute__((noipa)) void f6(int x, ...) {
  va_list ap;
  va_start(ap, x);
  while (x--)
    va_arg(ap, int);
  d = va_arg(ap, struct T);
  va_end(ap);
}

__attribute__((noipa)) struct U f7(int x, ...) {
  struct U r;
  va_list  ap;
  va_start(ap, x);
  while (x--)
    va_arg(ap, double);
  r = va_arg(ap, struct U);
  va_end(ap);
  return r;
}

__attribute__((noipa)) struct V f8(int x, ...) {
  struct V r;
  va_list  ap;
  va_start(ap, x);
  while (x--)
    va_arg(ap, double);
  r = va_arg(ap, struct V);
  va_end(ap);
  return r;
}

__attribute__((noipa)) void f9(int x, ...) {
  va_list ap;
  va_start(ap, x);
  while (x--)
    va_arg(ap, double);
  e = va_arg(ap, struct U);
  va_end(ap);
}

__attribute__((noipa)) void f10(int x, ...) {
  va_list ap;
  va_start(ap, x);
  while (x--)
    va_arg(ap, double);
  f = va_arg(ap, struct V);
  va_end(ap);
}

__attribute__((noipa)) struct W f11(int x, ...) {
  struct W r;
  va_list  ap;
  va_start(ap, x);
  while (x--) {
    va_arg(ap, int);
    va_arg(ap, double);
  }
  r = va_arg(ap, struct W);
  va_end(ap);
  return r;
}

__attribute__((noipa)) struct X f12(int x, ...) {
  struct X r;
  va_list  ap;
  va_start(ap, x);
  while (x--) {
    va_arg(ap, int);
    va_arg(ap, double);
  }
  r = va_arg(ap, struct X);
  va_end(ap);
  return r;
}

__attribute__((noipa)) void f13(int x, ...) {
  va_list ap;
  va_start(ap, x);
  while (x--) {
    va_arg(ap, int);
    va_arg(ap, double);
  }
  g = va_arg(ap, struct W);
  va_end(ap);
}

__attribute__((noipa)) void f14(int x, ...) {
  va_list ap;
  va_start(ap, x);
  while (x--) {
    va_arg(ap, int);
    va_arg(ap, double);
  }
  h = va_arg(ap, struct X);
  va_end(ap);
}

int main() {
  union Y {
#if __SIZEOF_INT128__ == 2 * __SIZEOF_LONG_LONG__
    __int128 b;
#endif
    struct S c;
    struct T d;
    struct U e;
    struct V f;
    struct W g;
    struct X h;
  } u, v;
  u.c.a = 0x5555555555555555ULL;
  u.c.b = 0xaaaaaaaaaaaaaaaaULL;
#define C(x)                                                                   \
  do {                                                                         \
    if (u.c.a != x.c.a || u.c.b != x.c.b)                                      \
      __builtin_abort();                                                       \
    u.c.a++;                                                                   \
    u.c.b--;                                                                   \
  } while (0)
#if __SIZEOF_INT128__ == 2 * __SIZEOF_LONG_LONG__
  v.b = f1(0, u.b);
  C(v);
  v.b = f1(1, 0, u.b);
  C(v);
  v.b = f1(2, 0, 0, u.b);
  C(v);
  v.b = f1(3, 0, 0, 0, u.b);
  C(v);
  v.b = f1(4, 0, 0, 0, 0, u.b);
  C(v);
  v.b = f1(5, 0, 0, 0, 0, 0, u.b);
  C(v);
  v.b = f1(6, 0, 0, 0, 0, 0, 0, u.b);
  C(v);
  v.b = f1(7, 0, 0, 0, 0, 0, 0, 0, u.b);
  C(v);
  v.b = f1(8, 0, 0, 0, 0, 0, 0, 0, 0, u.b);
  C(v);
  v.b = f1(9, 0, 0, 0, 0, 0, 0, 0, 0, 0, u.b);
  C(v);
#endif
  v.c = f2(0, u.c);
  C(v);
  v.c = f2(1, 0, u.c);
  C(v);
  v.c = f2(2, 0, 0, u.c);
  C(v);
  v.c = f2(3, 0, 0, 0, u.c);
  C(v);
  v.c = f2(4, 0, 0, 0, 0, u.c);
  C(v);
  v.c = f2(5, 0, 0, 0, 0, 0, u.c);
  C(v);
  v.c = f2(6, 0, 0, 0, 0, 0, 0, u.c);
  C(v);
  v.c = f2(7, 0, 0, 0, 0, 0, 0, 0, u.c);
  C(v);
  v.c = f2(8, 0, 0, 0, 0, 0, 0, 0, 0, u.c);
  C(v);
  v.c = f2(9, 0, 0, 0, 0, 0, 0, 0, 0, 0, u.c);
  C(v);
  v.d = f3(0, u.d);
  C(v);
  v.d = f3(1, 0, u.d);
  C(v);
  v.d = f3(2, 0, 0, u.d);
  C(v);
  v.d = f3(3, 0, 0, 0, u.d);
  C(v);
  v.d = f3(4, 0, 0, 0, 0, u.d);
  C(v);
  v.d = f3(5, 0, 0, 0, 0, 0, u.d);
  C(v);
  v.d = f3(6, 0, 0, 0, 0, 0, 0, u.d);
  C(v);
  v.d = f3(7, 0, 0, 0, 0, 0, 0, 0, u.d);
  C(v);
  v.d = f3(8, 0, 0, 0, 0, 0, 0, 0, 0, u.d);
  C(v);
  v.d = f3(9, 0, 0, 0, 0, 0, 0, 0, 0, 0, u.d);
  C(v);
#if __SIZEOF_INT128__ == 2 * __SIZEOF_LONG_LONG__
  f4(0, u.b);
  v.b = b;
  C(v);
  f4(1, 0, u.b);
  v.b = b;
  C(v);
  f4(2, 0, 0, u.b);
  v.b = b;
  C(v);
  f4(3, 0, 0, 0, u.b);
  v.b = b;
  C(v);
  f4(4, 0, 0, 0, 0, u.b);
  v.b = b;
  C(v);
  f4(5, 0, 0, 0, 0, 0, u.b);
  v.b = b;
  C(v);
  f4(6, 0, 0, 0, 0, 0, 0, u.b);
  v.b = b;
  C(v);
  f4(7, 0, 0, 0, 0, 0, 0, 0, u.b);
  v.b = b;
  C(v);
  f4(8, 0, 0, 0, 0, 0, 0, 0, 0, u.b);
  v.b = b;
  C(v);
  f4(9, 0, 0, 0, 0, 0, 0, 0, 0, 0, u.b);
  v.b = b;
  C(v);
#endif
  f5(0, u.c);
  v.c = c;
  C(v);
  f5(1, 0, u.c);
  v.c = c;
  C(v);
  f5(2, 0, 0, u.c);
  v.c = c;
  C(v);
  f5(3, 0, 0, 0, u.c);
  v.c = c;
  C(v);
  f5(4, 0, 0, 0, 0, u.c);
  v.c = c;
  C(v);
  f5(5, 0, 0, 0, 0, 0, u.c);
  v.c = c;
  C(v);
  f5(6, 0, 0, 0, 0, 0, 0, u.c);
  v.c = c;
  C(v);
  f5(7, 0, 0, 0, 0, 0, 0, 0, u.c);
  v.c = c;
  C(v);
  f5(8, 0, 0, 0, 0, 0, 0, 0, 0, u.c);
  v.c = c;
  C(v);
  f5(9, 0, 0, 0, 0, 0, 0, 0, 0, 0, u.c);
  v.c = c;
  C(v);
  f6(0, u.d);
  v.d = d;
  C(v);
  f6(1, 0, u.d);
  v.d = d;
  C(v);
  f6(2, 0, 0, u.d);
  v.d = d;
  C(v);
  f6(3, 0, 0, 0, u.d);
  v.d = d;
  C(v);
  f6(4, 0, 0, 0, 0, u.d);
  v.d = d;
  C(v);
  f6(5, 0, 0, 0, 0, 0, u.d);
  v.d = d;
  C(v);
  f6(6, 0, 0, 0, 0, 0, 0, u.d);
  v.d = d;
  C(v);
  f6(7, 0, 0, 0, 0, 0, 0, 0, u.d);
  v.d = d;
  C(v);
  f6(8, 0, 0, 0, 0, 0, 0, 0, 0, u.d);
  v.d = d;
  C(v);
  f6(9, 0, 0, 0, 0, 0, 0, 0, 0, 0, u.d);
  v.d = d;
  C(v);
  u.e.a = 1.25;
  u.e.b = 2.75;
  u.e.c = -3.5;
  u.e.d = -2.0;
#undef C
#define C(x)                                                                   \
  do {                                                                         \
    if (u.e.a != x.e.a || u.e.b != x.e.b || u.e.c != x.e.c || u.e.d != x.e.d)  \
      __builtin_abort();                                                       \
    u.e.a++;                                                                   \
    u.e.b--;                                                                   \
    u.e.c++;                                                                   \
    u.e.d--;                                                                   \
  } while (0)
  v.e = f7(0, u.e);
  C(v);
  v.e = f7(1, 0.0, u.e);
  C(v);
  v.e = f7(2, 0.0, 0.0, u.e);
  C(v);
  v.e = f7(3, 0.0, 0.0, 0.0, u.e);
  C(v);
  v.e = f7(4, 0.0, 0.0, 0.0, 0.0, u.e);
  C(v);
  v.e = f7(5, 0.0, 0.0, 0.0, 0.0, 0.0, u.e);
  C(v);
  v.e = f7(6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.e);
  C(v);
  v.e = f7(7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.e);
  C(v);
  v.e = f7(8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.e);
  C(v);
  v.e = f7(9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.e);
  C(v);
  v.f = f8(0, u.f);
  C(v);
  v.f = f8(1, 0.0, u.f);
  C(v);
  v.f = f8(2, 0.0, 0.0, u.f);
  C(v);
  v.f = f8(3, 0.0, 0.0, 0.0, u.f);
  C(v);
  v.f = f8(4, 0.0, 0.0, 0.0, 0.0, u.f);
  C(v);
  v.f = f8(5, 0.0, 0.0, 0.0, 0.0, 0.0, u.f);
  C(v);
  v.f = f8(6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.f);
  C(v);
  v.f = f8(7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.f);
  C(v);
  v.f = f8(8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.f);
  C(v);
  v.f = f8(9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.f);
  C(v);
  f9(0, u.e);
  v.e = e;
  C(v);
  f9(1, 0.0, u.e);
  v.e = e;
  C(v);
  f9(2, 0.0, 0.0, u.e);
  v.e = e;
  C(v);
  f9(3, 0.0, 0.0, 0.0, u.e);
  v.e = e;
  C(v);
  f9(4, 0.0, 0.0, 0.0, 0.0, u.e);
  v.e = e;
  C(v);
  f9(5, 0.0, 0.0, 0.0, 0.0, 0.0, u.e);
  v.e = e;
  C(v);
  f9(6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.e);
  v.e = e;
  C(v);
  f9(7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.e);
  v.e = e;
  C(v);
  f9(8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.e);
  v.e = e;
  C(v);
  f9(9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.e);
  v.e = e;
  C(v);
  f10(0, u.f);
  v.f = f;
  C(v);
  f10(1, 0.0, u.f);
  v.f = f;
  C(v);
  f10(2, 0.0, 0.0, u.f);
  v.f = f;
  C(v);
  f10(3, 0.0, 0.0, 0.0, u.f);
  v.f = f;
  C(v);
  f10(4, 0.0, 0.0, 0.0, 0.0, u.f);
  v.f = f;
  C(v);
  f10(5, 0.0, 0.0, 0.0, 0.0, 0.0, u.f);
  v.f = f;
  C(v);
  f10(6, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.f);
  v.f = f;
  C(v);
  f10(7, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.f);
  v.f = f;
  C(v);
  f10(8, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.f);
  v.f = f;
  C(v);
  f10(9, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, u.f);
  v.f = f;
  C(v);
  u.g.a = 9.5;
  u.g.b = 0x5555555555555555ULL;
#undef C
#define C(x)                                                                   \
  do {                                                                         \
    if (u.e.a != x.e.a || u.e.b != x.e.b)                                      \
      __builtin_abort();                                                       \
    u.e.a++;                                                                   \
    u.e.b--;                                                                   \
  } while (0)
  v.g = f11(0, u.g);
  C(v);
  v.g = f11(1, 0, 0.0, u.g);
  C(v);
  v.g = f11(2, 0, 0.0, 0, 0.0, u.g);
  C(v);
  v.g = f11(3, 0, 0.0, 0, 0.0, 0, 0.0, u.g);
  C(v);
  v.g = f11(4, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.g);
  C(v);
  v.g = f11(5, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.g);
  C(v);
  v.g = f11(6, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.g);
  C(v);
  v.g = f11(7, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.g);
  C(v);
  v.g = f11(8, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0,
            u.g);
  C(v);
  v.g = f11(9, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0,
            0, 0.0, u.g);
  C(v);
  v.h = f12(0, u.h);
  C(v);
  v.h = f12(1, 0, 0.0, u.h);
  C(v);
  v.h = f12(2, 0, 0.0, 0, 0.0, u.h);
  C(v);
  v.h = f12(3, 0, 0.0, 0, 0.0, 0, 0.0, u.h);
  C(v);
  v.h = f12(4, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.h);
  C(v);
  v.h = f12(5, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.h);
  C(v);
  v.h = f12(6, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.h);
  C(v);
  v.h = f12(7, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.h);
  C(v);
  v.h = f12(8, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0,
            u.h);
  C(v);
  v.h = f12(9, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0,
            0, 0.0, u.h);
  C(v);
  f13(0, u.g);
  v.g = g;
  C(v);
  f13(1, 0, 0.0, u.g);
  v.g = g;
  C(v);
  f13(2, 0, 0.0, 0, 0.0, u.g);
  v.g = g;
  C(v);
  f13(3, 0, 0.0, 0, 0.0, 0, 0.0, u.g);
  v.g = g;
  C(v);
  f13(4, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.g);
  v.g = g;
  C(v);
  f13(5, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.g);
  v.g = g;
  C(v);
  f13(6, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.g);
  v.g = g;
  C(v);
  f13(7, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.g);
  v.g = g;
  C(v);
  f13(8, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.g);
  v.g = g;
  C(v);
  f13(9, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0,
      u.g);
  v.g = g;
  C(v);
  f14(0, u.h);
  v.h = h;
  C(v);
  f14(1, 0, 0.0, u.h);
  v.h = h;
  C(v);
  f14(2, 0, 0.0, 0, 0.0, u.h);
  v.h = h;
  C(v);
  f14(3, 0, 0.0, 0, 0.0, 0, 0.0, u.h);
  v.h = h;
  C(v);
  f14(4, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.h);
  v.h = h;
  C(v);
  f14(5, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.h);
  v.h = h;
  C(v);
  f14(6, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.h);
  v.h = h;
  C(v);
  f14(7, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.h);
  v.h = h;
  C(v);
  f14(8, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, u.h);
  v.h = h;
  C(v);
  f14(9, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0, 0, 0.0,
      u.h);
  v.h = h;
  C(v);
  return 0;
}


// SLATE-FILECHECK-DEFINES DEFAULT

// SLATE-FILECHECK-BEGIN DEFAULT
// DEFAULT: module {
// DEFAULT-NEXT:     target "x86_64-unknown-linux-gnu" {
// DEFAULT-NEXT:         endian = little;
// DEFAULT-NEXT:         pointer [size=8, align=8];
// DEFAULT-NEXT:         stack_alignment = 16;
// DEFAULT-NEXT:         long_double = f80;
// DEFAULT-NEXT:         storage bool [size=1, align=1];
// DEFAULT-NEXT:         storage i8, u8 [size=1, align=1];
// DEFAULT-NEXT:         storage i16, u16 [size=2, align=2];
// DEFAULT-NEXT:         storage i32, u32 [size=4, align=4];
// DEFAULT-NEXT:         storage i64, u64 [size=8, align=8];
// DEFAULT-NEXT:         storage i128, u128 [size=16, align=16];
// DEFAULT-NEXT:         storage bf16 [size=2, align=2];
// DEFAULT-NEXT:         storage f16 [size=2, align=2];
// DEFAULT-NEXT:         storage f32 [size=4, align=4];
// DEFAULT-NEXT:         storage f64 [size=8, align=8];
// DEFAULT-NEXT:         storage f80 [size=16, align=16];
// DEFAULT-NEXT:         storage f128 [size=16, align=16];
// DEFAULT-NEXT:         storage d32 [size=4, align=4];
// DEFAULT-NEXT:         storage d64 [size=8, align=8];
// DEFAULT-NEXT:         storage d128 [size=16, align=16];
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     type @type0 va_list = va_list;
// DEFAULT-NEXT:     type @type1 S = struct {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type2 T = struct {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type3 U = struct {
// DEFAULT-NEXT:         field0 a: f64;
// DEFAULT-NEXT:         field1 b: f64;
// DEFAULT-NEXT:         field2 c: f64;
// DEFAULT-NEXT:         field3 d: f64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type4 V = struct {
// DEFAULT-NEXT:         field0 a: f64;
// DEFAULT-NEXT:         field1 b: f64;
// DEFAULT-NEXT:         field2 c: f64;
// DEFAULT-NEXT:         field3 d: f64;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type5 W = struct {
// DEFAULT-NEXT:         field0 a: f64;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type6 X = struct {
// DEFAULT-NEXT:         field0 a: f64;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type7 Y = union {
// DEFAULT-NEXT:         field0 b: i128;
// DEFAULT-NEXT:         field1 c: @type1;
// DEFAULT-NEXT:         field2 d: @type2;
// DEFAULT-NEXT:         field3 e: @type3;
// DEFAULT-NEXT:         field4 f: @type4;
// DEFAULT-NEXT:         field5 g: @type5;
// DEFAULT-NEXT:         field6 h: @type6;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0, 0, 0, 0, 0, 0]];
// DEFAULT-NEXT:     global %7 b: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %8 c: @type1 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %9 d: @type2 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %10 e: @type3 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %11 f: @type4 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %12 g: @type5 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %13 h: @type6 [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %14 @f1(%15 x: i32, ...) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %16 r: i128 [storage=automatic];
// DEFAULT-NEXT:         let %17 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%17);
// DEFAULT-NEXT:         while %67 {
// DEFAULT-NEXT:             let %221: i32 [synthetic] = read<i32>(%15);
// DEFAULT-NEXT:             let %222: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%221), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%15, read<i32>(%222));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%221), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<i32>(%17);
// DEFAULT-NEXT:         write<i128>(%16, va_arg<i128>(%17));
// DEFAULT-NEXT:         va_arg<i128>(%17);
// DEFAULT-NEXT:         va_end(%17);
// DEFAULT-NEXT:         return read<i128>(%16);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %18 @f2(%19 x: i32, ...) -> @type1 [linkage=external] [abi=sysv64(scalar) -> coerce<i64, i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %20 r: @type1 [storage=automatic];
// DEFAULT-NEXT:         let %21 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%21);
// DEFAULT-NEXT:         while %68 {
// DEFAULT-NEXT:             let %223: i32 [synthetic] = read<i32>(%19);
// DEFAULT-NEXT:             let %224: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%223), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%19, read<i32>(%224));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%223), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<i32>(%21);
// DEFAULT-NEXT:         write<@type1>(%20, copy<@type1, reason=assign>(va_arg<@type1>(%21)));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(va_arg<@type1>(%21));
// DEFAULT-NEXT:         va_end(%21);
// DEFAULT-NEXT:         return copy<@type1, reason=return>(read<@type1>(%20));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %22 @f3(%23 x: i32, ...) -> @type2 [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %24 r: @type2 [storage=automatic];
// DEFAULT-NEXT:         let %25 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%25);
// DEFAULT-NEXT:         while %69 {
// DEFAULT-NEXT:             let %225: i32 [synthetic] = read<i32>(%23);
// DEFAULT-NEXT:             let %226: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%225), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%23, read<i32>(%226));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%225), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<i32>(%25);
// DEFAULT-NEXT:         write<@type2>(%24, copy<@type2, reason=assign>(va_arg<@type2>(%25)));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(va_arg<@type2>(%25));
// DEFAULT-NEXT:         va_end(%25);
// DEFAULT-NEXT:         return copy<@type2, reason=return>(read<@type2>(%24));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %26 @f4(%27 x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %28 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%28);
// DEFAULT-NEXT:         while %70 {
// DEFAULT-NEXT:             let %227: i32 [synthetic] = read<i32>(%27);
// DEFAULT-NEXT:             let %228: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%227), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%27, read<i32>(%228));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%227), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<i32>(%28);
// DEFAULT-NEXT:         write<i128>(%7, va_arg<i128>(%28));
// DEFAULT-NEXT:         va_arg<i128>(%28);
// DEFAULT-NEXT:         va_end(%28);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %29 @f5(%30 x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %31 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%31);
// DEFAULT-NEXT:         while %71 {
// DEFAULT-NEXT:             let %229: i32 [synthetic] = read<i32>(%30);
// DEFAULT-NEXT:             let %230: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%229), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%30, read<i32>(%230));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%229), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<i32>(%31);
// DEFAULT-NEXT:         write<@type1>(%8, copy<@type1, reason=assign>(va_arg<@type1>(%31)));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(va_arg<@type1>(%31));
// DEFAULT-NEXT:         va_end(%31);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %32 @f6(%33 x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %34 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%34);
// DEFAULT-NEXT:         while %72 {
// DEFAULT-NEXT:             let %231: i32 [synthetic] = read<i32>(%33);
// DEFAULT-NEXT:             let %232: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%231), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%33, read<i32>(%232));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%231), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<i32>(%34);
// DEFAULT-NEXT:         write<@type2>(%9, copy<@type2, reason=assign>(va_arg<@type2>(%34)));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(va_arg<@type2>(%34));
// DEFAULT-NEXT:         va_end(%34);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %35 @f7(%36 x: i32, ...) -> @type3 [linkage=external] [abi=sysv64(scalar) -> sret<align=8>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %37 r: @type3 [storage=automatic];
// DEFAULT-NEXT:         let %38 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%38);
// DEFAULT-NEXT:         while %73 {
// DEFAULT-NEXT:             let %233: i32 [synthetic] = read<i32>(%36);
// DEFAULT-NEXT:             let %234: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%233), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%36, read<i32>(%234));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%233), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<f64>(%38);
// DEFAULT-NEXT:         write<@type3>(%37, copy<@type3, reason=assign>(va_arg<@type3>(%38)));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(va_arg<@type3>(%38));
// DEFAULT-NEXT:         va_end(%38);
// DEFAULT-NEXT:         return copy<@type3, reason=return>(read<@type3>(%37));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %39 @f8(%40 x: i32, ...) -> @type4 [linkage=external] [abi=sysv64(scalar) -> sret<align=32>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %41 r: @type4 [storage=automatic];
// DEFAULT-NEXT:         let %42 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%42);
// DEFAULT-NEXT:         while %74 {
// DEFAULT-NEXT:             let %235: i32 [synthetic] = read<i32>(%40);
// DEFAULT-NEXT:             let %236: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%235), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%40, read<i32>(%236));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%235), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<f64>(%42);
// DEFAULT-NEXT:         write<@type4>(%41, copy<@type4, reason=assign>(va_arg<@type4>(%42)));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(va_arg<@type4>(%42));
// DEFAULT-NEXT:         va_end(%42);
// DEFAULT-NEXT:         return copy<@type4, reason=return>(read<@type4>(%41));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %43 @f9(%44 x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %45 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%45);
// DEFAULT-NEXT:         while %75 {
// DEFAULT-NEXT:             let %237: i32 [synthetic] = read<i32>(%44);
// DEFAULT-NEXT:             let %238: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%237), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%44, read<i32>(%238));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%237), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<f64>(%45);
// DEFAULT-NEXT:         write<@type3>(%10, copy<@type3, reason=assign>(va_arg<@type3>(%45)));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(va_arg<@type3>(%45));
// DEFAULT-NEXT:         va_end(%45);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %46 @f10(%47 x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %48 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%48);
// DEFAULT-NEXT:         while %76 {
// DEFAULT-NEXT:             let %239: i32 [synthetic] = read<i32>(%47);
// DEFAULT-NEXT:             let %240: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%239), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%47, read<i32>(%240));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%239), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<f64>(%48);
// DEFAULT-NEXT:         write<@type4>(%11, copy<@type4, reason=assign>(va_arg<@type4>(%48)));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(va_arg<@type4>(%48));
// DEFAULT-NEXT:         va_end(%48);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %49 @f11(%50 x: i32, ...) -> @type5 [linkage=external] [abi=sysv64(scalar) -> coerce<f64, i64>] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %51 r: @type5 [storage=automatic];
// DEFAULT-NEXT:         let %52 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%52);
// DEFAULT-NEXT:         while %77 {
// DEFAULT-NEXT:             let %241: i32 [synthetic] = read<i32>(%50);
// DEFAULT-NEXT:             let %242: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%241), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%50, read<i32>(%242));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%241), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_arg<i32>(%52);
// DEFAULT-NEXT:                 va_arg<f64>(%52);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<@type5>(%51, copy<@type5, reason=assign>(va_arg<@type5>(%52)));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(va_arg<@type5>(%52));
// DEFAULT-NEXT:         va_end(%52);
// DEFAULT-NEXT:         return copy<@type5, reason=return>(read<@type5>(%51));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %53 @f12(%54 x: i32, ...) -> @type6 [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %55 r: @type6 [storage=automatic];
// DEFAULT-NEXT:         let %56 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%56);
// DEFAULT-NEXT:         while %78 {
// DEFAULT-NEXT:             let %243: i32 [synthetic] = read<i32>(%54);
// DEFAULT-NEXT:             let %244: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%243), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%54, read<i32>(%244));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%243), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_arg<i32>(%56);
// DEFAULT-NEXT:                 va_arg<f64>(%56);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<@type6>(%55, copy<@type6, reason=assign>(va_arg<@type6>(%56)));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(va_arg<@type6>(%56));
// DEFAULT-NEXT:         va_end(%56);
// DEFAULT-NEXT:         return copy<@type6, reason=return>(read<@type6>(%55));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %57 @f13(%58 x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %59 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%59);
// DEFAULT-NEXT:         while %79 {
// DEFAULT-NEXT:             let %245: i32 [synthetic] = read<i32>(%58);
// DEFAULT-NEXT:             let %246: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%245), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%58, read<i32>(%246));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%245), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_arg<i32>(%59);
// DEFAULT-NEXT:                 va_arg<f64>(%59);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<@type5>(%12, copy<@type5, reason=assign>(va_arg<@type5>(%59)));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(va_arg<@type5>(%59));
// DEFAULT-NEXT:         va_end(%59);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %60 @f14(%61 x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %62 ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%62);
// DEFAULT-NEXT:         while %80 {
// DEFAULT-NEXT:             let %247: i32 [synthetic] = read<i32>(%61);
// DEFAULT-NEXT:             let %248: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%247), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%61, read<i32>(%248));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%247), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_arg<i32>(%62);
// DEFAULT-NEXT:                 va_arg<f64>(%62);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<@type6>(%13, copy<@type6, reason=assign>(va_arg<@type6>(%62)));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(va_arg<@type6>(%62));
// DEFAULT-NEXT:         va_end(%62);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %63 @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %65 u: @type7 [storage=automatic];
// DEFAULT-NEXT:         let %66 v: @type7 [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field0(field1(%65)), reinterpret<i64, reason=assign, fits=always>(const<u64>(6148914691236517205)));
// DEFAULT-NEXT:         write<i64>(field1(field1(%65)), reinterpret<i64, reason=assign, fits=unknown>(const<u64>(12297829382473034410)));
// DEFAULT-NEXT:         write<i128>(field0(%66), call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(0), read<i128>(field0(%65))));
// DEFAULT-NEXT:         call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         do %81
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %249: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %250: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%249), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%250));
// DEFAULT-NEXT:                 let %251: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %252: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%251), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%252));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%66), call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(1), const<i32>(0), read<i128>(field0(%65))));
// DEFAULT-NEXT:         call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(1), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         do %82
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %253: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %254: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%253), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%254));
// DEFAULT-NEXT:                 let %255: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %256: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%255), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%256));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%66), call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(2), const<i32>(0), const<i32>(0), read<i128>(field0(%65))));
// DEFAULT-NEXT:         call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(2), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         do %83
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %257: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %258: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%257), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%258));
// DEFAULT-NEXT:                 let %259: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %260: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%259), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%260));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%66), call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65))));
// DEFAULT-NEXT:         call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         do %84
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %261: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %262: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%261), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%262));
// DEFAULT-NEXT:                 let %263: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %264: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%263), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%264));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%66), call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65))));
// DEFAULT-NEXT:         call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         do %85
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %265: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %266: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%265), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%266));
// DEFAULT-NEXT:                 let %267: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %268: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%267), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%268));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%66), call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65))));
// DEFAULT-NEXT:         call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         do %86
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %269: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %270: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%269), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%270));
// DEFAULT-NEXT:                 let %271: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %272: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%271), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%272));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%66), call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65))));
// DEFAULT-NEXT:         call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         do %87
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %273: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %274: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%273), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%274));
// DEFAULT-NEXT:                 let %275: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %276: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%275), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%276));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%66), call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65))));
// DEFAULT-NEXT:         call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         do %88
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %277: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %278: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%277), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%278));
// DEFAULT-NEXT:                 let %279: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %280: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%279), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%280));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%66), call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65))));
// DEFAULT-NEXT:         call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         do %89
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %281: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %282: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%281), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%282));
// DEFAULT-NEXT:                 let %283: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %284: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%283), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%284));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%66), call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65))));
// DEFAULT-NEXT:         call<i128, signature=fn(i32, ...) -> i128>(%14, const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         do %90
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %285: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %286: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%285), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%286));
// DEFAULT-NEXT:                 let %287: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %288: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%287), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%288));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65)))));
// DEFAULT-NEXT:         do %91
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %289: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %290: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%289), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%290));
// DEFAULT-NEXT:                 let %291: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %292: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%291), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%292));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(1), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(1), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65)))));
// DEFAULT-NEXT:         do %92
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %293: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %294: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%293), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%294));
// DEFAULT-NEXT:                 let %295: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %296: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%295), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%296));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(2), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(2), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65)))));
// DEFAULT-NEXT:         do %93
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %297: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %298: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%297), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%298));
// DEFAULT-NEXT:                 let %299: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %300: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%299), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%300));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65)))));
// DEFAULT-NEXT:         do %94
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %301: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %302: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%301), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%302));
// DEFAULT-NEXT:                 let %303: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %304: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%303), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%304));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65)))));
// DEFAULT-NEXT:         do %95
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %305: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %306: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%305), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%306));
// DEFAULT-NEXT:                 let %307: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %308: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%307), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%308));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65)))));
// DEFAULT-NEXT:         do %96
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %309: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %310: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%309), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%310));
// DEFAULT-NEXT:                 let %311: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %312: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%311), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%312));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65)))));
// DEFAULT-NEXT:         do %97
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %313: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %314: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%313), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%314));
// DEFAULT-NEXT:                 let %315: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %316: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%315), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%316));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65)))));
// DEFAULT-NEXT:         do %98
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %317: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %318: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%317), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%318));
// DEFAULT-NEXT:                 let %319: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %320: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%319), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%320));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65)))));
// DEFAULT-NEXT:         do %99
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %321: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %322: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%321), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%322));
// DEFAULT-NEXT:                 let %323: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %324: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%323), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%324));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))))));
// DEFAULT-NEXT:         copy<@type1, reason=assign>(call<@type1, signature=fn(i32, ...) -> @type1, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> coerce<i64, i64>>(%18, const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65)))));
// DEFAULT-NEXT:         do %100
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %325: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %326: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%325), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%326));
// DEFAULT-NEXT:                 let %327: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %328: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%327), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%328));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, native_c) -> native_c>(%22, const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, native_c) -> native_c>(%22, const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65)))));
// DEFAULT-NEXT:         do %101
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %329: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %330: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%329), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%330));
// DEFAULT-NEXT:                 let %331: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %332: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%331), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%332));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, native_c) -> native_c>(%22, const<i32>(1), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, native_c) -> native_c>(%22, const<i32>(1), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65)))));
// DEFAULT-NEXT:         do %102
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %333: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %334: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%333), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%334));
// DEFAULT-NEXT:                 let %335: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %336: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%335), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%336));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(2), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(2), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65)))));
// DEFAULT-NEXT:         do %103
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %337: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %338: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%337), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%338));
// DEFAULT-NEXT:                 let %339: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %340: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%339), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%340));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65)))));
// DEFAULT-NEXT:         do %104
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %341: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %342: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%341), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%342));
// DEFAULT-NEXT:                 let %343: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %344: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%343), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%344));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65)))));
// DEFAULT-NEXT:         do %105
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %345: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %346: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%345), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%346));
// DEFAULT-NEXT:                 let %347: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %348: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%347), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%348));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65)))));
// DEFAULT-NEXT:         do %106
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %349: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %350: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%349), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%350));
// DEFAULT-NEXT:                 let %351: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %352: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%351), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%352));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65)))));
// DEFAULT-NEXT:         do %107
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %353: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %354: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%353), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%354));
// DEFAULT-NEXT:                 let %355: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %356: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%355), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%356));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65)))));
// DEFAULT-NEXT:         do %108
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %357: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %358: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%357), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%358));
// DEFAULT-NEXT:                 let %359: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %360: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%359), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%360));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65)))));
// DEFAULT-NEXT:         do %109
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %361: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %362: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%361), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%362));
// DEFAULT-NEXT:                 let %363: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %364: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%363), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%364));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))))));
// DEFAULT-NEXT:         copy<@type2, reason=assign>(call<@type2, signature=fn(i32, ...) -> @type2, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%22, const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65)))));
// DEFAULT-NEXT:         do %110
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %365: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %366: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%365), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%366));
// DEFAULT-NEXT:                 let %367: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %368: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%367), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%368));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%26, const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         write<i128>(field0(%66), read<i128>(%7));
// DEFAULT-NEXT:         do %111
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %369: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %370: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%369), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%370));
// DEFAULT-NEXT:                 let %371: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %372: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%371), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%372));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%26, const<i32>(1), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         write<i128>(field0(%66), read<i128>(%7));
// DEFAULT-NEXT:         do %112
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %373: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %374: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%373), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%374));
// DEFAULT-NEXT:                 let %375: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %376: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%375), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%376));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%26, const<i32>(2), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         write<i128>(field0(%66), read<i128>(%7));
// DEFAULT-NEXT:         do %113
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %377: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %378: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%377), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%378));
// DEFAULT-NEXT:                 let %379: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %380: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%379), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%380));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%26, const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         write<i128>(field0(%66), read<i128>(%7));
// DEFAULT-NEXT:         do %114
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %381: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %382: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%381), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%382));
// DEFAULT-NEXT:                 let %383: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %384: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%383), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%384));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%26, const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         write<i128>(field0(%66), read<i128>(%7));
// DEFAULT-NEXT:         do %115
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %385: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %386: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%385), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%386));
// DEFAULT-NEXT:                 let %387: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %388: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%387), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%388));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%26, const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         write<i128>(field0(%66), read<i128>(%7));
// DEFAULT-NEXT:         do %116
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %389: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %390: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%389), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%390));
// DEFAULT-NEXT:                 let %391: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %392: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%391), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%392));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%26, const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         write<i128>(field0(%66), read<i128>(%7));
// DEFAULT-NEXT:         do %117
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %393: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %394: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%393), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%394));
// DEFAULT-NEXT:                 let %395: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %396: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%395), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%396));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%26, const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         write<i128>(field0(%66), read<i128>(%7));
// DEFAULT-NEXT:         do %118
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %397: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %398: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%397), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%398));
// DEFAULT-NEXT:                 let %399: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %400: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%399), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%400));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%26, const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         write<i128>(field0(%66), read<i128>(%7));
// DEFAULT-NEXT:         do %119
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %401: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %402: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%401), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%402));
// DEFAULT-NEXT:                 let %403: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %404: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%403), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%404));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%26, const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%65)));
// DEFAULT-NEXT:         write<i128>(field0(%66), read<i128>(%7));
// DEFAULT-NEXT:         do %120
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %405: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %406: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%405), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%406));
// DEFAULT-NEXT:                 let %407: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %408: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%407), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%408));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, coerce<i64, i64>) -> void>(%29, const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(read<@type1>(%8)));
// DEFAULT-NEXT:         do %121
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %409: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %410: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%409), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%410));
// DEFAULT-NEXT:                 let %411: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %412: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%411), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%412));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, coerce<i64, i64>) -> void>(%29, const<i32>(1), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(read<@type1>(%8)));
// DEFAULT-NEXT:         do %122
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %413: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %414: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%413), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%414));
// DEFAULT-NEXT:                 let %415: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %416: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%415), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%416));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, coerce<i64, i64>) -> void>(%29, const<i32>(2), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(read<@type1>(%8)));
// DEFAULT-NEXT:         do %123
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %417: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %418: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%417), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%418));
// DEFAULT-NEXT:                 let %419: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %420: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%419), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%420));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, coerce<i64, i64>) -> void>(%29, const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(read<@type1>(%8)));
// DEFAULT-NEXT:         do %124
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %421: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %422: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%421), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%422));
// DEFAULT-NEXT:                 let %423: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %424: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%423), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%424));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> void>(%29, const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(read<@type1>(%8)));
// DEFAULT-NEXT:         do %125
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %425: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %426: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%425), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%426));
// DEFAULT-NEXT:                 let %427: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %428: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%427), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%428));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> void>(%29, const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(read<@type1>(%8)));
// DEFAULT-NEXT:         do %126
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %429: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %430: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%429), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%430));
// DEFAULT-NEXT:                 let %431: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %432: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%431), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%432));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> void>(%29, const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(read<@type1>(%8)));
// DEFAULT-NEXT:         do %127
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %433: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %434: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%433), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%434));
// DEFAULT-NEXT:                 let %435: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %436: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%435), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%436));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> void>(%29, const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(read<@type1>(%8)));
// DEFAULT-NEXT:         do %128
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %437: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %438: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%437), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%438));
// DEFAULT-NEXT:                 let %439: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %440: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%439), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%440));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> void>(%29, const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(read<@type1>(%8)));
// DEFAULT-NEXT:         do %129
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %441: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %442: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%441), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%442));
// DEFAULT-NEXT:                 let %443: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %444: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%443), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%444));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<i64, i64>) -> void>(%29, const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type1, reason=vararg>(read<@type1>(field1(%65))));
// DEFAULT-NEXT:         write<@type1>(field1(%66), copy<@type1, reason=assign>(read<@type1>(%8)));
// DEFAULT-NEXT:         do %130
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %445: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %446: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%445), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%446));
// DEFAULT-NEXT:                 let %447: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %448: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%447), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%448));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c) -> void>(%32, const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(read<@type2>(%9)));
// DEFAULT-NEXT:         do %131
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %449: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %450: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%449), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%450));
// DEFAULT-NEXT:                 let %451: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %452: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%451), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%452));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, native_c) -> void>(%32, const<i32>(1), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(read<@type2>(%9)));
// DEFAULT-NEXT:         do %132
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %453: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %454: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%453), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%454));
// DEFAULT-NEXT:                 let %455: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %456: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%455), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%456));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, native_c) -> void>(%32, const<i32>(2), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(read<@type2>(%9)));
// DEFAULT-NEXT:         do %133
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %457: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %458: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%457), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%458));
// DEFAULT-NEXT:                 let %459: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %460: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%459), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%460));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%32, const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(read<@type2>(%9)));
// DEFAULT-NEXT:         do %134
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %461: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %462: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%461), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%462));
// DEFAULT-NEXT:                 let %463: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %464: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%463), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%464));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%32, const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(read<@type2>(%9)));
// DEFAULT-NEXT:         do %135
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %465: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %466: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%465), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%466));
// DEFAULT-NEXT:                 let %467: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %468: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%467), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%468));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%32, const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(read<@type2>(%9)));
// DEFAULT-NEXT:         do %136
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %469: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %470: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%469), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%470));
// DEFAULT-NEXT:                 let %471: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %472: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%471), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%472));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%32, const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(read<@type2>(%9)));
// DEFAULT-NEXT:         do %137
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %473: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %474: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%473), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%474));
// DEFAULT-NEXT:                 let %475: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %476: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%475), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%476));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%32, const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(read<@type2>(%9)));
// DEFAULT-NEXT:         do %138
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %477: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %478: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%477), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%478));
// DEFAULT-NEXT:                 let %479: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %480: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%479), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%480));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%32, const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(read<@type2>(%9)));
// DEFAULT-NEXT:         do %139
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %481: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %482: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%481), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%482));
// DEFAULT-NEXT:                 let %483: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %484: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%483), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%484));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%32, const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type2, reason=vararg>(read<@type2>(field2(%65))));
// DEFAULT-NEXT:         write<@type2>(field2(%66), copy<@type2, reason=assign>(read<@type2>(%9)));
// DEFAULT-NEXT:         do %140
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%65))), read<i64>(field0(field1(%66)))), ne<i64>(read<i64>(field1(field1(%65))), read<i64>(field1(field1(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %485: i64 [synthetic] = read<i64>(field0(field1(%65)));
// DEFAULT-NEXT:                 let %486: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%485), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%65)), read<i64>(%486));
// DEFAULT-NEXT:                 let %487: i64 [synthetic] = read<i64>(field1(field1(%65)));
// DEFAULT-NEXT:                 let %488: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%487), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%65)), read<i64>(%488));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<f64>(field0(field3(%65)), const<f64>(1.25));
// DEFAULT-NEXT:         write<f64>(field1(field3(%65)), const<f64>(2.75));
// DEFAULT-NEXT:         write<f64>(field2(field3(%65)), neg<f64>(const<f64>(3.5)));
// DEFAULT-NEXT:         write<f64>(field3(field3(%65)), neg<f64>(const<f64>(2.0)));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))))));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(0), copy<@type3, reason=vararg>(read<@type3>(field3(%65)))));
// DEFAULT-NEXT:         do %141
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %489: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %490: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%489), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%490));
// DEFAULT-NEXT:                 let %491: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %492: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%491), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%492));
// DEFAULT-NEXT:                 let %493: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %494: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%493), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%494));
// DEFAULT-NEXT:                 let %495: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %496: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%495), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%496));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(1), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))))));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(1), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65)))));
// DEFAULT-NEXT:         do %142
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %497: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %498: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%497), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%498));
// DEFAULT-NEXT:                 let %499: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %500: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%499), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%500));
// DEFAULT-NEXT:                 let %501: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %502: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%501), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%502));
// DEFAULT-NEXT:                 let %503: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %504: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%503), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%504));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(2), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))))));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(2), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65)))));
// DEFAULT-NEXT:         do %143
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %505: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %506: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%505), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%506));
// DEFAULT-NEXT:                 let %507: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %508: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%507), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%508));
// DEFAULT-NEXT:                 let %509: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %510: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%509), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%510));
// DEFAULT-NEXT:                 let %511: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %512: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%511), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%512));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(3), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))))));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(3), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65)))));
// DEFAULT-NEXT:         do %144
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %513: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %514: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%513), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%514));
// DEFAULT-NEXT:                 let %515: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %516: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%515), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%516));
// DEFAULT-NEXT:                 let %517: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %518: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%517), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%518));
// DEFAULT-NEXT:                 let %519: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %520: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%519), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%520));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(4), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))))));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(4), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65)))));
// DEFAULT-NEXT:         do %145
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %521: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %522: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%521), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%522));
// DEFAULT-NEXT:                 let %523: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %524: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%523), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%524));
// DEFAULT-NEXT:                 let %525: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %526: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%525), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%526));
// DEFAULT-NEXT:                 let %527: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %528: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%527), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%528));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(5), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))))));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(5), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65)))));
// DEFAULT-NEXT:         do %146
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %529: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %530: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%529), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%530));
// DEFAULT-NEXT:                 let %531: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %532: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%531), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%532));
// DEFAULT-NEXT:                 let %533: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %534: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%533), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%534));
// DEFAULT-NEXT:                 let %535: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %536: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%535), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%536));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(6), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))))));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(6), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65)))));
// DEFAULT-NEXT:         do %147
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %537: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %538: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%537), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%538));
// DEFAULT-NEXT:                 let %539: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %540: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%539), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%540));
// DEFAULT-NEXT:                 let %541: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %542: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%541), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%542));
// DEFAULT-NEXT:                 let %543: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %544: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%543), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%544));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(7), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))))));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(7), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65)))));
// DEFAULT-NEXT:         do %148
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %545: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %546: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%545), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%546));
// DEFAULT-NEXT:                 let %547: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %548: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%547), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%548));
// DEFAULT-NEXT:                 let %549: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %550: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%549), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%550));
// DEFAULT-NEXT:                 let %551: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %552: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%551), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%552));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(8), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))))));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(8), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65)))));
// DEFAULT-NEXT:         do %149
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %553: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %554: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%553), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%554));
// DEFAULT-NEXT:                 let %555: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %556: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%555), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%556));
// DEFAULT-NEXT:                 let %557: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %558: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%557), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%558));
// DEFAULT-NEXT:                 let %559: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %560: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%559), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%560));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(9), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))))));
// DEFAULT-NEXT:         copy<@type3, reason=assign>(call<@type3, signature=fn(i32, ...) -> @type3, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> sret<align=8>>(%35, const<i32>(9), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65)))));
// DEFAULT-NEXT:         do %150
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %561: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %562: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%561), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%562));
// DEFAULT-NEXT:                 let %563: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %564: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%563), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%564));
// DEFAULT-NEXT:                 let %565: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %566: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%565), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%566));
// DEFAULT-NEXT:                 let %567: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %568: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%567), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%568));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))))));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(0), copy<@type4, reason=vararg>(read<@type4>(field4(%65)))));
// DEFAULT-NEXT:         do %151
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %569: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %570: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%569), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%570));
// DEFAULT-NEXT:                 let %571: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %572: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%571), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%572));
// DEFAULT-NEXT:                 let %573: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %574: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%573), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%574));
// DEFAULT-NEXT:                 let %575: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %576: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%575), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%576));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(1), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))))));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(1), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65)))));
// DEFAULT-NEXT:         do %152
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %577: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %578: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%577), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%578));
// DEFAULT-NEXT:                 let %579: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %580: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%579), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%580));
// DEFAULT-NEXT:                 let %581: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %582: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%581), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%582));
// DEFAULT-NEXT:                 let %583: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %584: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%583), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%584));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(2), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))))));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(2), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65)))));
// DEFAULT-NEXT:         do %153
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %585: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %586: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%585), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%586));
// DEFAULT-NEXT:                 let %587: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %588: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%587), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%588));
// DEFAULT-NEXT:                 let %589: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %590: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%589), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%590));
// DEFAULT-NEXT:                 let %591: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %592: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%591), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%592));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(3), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))))));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(3), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65)))));
// DEFAULT-NEXT:         do %154
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %593: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %594: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%593), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%594));
// DEFAULT-NEXT:                 let %595: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %596: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%595), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%596));
// DEFAULT-NEXT:                 let %597: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %598: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%597), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%598));
// DEFAULT-NEXT:                 let %599: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %600: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%599), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%600));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(4), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))))));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(4), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65)))));
// DEFAULT-NEXT:         do %155
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %601: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %602: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%601), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%602));
// DEFAULT-NEXT:                 let %603: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %604: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%603), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%604));
// DEFAULT-NEXT:                 let %605: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %606: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%605), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%606));
// DEFAULT-NEXT:                 let %607: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %608: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%607), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%608));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(5), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))))));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(5), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65)))));
// DEFAULT-NEXT:         do %156
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %609: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %610: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%609), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%610));
// DEFAULT-NEXT:                 let %611: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %612: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%611), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%612));
// DEFAULT-NEXT:                 let %613: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %614: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%613), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%614));
// DEFAULT-NEXT:                 let %615: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %616: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%615), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%616));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(6), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))))));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(6), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65)))));
// DEFAULT-NEXT:         do %157
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %617: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %618: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%617), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%618));
// DEFAULT-NEXT:                 let %619: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %620: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%619), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%620));
// DEFAULT-NEXT:                 let %621: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %622: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%621), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%622));
// DEFAULT-NEXT:                 let %623: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %624: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%623), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%624));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(7), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))))));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(7), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65)))));
// DEFAULT-NEXT:         do %158
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %625: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %626: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%625), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%626));
// DEFAULT-NEXT:                 let %627: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %628: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%627), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%628));
// DEFAULT-NEXT:                 let %629: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %630: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%629), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%630));
// DEFAULT-NEXT:                 let %631: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %632: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%631), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%632));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(8), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))))));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(8), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65)))));
// DEFAULT-NEXT:         do %159
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %633: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %634: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%633), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%634));
// DEFAULT-NEXT:                 let %635: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %636: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%635), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%636));
// DEFAULT-NEXT:                 let %637: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %638: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%637), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%638));
// DEFAULT-NEXT:                 let %639: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %640: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%639), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%640));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(9), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))))));
// DEFAULT-NEXT:         copy<@type4, reason=assign>(call<@type4, signature=fn(i32, ...) -> @type4, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> sret<align=32>>(%39, const<i32>(9), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65)))));
// DEFAULT-NEXT:         do %160
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %641: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %642: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%641), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%642));
// DEFAULT-NEXT:                 let %643: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %644: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%643), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%644));
// DEFAULT-NEXT:                 let %645: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %646: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%645), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%646));
// DEFAULT-NEXT:                 let %647: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %648: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%647), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%648));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, byval<align=8>) -> void>(%43, const<i32>(0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(read<@type3>(%10)));
// DEFAULT-NEXT:         do %161
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %649: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %650: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%649), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%650));
// DEFAULT-NEXT:                 let %651: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %652: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%651), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%652));
// DEFAULT-NEXT:                 let %653: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %654: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%653), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%654));
// DEFAULT-NEXT:                 let %655: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %656: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%655), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%656));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, byval<align=8>) -> void>(%43, const<i32>(1), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(read<@type3>(%10)));
// DEFAULT-NEXT:         do %162
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %657: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %658: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%657), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%658));
// DEFAULT-NEXT:                 let %659: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %660: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%659), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%660));
// DEFAULT-NEXT:                 let %661: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %662: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%661), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%662));
// DEFAULT-NEXT:                 let %663: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %664: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%663), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%664));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, byval<align=8>) -> void>(%43, const<i32>(2), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(read<@type3>(%10)));
// DEFAULT-NEXT:         do %163
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %665: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %666: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%665), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%666));
// DEFAULT-NEXT:                 let %667: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %668: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%667), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%668));
// DEFAULT-NEXT:                 let %669: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %670: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%669), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%670));
// DEFAULT-NEXT:                 let %671: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %672: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%671), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%672));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, byval<align=8>) -> void>(%43, const<i32>(3), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(read<@type3>(%10)));
// DEFAULT-NEXT:         do %164
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %673: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %674: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%673), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%674));
// DEFAULT-NEXT:                 let %675: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %676: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%675), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%676));
// DEFAULT-NEXT:                 let %677: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %678: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%677), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%678));
// DEFAULT-NEXT:                 let %679: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %680: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%679), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%680));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> void>(%43, const<i32>(4), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(read<@type3>(%10)));
// DEFAULT-NEXT:         do %165
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %681: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %682: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%681), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%682));
// DEFAULT-NEXT:                 let %683: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %684: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%683), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%684));
// DEFAULT-NEXT:                 let %685: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %686: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%685), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%686));
// DEFAULT-NEXT:                 let %687: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %688: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%687), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%688));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> void>(%43, const<i32>(5), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(read<@type3>(%10)));
// DEFAULT-NEXT:         do %166
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %689: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %690: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%689), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%690));
// DEFAULT-NEXT:                 let %691: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %692: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%691), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%692));
// DEFAULT-NEXT:                 let %693: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %694: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%693), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%694));
// DEFAULT-NEXT:                 let %695: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %696: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%695), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%696));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> void>(%43, const<i32>(6), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(read<@type3>(%10)));
// DEFAULT-NEXT:         do %167
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %697: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %698: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%697), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%698));
// DEFAULT-NEXT:                 let %699: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %700: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%699), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%700));
// DEFAULT-NEXT:                 let %701: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %702: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%701), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%702));
// DEFAULT-NEXT:                 let %703: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %704: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%703), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%704));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> void>(%43, const<i32>(7), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(read<@type3>(%10)));
// DEFAULT-NEXT:         do %168
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %705: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %706: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%705), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%706));
// DEFAULT-NEXT:                 let %707: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %708: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%707), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%708));
// DEFAULT-NEXT:                 let %709: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %710: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%709), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%710));
// DEFAULT-NEXT:                 let %711: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %712: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%711), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%712));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> void>(%43, const<i32>(8), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(read<@type3>(%10)));
// DEFAULT-NEXT:         do %169
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %713: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %714: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%713), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%714));
// DEFAULT-NEXT:                 let %715: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %716: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%715), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%716));
// DEFAULT-NEXT:                 let %717: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %718: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%717), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%718));
// DEFAULT-NEXT:                 let %719: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %720: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%719), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%720));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=8>) -> void>(%43, const<i32>(9), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type3, reason=vararg>(read<@type3>(field3(%65))));
// DEFAULT-NEXT:         write<@type3>(field3(%66), copy<@type3, reason=assign>(read<@type3>(%10)));
// DEFAULT-NEXT:         do %170
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %721: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %722: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%721), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%722));
// DEFAULT-NEXT:                 let %723: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %724: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%723), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%724));
// DEFAULT-NEXT:                 let %725: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %726: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%725), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%726));
// DEFAULT-NEXT:                 let %727: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %728: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%727), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%728));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, byval<align=32>) -> void>(%46, const<i32>(0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(read<@type4>(%11)));
// DEFAULT-NEXT:         do %171
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %729: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %730: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%729), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%730));
// DEFAULT-NEXT:                 let %731: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %732: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%731), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%732));
// DEFAULT-NEXT:                 let %733: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %734: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%733), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%734));
// DEFAULT-NEXT:                 let %735: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %736: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%735), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%736));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, byval<align=32>) -> void>(%46, const<i32>(1), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(read<@type4>(%11)));
// DEFAULT-NEXT:         do %172
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %737: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %738: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%737), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%738));
// DEFAULT-NEXT:                 let %739: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %740: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%739), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%740));
// DEFAULT-NEXT:                 let %741: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %742: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%741), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%742));
// DEFAULT-NEXT:                 let %743: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %744: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%743), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%744));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, byval<align=32>) -> void>(%46, const<i32>(2), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(read<@type4>(%11)));
// DEFAULT-NEXT:         do %173
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %745: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %746: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%745), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%746));
// DEFAULT-NEXT:                 let %747: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %748: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%747), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%748));
// DEFAULT-NEXT:                 let %749: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %750: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%749), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%750));
// DEFAULT-NEXT:                 let %751: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %752: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%751), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%752));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, byval<align=32>) -> void>(%46, const<i32>(3), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(read<@type4>(%11)));
// DEFAULT-NEXT:         do %174
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %753: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %754: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%753), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%754));
// DEFAULT-NEXT:                 let %755: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %756: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%755), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%756));
// DEFAULT-NEXT:                 let %757: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %758: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%757), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%758));
// DEFAULT-NEXT:                 let %759: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %760: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%759), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%760));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> void>(%46, const<i32>(4), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(read<@type4>(%11)));
// DEFAULT-NEXT:         do %175
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %761: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %762: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%761), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%762));
// DEFAULT-NEXT:                 let %763: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %764: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%763), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%764));
// DEFAULT-NEXT:                 let %765: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %766: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%765), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%766));
// DEFAULT-NEXT:                 let %767: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %768: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%767), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%768));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> void>(%46, const<i32>(5), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(read<@type4>(%11)));
// DEFAULT-NEXT:         do %176
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %769: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %770: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%769), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%770));
// DEFAULT-NEXT:                 let %771: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %772: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%771), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%772));
// DEFAULT-NEXT:                 let %773: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %774: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%773), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%774));
// DEFAULT-NEXT:                 let %775: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %776: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%775), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%776));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> void>(%46, const<i32>(6), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(read<@type4>(%11)));
// DEFAULT-NEXT:         do %177
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %777: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %778: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%777), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%778));
// DEFAULT-NEXT:                 let %779: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %780: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%779), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%780));
// DEFAULT-NEXT:                 let %781: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %782: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%781), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%782));
// DEFAULT-NEXT:                 let %783: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %784: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%783), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%784));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> void>(%46, const<i32>(7), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(read<@type4>(%11)));
// DEFAULT-NEXT:         do %178
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %785: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %786: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%785), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%786));
// DEFAULT-NEXT:                 let %787: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %788: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%787), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%788));
// DEFAULT-NEXT:                 let %789: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %790: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%789), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%790));
// DEFAULT-NEXT:                 let %791: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %792: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%791), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%792));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> void>(%46, const<i32>(8), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(read<@type4>(%11)));
// DEFAULT-NEXT:         do %179
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %793: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %794: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%793), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%794));
// DEFAULT-NEXT:                 let %795: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %796: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%795), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%796));
// DEFAULT-NEXT:                 let %797: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %798: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%797), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%798));
// DEFAULT-NEXT:                 let %799: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %800: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%799), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%800));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, byval<align=32>) -> void>(%46, const<i32>(9), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type4, reason=vararg>(read<@type4>(field4(%65))));
// DEFAULT-NEXT:         write<@type4>(field4(%66), copy<@type4, reason=assign>(read<@type4>(%11)));
// DEFAULT-NEXT:         do %180
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field2(field3(%65))), read<f64>(field2(field3(%66))))), ne<f64, exceptions=ignore>(read<f64>(field3(field3(%65))), read<f64>(field3(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %801: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %802: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%801), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%802));
// DEFAULT-NEXT:                 let %803: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %804: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%803), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%804));
// DEFAULT-NEXT:                 let %805: f64 [synthetic] = read<f64>(field2(field3(%65)));
// DEFAULT-NEXT:                 let %806: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%805), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%65)), read<f64>(%806));
// DEFAULT-NEXT:                 let %807: f64 [synthetic] = read<f64>(field3(field3(%65)));
// DEFAULT-NEXT:                 let %808: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%807), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%65)), read<f64>(%808));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<f64>(field0(field5(%65)), const<f64>(9.5));
// DEFAULT-NEXT:         write<i64>(field1(field5(%65)), reinterpret<i64, reason=assign, fits=always>(const<u64>(6148914691236517205)));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))))));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(0), copy<@type5, reason=vararg>(read<@type5>(field5(%65)))));
// DEFAULT-NEXT:         do %181
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %809: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %810: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%809), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%810));
// DEFAULT-NEXT:                 let %811: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %812: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%811), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%812));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(1), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))))));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(1), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65)))));
// DEFAULT-NEXT:         do %182
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %813: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %814: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%813), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%814));
// DEFAULT-NEXT:                 let %815: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %816: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%815), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%816));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(2), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))))));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(2), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65)))));
// DEFAULT-NEXT:         do %183
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %817: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %818: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%817), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%818));
// DEFAULT-NEXT:                 let %819: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %820: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%819), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%820));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(3), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))))));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(3), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65)))));
// DEFAULT-NEXT:         do %184
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %821: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %822: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%821), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%822));
// DEFAULT-NEXT:                 let %823: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %824: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%823), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%824));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(4), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))))));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(4), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65)))));
// DEFAULT-NEXT:         do %185
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %825: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %826: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%825), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%826));
// DEFAULT-NEXT:                 let %827: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %828: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%827), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%828));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(5), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))))));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(5), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65)))));
// DEFAULT-NEXT:         do %186
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %829: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %830: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%829), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%830));
// DEFAULT-NEXT:                 let %831: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %832: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%831), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%832));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(6), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))))));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(6), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65)))));
// DEFAULT-NEXT:         do %187
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %833: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %834: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%833), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%834));
// DEFAULT-NEXT:                 let %835: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %836: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%835), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%836));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(7), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))))));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(7), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65)))));
// DEFAULT-NEXT:         do %188
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %837: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %838: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%837), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%838));
// DEFAULT-NEXT:                 let %839: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %840: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%839), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%840));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(8), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))))));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(8), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65)))));
// DEFAULT-NEXT:         do %189
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %841: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %842: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%841), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%842));
// DEFAULT-NEXT:                 let %843: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %844: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%843), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%844));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(9), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))))));
// DEFAULT-NEXT:         copy<@type5, reason=assign>(call<@type5, signature=fn(i32, ...) -> @type5, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> coerce<f64, i64>>(%49, const<i32>(9), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65)))));
// DEFAULT-NEXT:         do %190
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %845: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %846: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%845), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%846));
// DEFAULT-NEXT:                 let %847: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %848: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%847), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%848));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, native_c) -> native_c>(%53, const<i32>(0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))))));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, native_c) -> native_c>(%53, const<i32>(0), copy<@type6, reason=vararg>(read<@type6>(field6(%65)))));
// DEFAULT-NEXT:         do %191
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %849: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %850: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%849), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%850));
// DEFAULT-NEXT:                 let %851: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %852: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%851), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%852));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(1), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))))));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(1), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65)))));
// DEFAULT-NEXT:         do %192
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %853: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %854: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%853), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%854));
// DEFAULT-NEXT:                 let %855: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %856: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%855), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%856));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(2), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))))));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(2), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65)))));
// DEFAULT-NEXT:         do %193
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %857: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %858: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%857), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%858));
// DEFAULT-NEXT:                 let %859: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %860: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%859), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%860));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(3), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))))));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(3), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65)))));
// DEFAULT-NEXT:         do %194
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %861: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %862: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%861), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%862));
// DEFAULT-NEXT:                 let %863: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %864: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%863), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%864));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(4), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))))));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(4), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65)))));
// DEFAULT-NEXT:         do %195
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %865: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %866: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%865), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%866));
// DEFAULT-NEXT:                 let %867: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %868: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%867), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%868));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(5), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))))));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(5), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65)))));
// DEFAULT-NEXT:         do %196
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %869: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %870: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%869), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%870));
// DEFAULT-NEXT:                 let %871: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %872: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%871), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%872));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(6), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))))));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(6), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65)))));
// DEFAULT-NEXT:         do %197
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %873: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %874: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%873), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%874));
// DEFAULT-NEXT:                 let %875: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %876: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%875), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%876));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(7), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))))));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(7), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65)))));
// DEFAULT-NEXT:         do %198
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %877: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %878: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%877), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%878));
// DEFAULT-NEXT:                 let %879: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %880: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%879), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%880));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(8), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))))));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(8), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65)))));
// DEFAULT-NEXT:         do %199
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %881: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %882: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%881), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%882));
// DEFAULT-NEXT:                 let %883: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %884: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%883), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%884));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(9), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))))));
// DEFAULT-NEXT:         copy<@type6, reason=assign>(call<@type6, signature=fn(i32, ...) -> @type6, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%53, const<i32>(9), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65)))));
// DEFAULT-NEXT:         do %200
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %885: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %886: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%885), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%886));
// DEFAULT-NEXT:                 let %887: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %888: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%887), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%888));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, coerce<f64, i64>) -> void>(%57, const<i32>(0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(read<@type5>(%12)));
// DEFAULT-NEXT:         do %201
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %889: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %890: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%889), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%890));
// DEFAULT-NEXT:                 let %891: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %892: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%891), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%892));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, coerce<f64, i64>) -> void>(%57, const<i32>(1), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(read<@type5>(%12)));
// DEFAULT-NEXT:         do %202
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %893: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %894: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%893), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%894));
// DEFAULT-NEXT:                 let %895: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %896: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%895), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%896));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> void>(%57, const<i32>(2), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(read<@type5>(%12)));
// DEFAULT-NEXT:         do %203
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %897: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %898: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%897), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%898));
// DEFAULT-NEXT:                 let %899: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %900: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%899), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%900));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> void>(%57, const<i32>(3), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(read<@type5>(%12)));
// DEFAULT-NEXT:         do %204
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %901: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %902: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%901), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%902));
// DEFAULT-NEXT:                 let %903: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %904: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%903), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%904));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> void>(%57, const<i32>(4), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(read<@type5>(%12)));
// DEFAULT-NEXT:         do %205
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %905: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %906: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%905), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%906));
// DEFAULT-NEXT:                 let %907: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %908: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%907), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%908));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> void>(%57, const<i32>(5), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(read<@type5>(%12)));
// DEFAULT-NEXT:         do %206
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %909: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %910: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%909), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%910));
// DEFAULT-NEXT:                 let %911: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %912: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%911), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%912));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> void>(%57, const<i32>(6), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(read<@type5>(%12)));
// DEFAULT-NEXT:         do %207
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %913: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %914: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%913), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%914));
// DEFAULT-NEXT:                 let %915: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %916: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%915), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%916));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> void>(%57, const<i32>(7), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(read<@type5>(%12)));
// DEFAULT-NEXT:         do %208
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %917: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %918: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%917), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%918));
// DEFAULT-NEXT:                 let %919: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %920: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%919), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%920));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> void>(%57, const<i32>(8), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(read<@type5>(%12)));
// DEFAULT-NEXT:         do %209
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %921: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %922: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%921), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%922));
// DEFAULT-NEXT:                 let %923: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %924: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%923), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%924));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, coerce<f64, i64>) -> void>(%57, const<i32>(9), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type5, reason=vararg>(read<@type5>(field5(%65))));
// DEFAULT-NEXT:         write<@type5>(field5(%66), copy<@type5, reason=assign>(read<@type5>(%12)));
// DEFAULT-NEXT:         do %210
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %925: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %926: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%925), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%926));
// DEFAULT-NEXT:                 let %927: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %928: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%927), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%928));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c) -> void>(%60, const<i32>(0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(read<@type6>(%13)));
// DEFAULT-NEXT:         do %211
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %929: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %930: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%929), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%930));
// DEFAULT-NEXT:                 let %931: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %932: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%931), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%932));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, native_c) -> void>(%60, const<i32>(1), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(read<@type6>(%13)));
// DEFAULT-NEXT:         do %212
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %933: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %934: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%933), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%934));
// DEFAULT-NEXT:                 let %935: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %936: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%935), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%936));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%60, const<i32>(2), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(read<@type6>(%13)));
// DEFAULT-NEXT:         do %213
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %937: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %938: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%937), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%938));
// DEFAULT-NEXT:                 let %939: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %940: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%939), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%940));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%60, const<i32>(3), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(read<@type6>(%13)));
// DEFAULT-NEXT:         do %214
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %941: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %942: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%941), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%942));
// DEFAULT-NEXT:                 let %943: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %944: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%943), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%944));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%60, const<i32>(4), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(read<@type6>(%13)));
// DEFAULT-NEXT:         do %215
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %945: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %946: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%945), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%946));
// DEFAULT-NEXT:                 let %947: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %948: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%947), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%948));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%60, const<i32>(5), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(read<@type6>(%13)));
// DEFAULT-NEXT:         do %216
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %949: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %950: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%949), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%950));
// DEFAULT-NEXT:                 let %951: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %952: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%951), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%952));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%60, const<i32>(6), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(read<@type6>(%13)));
// DEFAULT-NEXT:         do %217
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %953: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %954: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%953), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%954));
// DEFAULT-NEXT:                 let %955: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %956: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%955), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%956));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%60, const<i32>(7), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(read<@type6>(%13)));
// DEFAULT-NEXT:         do %218
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %957: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %958: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%957), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%958));
// DEFAULT-NEXT:                 let %959: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %960: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%959), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%960));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%60, const<i32>(8), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(read<@type6>(%13)));
// DEFAULT-NEXT:         do %219
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %961: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %962: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%961), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%962));
// DEFAULT-NEXT:                 let %963: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %964: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%963), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%964));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%60, const<i32>(9), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type6, reason=vararg>(read<@type6>(field6(%65))));
// DEFAULT-NEXT:         write<@type6>(field6(%66), copy<@type6, reason=assign>(read<@type6>(%13)));
// DEFAULT-NEXT:         do %220
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=ignore>(read<f64>(field0(field3(%65))), read<f64>(field0(field3(%66)))), ne<f64, exceptions=ignore>(read<f64>(field1(field3(%65))), read<f64>(field1(field3(%66)))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(__builtin_abort);
// DEFAULT-NEXT:                 let %965: f64 [synthetic] = read<f64>(field0(field3(%65)));
// DEFAULT-NEXT:                 let %966: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%965), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%65)), read<f64>(%966));
// DEFAULT-NEXT:                 let %967: f64 [synthetic] = read<f64>(field1(field3(%65)));
// DEFAULT-NEXT:                 let %968: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=ignore>(read<f64>(%967), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=ignore>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%65)), read<f64>(%968));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
