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
// DEFAULT-NEXT:     type @type[[TYPE___gnuc_va_list:[0-9]+]] __gnuc_va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_va_list:[0-9]+]] va_list = va_list;
// DEFAULT-NEXT:     type @type[[TYPE_S:[0-9]+]] S = struct {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_T:[0-9]+]] T = struct {
// DEFAULT-NEXT:         field0 a: i64;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_U:[0-9]+]] U = struct {
// DEFAULT-NEXT:         field0 a: f64;
// DEFAULT-NEXT:         field1 b: f64;
// DEFAULT-NEXT:         field2 c: f64;
// DEFAULT-NEXT:         field3 d: f64;
// DEFAULT-NEXT:     } [size=32, align=8, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_V:[0-9]+]] V = struct {
// DEFAULT-NEXT:         field0 a: f64;
// DEFAULT-NEXT:         field1 b: f64;
// DEFAULT-NEXT:         field2 c: f64;
// DEFAULT-NEXT:         field3 d: f64;
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 8, 16, 24]];
// DEFAULT-NEXT:     type @type[[TYPE_W:[0-9]+]] W = struct {
// DEFAULT-NEXT:         field0 a: f64;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=8, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_X:[0-9]+]] X = struct {
// DEFAULT-NEXT:         field0 a: f64;
// DEFAULT-NEXT:         field1 b: i64;
// DEFAULT-NEXT:     } [size=16, align=16, offsets=[0, 8]];
// DEFAULT-NEXT:     type @type[[TYPE_Y:[0-9]+]] Y = union {
// DEFAULT-NEXT:         field0 b: i128;
// DEFAULT-NEXT:         field1 c: @type[[TYPE_S]];
// DEFAULT-NEXT:         field2 d: @type[[TYPE_T]];
// DEFAULT-NEXT:         field3 e: @type[[TYPE_U]];
// DEFAULT-NEXT:         field4 f: @type[[TYPE_V]];
// DEFAULT-NEXT:         field5 g: @type[[TYPE_W]];
// DEFAULT-NEXT:         field6 h: @type[[TYPE_X]];
// DEFAULT-NEXT:     } [size=32, align=32, offsets=[0, 0, 0, 0, 0, 0, 0]];
// DEFAULT-NEXT:     global %[[VALUE_b:[0-9]+]] b: i128 [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_c:[0-9]+]] c: @type[[TYPE_S]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_d:[0-9]+]] d: @type[[TYPE_T]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_e:[0-9]+]] e: @type[[TYPE_U]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_f:[0-9]+]] f: @type[[TYPE_V]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_g:[0-9]+]] g: @type[[TYPE_W]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     global %[[VALUE_h:[0-9]+]] h: @type[[TYPE_X]] [storage=static] [linkage=external];
// DEFAULT-NEXT:     fn %[[VALUE_f1:[0-9]+]] @f1(%[[VALUE_x:[0-9]+]] x: i32, ...) -> i128 [linkage=external] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r:[0-9]+]] r: i128 [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap]]);
// DEFAULT-NEXT:         while %[[VALUE0:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE1:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x]]);
// DEFAULT-NEXT:             let %[[VALUE2:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE1]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x]], read<i32>(%[[VALUE2]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE1]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<i32>(%[[VALUE_ap]]);
// DEFAULT-NEXT:         write<i128>(%[[VALUE_r]], va_arg<i128>(%[[VALUE_ap]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap]]);
// DEFAULT-NEXT:         return read<i128>(%[[VALUE_r]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f2:[0-9]+]] @f2(%[[VALUE_x_2:[0-9]+]] x: i32, ...) -> @type[[TYPE_S]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_2:[0-9]+]] r: @type[[TYPE_S]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap_2:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         while %[[VALUE3:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE4:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_2]]);
// DEFAULT-NEXT:             let %[[VALUE5:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE4]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_2]], read<i32>(%[[VALUE5]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE4]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<i32>(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(%[[VALUE_r_2]], copy<@type[[TYPE_S]], reason=assign>(va_arg<@type[[TYPE_S]]>(%[[VALUE_ap_2]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_2]]);
// DEFAULT-NEXT:         return copy<@type[[TYPE_S]], reason=return>(read<@type[[TYPE_S]]>(%[[VALUE_r_2]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f3:[0-9]+]] @f3(%[[VALUE_x_3:[0-9]+]] x: i32, ...) -> @type[[TYPE_T]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_3:[0-9]+]] r: @type[[TYPE_T]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap_3:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         while %[[VALUE6:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE7:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_3]]);
// DEFAULT-NEXT:             let %[[VALUE8:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE7]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_3]], read<i32>(%[[VALUE8]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE7]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<i32>(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(%[[VALUE_r_3]], copy<@type[[TYPE_T]], reason=assign>(va_arg<@type[[TYPE_T]]>(%[[VALUE_ap_3]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_3]]);
// DEFAULT-NEXT:         return copy<@type[[TYPE_T]], reason=return>(read<@type[[TYPE_T]]>(%[[VALUE_r_3]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f4:[0-9]+]] @f4(%[[VALUE_x_4:[0-9]+]] x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_4:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         while %[[VALUE9:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE10:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_4]]);
// DEFAULT-NEXT:             let %[[VALUE11:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE10]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_4]], read<i32>(%[[VALUE11]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE10]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<i32>(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:         write<i128>(%[[VALUE_b]], va_arg<i128>(%[[VALUE_ap_4]]));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_4]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f5:[0-9]+]] @f5(%[[VALUE_x_5:[0-9]+]] x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_5:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         while %[[VALUE12:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE13:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_5]]);
// DEFAULT-NEXT:             let %[[VALUE14:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE13]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_5]], read<i32>(%[[VALUE14]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE13]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<i32>(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(%[[VALUE_c]], copy<@type[[TYPE_S]], reason=assign>(va_arg<@type[[TYPE_S]]>(%[[VALUE_ap_5]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_5]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f6:[0-9]+]] @f6(%[[VALUE_x_6:[0-9]+]] x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_6:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         while %[[VALUE15:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE16:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_6]]);
// DEFAULT-NEXT:             let %[[VALUE17:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE16]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_6]], read<i32>(%[[VALUE17]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE16]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<i32>(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(%[[VALUE_d]], copy<@type[[TYPE_T]], reason=assign>(va_arg<@type[[TYPE_T]]>(%[[VALUE_ap_6]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_6]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f7:[0-9]+]] @f7(%[[VALUE_x_7:[0-9]+]] x: i32, ...) -> @type[[TYPE_U]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_4:[0-9]+]] r: @type[[TYPE_U]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap_7:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         while %[[VALUE18:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE19:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_7]]);
// DEFAULT-NEXT:             let %[[VALUE20:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE19]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_7]], read<i32>(%[[VALUE20]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE19]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<f64>(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(%[[VALUE_r_4]], copy<@type[[TYPE_U]], reason=assign>(va_arg<@type[[TYPE_U]]>(%[[VALUE_ap_7]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_7]]);
// DEFAULT-NEXT:         return copy<@type[[TYPE_U]], reason=return>(read<@type[[TYPE_U]]>(%[[VALUE_r_4]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f8:[0-9]+]] @f8(%[[VALUE_x_8:[0-9]+]] x: i32, ...) -> @type[[TYPE_V]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_5:[0-9]+]] r: @type[[TYPE_V]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap_8:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         while %[[VALUE21:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE22:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_8]]);
// DEFAULT-NEXT:             let %[[VALUE23:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE22]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_8]], read<i32>(%[[VALUE23]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE22]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<f64>(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(%[[VALUE_r_5]], copy<@type[[TYPE_V]], reason=assign>(va_arg<@type[[TYPE_V]]>(%[[VALUE_ap_8]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_8]]);
// DEFAULT-NEXT:         return copy<@type[[TYPE_V]], reason=return>(read<@type[[TYPE_V]]>(%[[VALUE_r_5]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f9:[0-9]+]] @f9(%[[VALUE_x_9:[0-9]+]] x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_9:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:         while %[[VALUE24:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE25:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_9]]);
// DEFAULT-NEXT:             let %[[VALUE26:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE25]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_9]], read<i32>(%[[VALUE26]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE25]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<f64>(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(%[[VALUE_e]], copy<@type[[TYPE_U]], reason=assign>(va_arg<@type[[TYPE_U]]>(%[[VALUE_ap_9]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_9]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f10:[0-9]+]] @f10(%[[VALUE_x_10:[0-9]+]] x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_10:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_10]]);
// DEFAULT-NEXT:         while %[[VALUE27:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE28:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_10]]);
// DEFAULT-NEXT:             let %[[VALUE29:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE28]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_10]], read<i32>(%[[VALUE29]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE28]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             va_arg<f64>(%[[VALUE_ap_10]]);
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(%[[VALUE_f]], copy<@type[[TYPE_V]], reason=assign>(va_arg<@type[[TYPE_V]]>(%[[VALUE_ap_10]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_10]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f11:[0-9]+]] @f11(%[[VALUE_x_11:[0-9]+]] x: i32, ...) -> @type[[TYPE_W]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_6:[0-9]+]] r: @type[[TYPE_W]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap_11:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_11]]);
// DEFAULT-NEXT:         while %[[VALUE30:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE31:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_11]]);
// DEFAULT-NEXT:             let %[[VALUE32:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE31]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_11]], read<i32>(%[[VALUE32]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE31]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_arg<i32>(%[[VALUE_ap_11]]);
// DEFAULT-NEXT:                 va_arg<f64>(%[[VALUE_ap_11]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(%[[VALUE_r_6]], copy<@type[[TYPE_W]], reason=assign>(va_arg<@type[[TYPE_W]]>(%[[VALUE_ap_11]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_11]]);
// DEFAULT-NEXT:         return copy<@type[[TYPE_W]], reason=return>(read<@type[[TYPE_W]]>(%[[VALUE_r_6]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f12:[0-9]+]] @f12(%[[VALUE_x_12:[0-9]+]] x: i32, ...) -> @type[[TYPE_X]] [linkage=external] [abi=sysv64(scalar) -> native_c] [fallthrough=ub_if_used] {
// DEFAULT-NEXT:         let %[[VALUE_r_7:[0-9]+]] r: @type[[TYPE_X]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_ap_12:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:         while %[[VALUE33:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE34:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_12]]);
// DEFAULT-NEXT:             let %[[VALUE35:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE34]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_12]], read<i32>(%[[VALUE35]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE34]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_arg<i32>(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:                 va_arg<f64>(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(%[[VALUE_r_7]], copy<@type[[TYPE_X]], reason=assign>(va_arg<@type[[TYPE_X]]>(%[[VALUE_ap_12]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_12]]);
// DEFAULT-NEXT:         return copy<@type[[TYPE_X]], reason=return>(read<@type[[TYPE_X]]>(%[[VALUE_r_7]]));
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f13:[0-9]+]] @f13(%[[VALUE_x_13:[0-9]+]] x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_13:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:         while %[[VALUE36:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE37:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_13]]);
// DEFAULT-NEXT:             let %[[VALUE38:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE37]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_13]], read<i32>(%[[VALUE38]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE37]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_arg<i32>(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:                 va_arg<f64>(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(%[[VALUE_g]], copy<@type[[TYPE_W]], reason=assign>(va_arg<@type[[TYPE_W]]>(%[[VALUE_ap_13]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_13]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE_f14:[0-9]+]] @f14(%[[VALUE_x_14:[0-9]+]] x: i32, ...) -> void [linkage=external] [fallthrough=ret_void] {
// DEFAULT-NEXT:         let %[[VALUE_ap_14:[0-9]+]] ap: va_list [storage=automatic];
// DEFAULT-NEXT:         va_start(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:         while %[[VALUE39:[0-9]+]] {
// DEFAULT-NEXT:             let %[[VALUE40:[0-9]+]]: i32 [synthetic] = read<i32>(%[[VALUE_x_14]]);
// DEFAULT-NEXT:             let %[[VALUE41:[0-9]+]]: i32 [synthetic] = sub<i32, overflow=ub>(read<i32>(%[[VALUE40]]), const<i32>(1));
// DEFAULT-NEXT:             write<i32>(%[[VALUE_x_14]], read<i32>(%[[VALUE41]]));
// DEFAULT-NEXT:             yield ne<i32>(read<i32>(%[[VALUE40]]), const<i32>(0));
// DEFAULT-NEXT:         }
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 va_arg<i32>(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:                 va_arg<f64>(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(%[[VALUE_h]], copy<@type[[TYPE_X]], reason=assign>(va_arg<@type[[TYPE_X]]>(%[[VALUE_ap_14]])));
// DEFAULT-NEXT:         va_end(%[[VALUE_ap_14]]);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT:     fn %[[VALUE___builtin_abort:[0-9]+]] @__builtin_abort() -> void [linkage=external] [noreturn];
// DEFAULT-NEXT:     fn %[[VALUE_main:[0-9]+]] @main() -> i32 [linkage=external] [fallthrough=ret_zero] {
// DEFAULT-NEXT:         let %[[VALUE_u:[0-9]+]] u: @type[[TYPE_Y]] [storage=automatic];
// DEFAULT-NEXT:         let %[[VALUE_v:[0-9]+]] v: @type[[TYPE_Y]] [storage=automatic];
// DEFAULT-NEXT:         write<i64>(field0(field1(%[[VALUE_u]])), reinterpret<i64, reason=assign, fits=always>(const<u64>(6148914691236517205)));
// DEFAULT-NEXT:         write<i64>(field1(field1(%[[VALUE_u]])), reinterpret<i64, reason=assign, fits=unknown>(const<u64>(12297829382473034410)));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), call<i128, signature=fn(i32, ...) -> i128>(%[[VALUE_f1]], const<i32>(0), read<i128>(field0(%[[VALUE_u]]))));
// DEFAULT-NEXT:         do %[[VALUE42:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE43:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE44:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE43]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE44]]));
// DEFAULT-NEXT:                 let %[[VALUE45:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE46:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE45]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE46]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), call<i128, signature=fn(i32, ...) -> i128>(%[[VALUE_f1]], const<i32>(1), const<i32>(0), read<i128>(field0(%[[VALUE_u]]))));
// DEFAULT-NEXT:         do %[[VALUE47:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE48:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE49:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE48]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE49]]));
// DEFAULT-NEXT:                 let %[[VALUE50:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE51:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE50]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE51]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), call<i128, signature=fn(i32, ...) -> i128>(%[[VALUE_f1]], const<i32>(2), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]]))));
// DEFAULT-NEXT:         do %[[VALUE52:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE53:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE54:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE53]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE54]]));
// DEFAULT-NEXT:                 let %[[VALUE55:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE56:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE55]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE56]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), call<i128, signature=fn(i32, ...) -> i128>(%[[VALUE_f1]], const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]]))));
// DEFAULT-NEXT:         do %[[VALUE57:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE58:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE59:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE58]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE59]]));
// DEFAULT-NEXT:                 let %[[VALUE60:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE61:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE60]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE61]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), call<i128, signature=fn(i32, ...) -> i128>(%[[VALUE_f1]], const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]]))));
// DEFAULT-NEXT:         do %[[VALUE62:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE63:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE64:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE63]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE64]]));
// DEFAULT-NEXT:                 let %[[VALUE65:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE66:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE65]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE66]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), call<i128, signature=fn(i32, ...) -> i128>(%[[VALUE_f1]], const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]]))));
// DEFAULT-NEXT:         do %[[VALUE67:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE68:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE69:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE68]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE69]]));
// DEFAULT-NEXT:                 let %[[VALUE70:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE71:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE70]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE71]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), call<i128, signature=fn(i32, ...) -> i128>(%[[VALUE_f1]], const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]]))));
// DEFAULT-NEXT:         do %[[VALUE72:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE73:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE74:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE73]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE74]]));
// DEFAULT-NEXT:                 let %[[VALUE75:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE76:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE75]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE76]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), call<i128, signature=fn(i32, ...) -> i128>(%[[VALUE_f1]], const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]]))));
// DEFAULT-NEXT:         do %[[VALUE77:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE78:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE79:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE78]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE79]]));
// DEFAULT-NEXT:                 let %[[VALUE80:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE81:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE80]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE81]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), call<i128, signature=fn(i32, ...) -> i128>(%[[VALUE_f1]], const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]]))));
// DEFAULT-NEXT:         do %[[VALUE82:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE83:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE84:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE83]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE84]]));
// DEFAULT-NEXT:                 let %[[VALUE85:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE86:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE85]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE86]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), call<i128, signature=fn(i32, ...) -> i128>(%[[VALUE_f1]], const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]]))));
// DEFAULT-NEXT:         do %[[VALUE87:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE88:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE89:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE88]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE89]]));
// DEFAULT-NEXT:                 let %[[VALUE90:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE91:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE90]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE91]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(call<@type[[TYPE_S]], signature=fn(i32, ...) -> @type[[TYPE_S]], abi=sysv64(scalar, native_c) -> native_c>(%[[VALUE_f2]], const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE92:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE93:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE94:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE93]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE94]]));
// DEFAULT-NEXT:                 let %[[VALUE95:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE96:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE95]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE96]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(call<@type[[TYPE_S]], signature=fn(i32, ...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, native_c) -> native_c>(%[[VALUE_f2]], const<i32>(1), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE97:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE98:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE99:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE98]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE99]]));
// DEFAULT-NEXT:                 let %[[VALUE100:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE101:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE100]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE101]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(call<@type[[TYPE_S]], signature=fn(i32, ...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f2]], const<i32>(2), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE102:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE103:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE104:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE103]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE104]]));
// DEFAULT-NEXT:                 let %[[VALUE105:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE106:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE105]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE106]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(call<@type[[TYPE_S]], signature=fn(i32, ...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f2]], const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE107:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE108:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE109:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE108]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE109]]));
// DEFAULT-NEXT:                 let %[[VALUE110:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE111:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE110]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE111]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(call<@type[[TYPE_S]], signature=fn(i32, ...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f2]], const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE112:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE113:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE114:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE113]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE114]]));
// DEFAULT-NEXT:                 let %[[VALUE115:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE116:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE115]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE116]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(call<@type[[TYPE_S]], signature=fn(i32, ...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f2]], const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE117:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE118:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE119:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE118]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE119]]));
// DEFAULT-NEXT:                 let %[[VALUE120:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE121:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE120]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE121]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(call<@type[[TYPE_S]], signature=fn(i32, ...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f2]], const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE122:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE123:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE124:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE123]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE124]]));
// DEFAULT-NEXT:                 let %[[VALUE125:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE126:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE125]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE126]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(call<@type[[TYPE_S]], signature=fn(i32, ...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f2]], const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE127:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE128:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE129:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE128]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE129]]));
// DEFAULT-NEXT:                 let %[[VALUE130:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE131:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE130]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE131]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(call<@type[[TYPE_S]], signature=fn(i32, ...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f2]], const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE132:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE133:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE134:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE133]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE134]]));
// DEFAULT-NEXT:                 let %[[VALUE135:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE136:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE135]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE136]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(call<@type[[TYPE_S]], signature=fn(i32, ...) -> @type[[TYPE_S]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f2]], const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE137:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE138:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE139:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE138]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE139]]));
// DEFAULT-NEXT:                 let %[[VALUE140:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE141:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE140]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE141]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(call<@type[[TYPE_T]], signature=fn(i32, ...) -> @type[[TYPE_T]], abi=sysv64(scalar, native_c) -> native_c>(%[[VALUE_f3]], const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE142:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE143:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE144:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE143]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE144]]));
// DEFAULT-NEXT:                 let %[[VALUE145:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE146:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE145]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE146]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(call<@type[[TYPE_T]], signature=fn(i32, ...) -> @type[[TYPE_T]], abi=sysv64(scalar, scalar, native_c) -> native_c>(%[[VALUE_f3]], const<i32>(1), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE147:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE148:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE149:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE148]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE149]]));
// DEFAULT-NEXT:                 let %[[VALUE150:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE151:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE150]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE151]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(call<@type[[TYPE_T]], signature=fn(i32, ...) -> @type[[TYPE_T]], abi=sysv64(scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f3]], const<i32>(2), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE152:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE153:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE154:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE153]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE154]]));
// DEFAULT-NEXT:                 let %[[VALUE155:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE156:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE155]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE156]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(call<@type[[TYPE_T]], signature=fn(i32, ...) -> @type[[TYPE_T]], abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f3]], const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE157:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE158:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE159:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE158]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE159]]));
// DEFAULT-NEXT:                 let %[[VALUE160:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE161:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE160]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE161]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(call<@type[[TYPE_T]], signature=fn(i32, ...) -> @type[[TYPE_T]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f3]], const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE162:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE163:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE164:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE163]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE164]]));
// DEFAULT-NEXT:                 let %[[VALUE165:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE166:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE165]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE166]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(call<@type[[TYPE_T]], signature=fn(i32, ...) -> @type[[TYPE_T]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f3]], const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE167:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE168:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE169:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE168]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE169]]));
// DEFAULT-NEXT:                 let %[[VALUE170:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE171:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE170]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE171]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(call<@type[[TYPE_T]], signature=fn(i32, ...) -> @type[[TYPE_T]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f3]], const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE172:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE173:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE174:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE173]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE174]]));
// DEFAULT-NEXT:                 let %[[VALUE175:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE176:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE175]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE176]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(call<@type[[TYPE_T]], signature=fn(i32, ...) -> @type[[TYPE_T]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f3]], const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE177:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE178:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE179:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE178]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE179]]));
// DEFAULT-NEXT:                 let %[[VALUE180:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE181:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE180]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE181]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(call<@type[[TYPE_T]], signature=fn(i32, ...) -> @type[[TYPE_T]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f3]], const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE182:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE183:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE184:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE183]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE184]]));
// DEFAULT-NEXT:                 let %[[VALUE185:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE186:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE185]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE186]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(call<@type[[TYPE_T]], signature=fn(i32, ...) -> @type[[TYPE_T]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f3]], const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE187:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE188:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE189:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE188]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE189]]));
// DEFAULT-NEXT:                 let %[[VALUE190:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE191:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE190]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE191]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(0), read<i128>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), read<i128>(%[[VALUE_b]]));
// DEFAULT-NEXT:         do %[[VALUE192:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE193:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE194:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE193]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE194]]));
// DEFAULT-NEXT:                 let %[[VALUE195:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE196:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE195]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE196]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(1), const<i32>(0), read<i128>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), read<i128>(%[[VALUE_b]]));
// DEFAULT-NEXT:         do %[[VALUE197:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE198:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE199:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE198]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE199]]));
// DEFAULT-NEXT:                 let %[[VALUE200:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE201:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE200]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE201]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(2), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), read<i128>(%[[VALUE_b]]));
// DEFAULT-NEXT:         do %[[VALUE202:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE203:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE204:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE203]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE204]]));
// DEFAULT-NEXT:                 let %[[VALUE205:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE206:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE205]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE206]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), read<i128>(%[[VALUE_b]]));
// DEFAULT-NEXT:         do %[[VALUE207:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE208:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE209:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE208]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE209]]));
// DEFAULT-NEXT:                 let %[[VALUE210:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE211:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE210]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE211]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), read<i128>(%[[VALUE_b]]));
// DEFAULT-NEXT:         do %[[VALUE212:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE213:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE214:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE213]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE214]]));
// DEFAULT-NEXT:                 let %[[VALUE215:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE216:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE215]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE216]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), read<i128>(%[[VALUE_b]]));
// DEFAULT-NEXT:         do %[[VALUE217:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE218:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE219:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE218]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE219]]));
// DEFAULT-NEXT:                 let %[[VALUE220:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE221:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE220]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE221]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), read<i128>(%[[VALUE_b]]));
// DEFAULT-NEXT:         do %[[VALUE222:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE223:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE224:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE223]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE224]]));
// DEFAULT-NEXT:                 let %[[VALUE225:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE226:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE225]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE226]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), read<i128>(%[[VALUE_b]]));
// DEFAULT-NEXT:         do %[[VALUE227:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE228:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE229:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE228]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE229]]));
// DEFAULT-NEXT:                 let %[[VALUE230:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE231:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE230]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE231]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), read<i128>(%[[VALUE_b]]));
// DEFAULT-NEXT:         do %[[VALUE232:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE233:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE234:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE233]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE234]]));
// DEFAULT-NEXT:                 let %[[VALUE235:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE236:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE235]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE236]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void>(%[[VALUE_f4]], const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), read<i128>(field0(%[[VALUE_u]])));
// DEFAULT-NEXT:         write<i128>(field0(%[[VALUE_v]]), read<i128>(%[[VALUE_b]]));
// DEFAULT-NEXT:         do %[[VALUE237:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE238:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE239:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE238]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE239]]));
// DEFAULT-NEXT:                 let %[[VALUE240:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE241:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE240]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE241]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c) -> void>(%[[VALUE_f5]], const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_c]])));
// DEFAULT-NEXT:         do %[[VALUE242:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE243:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE244:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE243]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE244]]));
// DEFAULT-NEXT:                 let %[[VALUE245:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE246:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE245]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE246]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, native_c) -> void>(%[[VALUE_f5]], const<i32>(1), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_c]])));
// DEFAULT-NEXT:         do %[[VALUE247:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE248:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE249:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE248]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE249]]));
// DEFAULT-NEXT:                 let %[[VALUE250:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE251:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE250]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE251]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f5]], const<i32>(2), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_c]])));
// DEFAULT-NEXT:         do %[[VALUE252:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE253:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE254:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE253]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE254]]));
// DEFAULT-NEXT:                 let %[[VALUE255:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE256:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE255]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE256]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f5]], const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_c]])));
// DEFAULT-NEXT:         do %[[VALUE257:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE258:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE259:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE258]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE259]]));
// DEFAULT-NEXT:                 let %[[VALUE260:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE261:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE260]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE261]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f5]], const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_c]])));
// DEFAULT-NEXT:         do %[[VALUE262:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE263:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE264:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE263]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE264]]));
// DEFAULT-NEXT:                 let %[[VALUE265:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE266:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE265]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE266]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f5]], const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_c]])));
// DEFAULT-NEXT:         do %[[VALUE267:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE268:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE269:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE268]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE269]]));
// DEFAULT-NEXT:                 let %[[VALUE270:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE271:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE270]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE271]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f5]], const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_c]])));
// DEFAULT-NEXT:         do %[[VALUE272:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE273:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE274:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE273]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE274]]));
// DEFAULT-NEXT:                 let %[[VALUE275:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE276:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE275]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE276]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f5]], const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_c]])));
// DEFAULT-NEXT:         do %[[VALUE277:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE278:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE279:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE278]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE279]]));
// DEFAULT-NEXT:                 let %[[VALUE280:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE281:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE280]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE281]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f5]], const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_c]])));
// DEFAULT-NEXT:         do %[[VALUE282:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE283:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE284:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE283]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE284]]));
// DEFAULT-NEXT:                 let %[[VALUE285:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE286:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE285]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE286]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f5]], const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_S]], reason=vararg>(read<@type[[TYPE_S]]>(field1(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_S]]>(field1(%[[VALUE_v]]), copy<@type[[TYPE_S]], reason=assign>(read<@type[[TYPE_S]]>(%[[VALUE_c]])));
// DEFAULT-NEXT:         do %[[VALUE287:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE288:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE289:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE288]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE289]]));
// DEFAULT-NEXT:                 let %[[VALUE290:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE291:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE290]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE291]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c) -> void>(%[[VALUE_f6]], const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_d]])));
// DEFAULT-NEXT:         do %[[VALUE292:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE293:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE294:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE293]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE294]]));
// DEFAULT-NEXT:                 let %[[VALUE295:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE296:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE295]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE296]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, native_c) -> void>(%[[VALUE_f6]], const<i32>(1), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_d]])));
// DEFAULT-NEXT:         do %[[VALUE297:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE298:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE299:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE298]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE299]]));
// DEFAULT-NEXT:                 let %[[VALUE300:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE301:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE300]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE301]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f6]], const<i32>(2), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_d]])));
// DEFAULT-NEXT:         do %[[VALUE302:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE303:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE304:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE303]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE304]]));
// DEFAULT-NEXT:                 let %[[VALUE305:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE306:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE305]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE306]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f6]], const<i32>(3), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_d]])));
// DEFAULT-NEXT:         do %[[VALUE307:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE308:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE309:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE308]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE309]]));
// DEFAULT-NEXT:                 let %[[VALUE310:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE311:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE310]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE311]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f6]], const<i32>(4), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_d]])));
// DEFAULT-NEXT:         do %[[VALUE312:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE313:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE314:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE313]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE314]]));
// DEFAULT-NEXT:                 let %[[VALUE315:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE316:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE315]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE316]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f6]], const<i32>(5), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_d]])));
// DEFAULT-NEXT:         do %[[VALUE317:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE318:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE319:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE318]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE319]]));
// DEFAULT-NEXT:                 let %[[VALUE320:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE321:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE320]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE321]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f6]], const<i32>(6), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_d]])));
// DEFAULT-NEXT:         do %[[VALUE322:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE323:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE324:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE323]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE324]]));
// DEFAULT-NEXT:                 let %[[VALUE325:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE326:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE325]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE326]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f6]], const<i32>(7), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_d]])));
// DEFAULT-NEXT:         do %[[VALUE327:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE328:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE329:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE328]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE329]]));
// DEFAULT-NEXT:                 let %[[VALUE330:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE331:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE330]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE331]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f6]], const<i32>(8), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_d]])));
// DEFAULT-NEXT:         do %[[VALUE332:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE333:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE334:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE333]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE334]]));
// DEFAULT-NEXT:                 let %[[VALUE335:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE336:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE335]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE336]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f6]], const<i32>(9), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), const<i32>(0), copy<@type[[TYPE_T]], reason=vararg>(read<@type[[TYPE_T]]>(field2(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_T]]>(field2(%[[VALUE_v]]), copy<@type[[TYPE_T]], reason=assign>(read<@type[[TYPE_T]]>(%[[VALUE_d]])));
// DEFAULT-NEXT:         do %[[VALUE337:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<i64>(read<i64>(field0(field1(%[[VALUE_u]]))), read<i64>(field0(field1(%[[VALUE_v]])))), ne<i64>(read<i64>(field1(field1(%[[VALUE_u]]))), read<i64>(field1(field1(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE338:[0-9]+]]: i64 [synthetic] = read<i64>(field0(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE339:[0-9]+]]: i64 [synthetic] = add<i64, overflow=ub>(read<i64>(%[[VALUE338]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field0(field1(%[[VALUE_u]])), read<i64>(%[[VALUE339]]));
// DEFAULT-NEXT:                 let %[[VALUE340:[0-9]+]]: i64 [synthetic] = read<i64>(field1(field1(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE341:[0-9]+]]: i64 [synthetic] = sub<i64, overflow=ub>(read<i64>(%[[VALUE340]]), widen<i64, reason=usual_arith>(const<i32>(1)));
// DEFAULT-NEXT:                 write<i64>(field1(field1(%[[VALUE_u]])), read<i64>(%[[VALUE341]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<f64>(field0(field3(%[[VALUE_u]])), const<f64>(1.25));
// DEFAULT-NEXT:         write<f64>(field1(field3(%[[VALUE_u]])), const<f64>(2.75));
// DEFAULT-NEXT:         write<f64>(field2(field3(%[[VALUE_u]])), neg<f64>(const<f64>(3.5)));
// DEFAULT-NEXT:         write<f64>(field3(field3(%[[VALUE_u]])), neg<f64>(const<f64>(2.0)));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(call<@type[[TYPE_U]], signature=fn(i32, ...) -> @type[[TYPE_U]], abi=sysv64(scalar, native_c) -> native_c>(%[[VALUE_f7]], const<i32>(0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE342:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE343:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE344:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE343]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE344]]));
// DEFAULT-NEXT:                 let %[[VALUE345:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE346:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE345]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE346]]));
// DEFAULT-NEXT:                 let %[[VALUE347:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE348:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE347]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE348]]));
// DEFAULT-NEXT:                 let %[[VALUE349:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE350:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE349]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE350]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(call<@type[[TYPE_U]], signature=fn(i32, ...) -> @type[[TYPE_U]], abi=sysv64(scalar, scalar, native_c) -> native_c>(%[[VALUE_f7]], const<i32>(1), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE351:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE352:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE353:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE352]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE353]]));
// DEFAULT-NEXT:                 let %[[VALUE354:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE355:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE354]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE355]]));
// DEFAULT-NEXT:                 let %[[VALUE356:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE357:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE356]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE357]]));
// DEFAULT-NEXT:                 let %[[VALUE358:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE359:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE358]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE359]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(call<@type[[TYPE_U]], signature=fn(i32, ...) -> @type[[TYPE_U]], abi=sysv64(scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f7]], const<i32>(2), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE360:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE361:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE362:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE361]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE362]]));
// DEFAULT-NEXT:                 let %[[VALUE363:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE364:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE363]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE364]]));
// DEFAULT-NEXT:                 let %[[VALUE365:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE366:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE365]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE366]]));
// DEFAULT-NEXT:                 let %[[VALUE367:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE368:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE367]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE368]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(call<@type[[TYPE_U]], signature=fn(i32, ...) -> @type[[TYPE_U]], abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f7]], const<i32>(3), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE369:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE370:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE371:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE370]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE371]]));
// DEFAULT-NEXT:                 let %[[VALUE372:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE373:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE372]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE373]]));
// DEFAULT-NEXT:                 let %[[VALUE374:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE375:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE374]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE375]]));
// DEFAULT-NEXT:                 let %[[VALUE376:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE377:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE376]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE377]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(call<@type[[TYPE_U]], signature=fn(i32, ...) -> @type[[TYPE_U]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f7]], const<i32>(4), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE378:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE379:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE380:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE379]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE380]]));
// DEFAULT-NEXT:                 let %[[VALUE381:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE382:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE381]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE382]]));
// DEFAULT-NEXT:                 let %[[VALUE383:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE384:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE383]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE384]]));
// DEFAULT-NEXT:                 let %[[VALUE385:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE386:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE385]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE386]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(call<@type[[TYPE_U]], signature=fn(i32, ...) -> @type[[TYPE_U]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f7]], const<i32>(5), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE387:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE388:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE389:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE388]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE389]]));
// DEFAULT-NEXT:                 let %[[VALUE390:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE391:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE390]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE391]]));
// DEFAULT-NEXT:                 let %[[VALUE392:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE393:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE392]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE393]]));
// DEFAULT-NEXT:                 let %[[VALUE394:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE395:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE394]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE395]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(call<@type[[TYPE_U]], signature=fn(i32, ...) -> @type[[TYPE_U]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f7]], const<i32>(6), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE396:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE397:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE398:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE397]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE398]]));
// DEFAULT-NEXT:                 let %[[VALUE399:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE400:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE399]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE400]]));
// DEFAULT-NEXT:                 let %[[VALUE401:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE402:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE401]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE402]]));
// DEFAULT-NEXT:                 let %[[VALUE403:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE404:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE403]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE404]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(call<@type[[TYPE_U]], signature=fn(i32, ...) -> @type[[TYPE_U]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f7]], const<i32>(7), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE405:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE406:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE407:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE406]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE407]]));
// DEFAULT-NEXT:                 let %[[VALUE408:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE409:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE408]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE409]]));
// DEFAULT-NEXT:                 let %[[VALUE410:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE411:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE410]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE411]]));
// DEFAULT-NEXT:                 let %[[VALUE412:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE413:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE412]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE413]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(call<@type[[TYPE_U]], signature=fn(i32, ...) -> @type[[TYPE_U]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f7]], const<i32>(8), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE414:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE415:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE416:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE415]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE416]]));
// DEFAULT-NEXT:                 let %[[VALUE417:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE418:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE417]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE418]]));
// DEFAULT-NEXT:                 let %[[VALUE419:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE420:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE419]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE420]]));
// DEFAULT-NEXT:                 let %[[VALUE421:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE422:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE421]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE422]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(call<@type[[TYPE_U]], signature=fn(i32, ...) -> @type[[TYPE_U]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f7]], const<i32>(9), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE423:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE424:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE425:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE424]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE425]]));
// DEFAULT-NEXT:                 let %[[VALUE426:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE427:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE426]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE427]]));
// DEFAULT-NEXT:                 let %[[VALUE428:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE429:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE428]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE429]]));
// DEFAULT-NEXT:                 let %[[VALUE430:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE431:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE430]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE431]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(call<@type[[TYPE_V]], signature=fn(i32, ...) -> @type[[TYPE_V]], abi=sysv64(scalar, native_c) -> native_c>(%[[VALUE_f8]], const<i32>(0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE432:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE433:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE434:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE433]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE434]]));
// DEFAULT-NEXT:                 let %[[VALUE435:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE436:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE435]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE436]]));
// DEFAULT-NEXT:                 let %[[VALUE437:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE438:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE437]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE438]]));
// DEFAULT-NEXT:                 let %[[VALUE439:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE440:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE439]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE440]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(call<@type[[TYPE_V]], signature=fn(i32, ...) -> @type[[TYPE_V]], abi=sysv64(scalar, scalar, native_c) -> native_c>(%[[VALUE_f8]], const<i32>(1), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE441:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE442:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE443:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE442]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE443]]));
// DEFAULT-NEXT:                 let %[[VALUE444:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE445:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE444]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE445]]));
// DEFAULT-NEXT:                 let %[[VALUE446:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE447:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE446]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE447]]));
// DEFAULT-NEXT:                 let %[[VALUE448:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE449:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE448]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE449]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(call<@type[[TYPE_V]], signature=fn(i32, ...) -> @type[[TYPE_V]], abi=sysv64(scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f8]], const<i32>(2), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE450:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE451:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE452:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE451]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE452]]));
// DEFAULT-NEXT:                 let %[[VALUE453:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE454:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE453]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE454]]));
// DEFAULT-NEXT:                 let %[[VALUE455:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE456:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE455]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE456]]));
// DEFAULT-NEXT:                 let %[[VALUE457:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE458:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE457]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE458]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(call<@type[[TYPE_V]], signature=fn(i32, ...) -> @type[[TYPE_V]], abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f8]], const<i32>(3), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE459:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE460:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE461:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE460]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE461]]));
// DEFAULT-NEXT:                 let %[[VALUE462:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE463:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE462]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE463]]));
// DEFAULT-NEXT:                 let %[[VALUE464:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE465:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE464]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE465]]));
// DEFAULT-NEXT:                 let %[[VALUE466:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE467:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE466]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE467]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(call<@type[[TYPE_V]], signature=fn(i32, ...) -> @type[[TYPE_V]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f8]], const<i32>(4), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE468:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE469:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE470:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE469]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE470]]));
// DEFAULT-NEXT:                 let %[[VALUE471:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE472:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE471]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE472]]));
// DEFAULT-NEXT:                 let %[[VALUE473:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE474:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE473]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE474]]));
// DEFAULT-NEXT:                 let %[[VALUE475:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE476:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE475]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE476]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(call<@type[[TYPE_V]], signature=fn(i32, ...) -> @type[[TYPE_V]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f8]], const<i32>(5), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE477:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE478:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE479:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE478]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE479]]));
// DEFAULT-NEXT:                 let %[[VALUE480:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE481:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE480]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE481]]));
// DEFAULT-NEXT:                 let %[[VALUE482:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE483:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE482]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE483]]));
// DEFAULT-NEXT:                 let %[[VALUE484:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE485:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE484]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE485]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(call<@type[[TYPE_V]], signature=fn(i32, ...) -> @type[[TYPE_V]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f8]], const<i32>(6), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE486:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE487:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE488:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE487]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE488]]));
// DEFAULT-NEXT:                 let %[[VALUE489:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE490:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE489]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE490]]));
// DEFAULT-NEXT:                 let %[[VALUE491:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE492:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE491]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE492]]));
// DEFAULT-NEXT:                 let %[[VALUE493:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE494:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE493]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE494]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(call<@type[[TYPE_V]], signature=fn(i32, ...) -> @type[[TYPE_V]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f8]], const<i32>(7), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE495:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE496:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE497:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE496]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE497]]));
// DEFAULT-NEXT:                 let %[[VALUE498:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE499:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE498]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE499]]));
// DEFAULT-NEXT:                 let %[[VALUE500:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE501:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE500]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE501]]));
// DEFAULT-NEXT:                 let %[[VALUE502:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE503:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE502]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE503]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(call<@type[[TYPE_V]], signature=fn(i32, ...) -> @type[[TYPE_V]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f8]], const<i32>(8), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE504:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE505:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE506:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE505]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE506]]));
// DEFAULT-NEXT:                 let %[[VALUE507:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE508:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE507]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE508]]));
// DEFAULT-NEXT:                 let %[[VALUE509:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE510:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE509]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE510]]));
// DEFAULT-NEXT:                 let %[[VALUE511:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE512:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE511]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE512]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(call<@type[[TYPE_V]], signature=fn(i32, ...) -> @type[[TYPE_V]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f8]], const<i32>(9), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE513:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE514:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE515:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE514]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE515]]));
// DEFAULT-NEXT:                 let %[[VALUE516:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE517:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE516]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE517]]));
// DEFAULT-NEXT:                 let %[[VALUE518:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE519:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE518]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE519]]));
// DEFAULT-NEXT:                 let %[[VALUE520:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE521:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE520]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE521]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c) -> void>(%[[VALUE_f9]], const<i32>(0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_e]])));
// DEFAULT-NEXT:         do %[[VALUE522:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE523:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE524:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE523]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE524]]));
// DEFAULT-NEXT:                 let %[[VALUE525:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE526:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE525]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE526]]));
// DEFAULT-NEXT:                 let %[[VALUE527:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE528:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE527]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE528]]));
// DEFAULT-NEXT:                 let %[[VALUE529:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE530:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE529]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE530]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, native_c) -> void>(%[[VALUE_f9]], const<i32>(1), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_e]])));
// DEFAULT-NEXT:         do %[[VALUE531:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE532:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE533:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE532]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE533]]));
// DEFAULT-NEXT:                 let %[[VALUE534:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE535:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE534]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE535]]));
// DEFAULT-NEXT:                 let %[[VALUE536:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE537:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE536]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE537]]));
// DEFAULT-NEXT:                 let %[[VALUE538:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE539:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE538]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE539]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f9]], const<i32>(2), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_e]])));
// DEFAULT-NEXT:         do %[[VALUE540:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE541:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE542:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE541]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE542]]));
// DEFAULT-NEXT:                 let %[[VALUE543:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE544:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE543]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE544]]));
// DEFAULT-NEXT:                 let %[[VALUE545:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE546:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE545]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE546]]));
// DEFAULT-NEXT:                 let %[[VALUE547:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE548:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE547]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE548]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f9]], const<i32>(3), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_e]])));
// DEFAULT-NEXT:         do %[[VALUE549:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE550:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE551:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE550]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE551]]));
// DEFAULT-NEXT:                 let %[[VALUE552:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE553:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE552]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE553]]));
// DEFAULT-NEXT:                 let %[[VALUE554:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE555:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE554]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE555]]));
// DEFAULT-NEXT:                 let %[[VALUE556:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE557:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE556]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE557]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f9]], const<i32>(4), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_e]])));
// DEFAULT-NEXT:         do %[[VALUE558:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE559:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE560:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE559]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE560]]));
// DEFAULT-NEXT:                 let %[[VALUE561:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE562:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE561]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE562]]));
// DEFAULT-NEXT:                 let %[[VALUE563:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE564:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE563]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE564]]));
// DEFAULT-NEXT:                 let %[[VALUE565:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE566:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE565]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE566]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f9]], const<i32>(5), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_e]])));
// DEFAULT-NEXT:         do %[[VALUE567:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE568:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE569:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE568]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE569]]));
// DEFAULT-NEXT:                 let %[[VALUE570:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE571:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE570]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE571]]));
// DEFAULT-NEXT:                 let %[[VALUE572:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE573:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE572]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE573]]));
// DEFAULT-NEXT:                 let %[[VALUE574:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE575:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE574]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE575]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f9]], const<i32>(6), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_e]])));
// DEFAULT-NEXT:         do %[[VALUE576:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE577:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE578:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE577]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE578]]));
// DEFAULT-NEXT:                 let %[[VALUE579:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE580:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE579]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE580]]));
// DEFAULT-NEXT:                 let %[[VALUE581:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE582:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE581]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE582]]));
// DEFAULT-NEXT:                 let %[[VALUE583:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE584:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE583]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE584]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f9]], const<i32>(7), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_e]])));
// DEFAULT-NEXT:         do %[[VALUE585:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE586:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE587:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE586]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE587]]));
// DEFAULT-NEXT:                 let %[[VALUE588:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE589:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE588]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE589]]));
// DEFAULT-NEXT:                 let %[[VALUE590:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE591:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE590]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE591]]));
// DEFAULT-NEXT:                 let %[[VALUE592:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE593:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE592]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE593]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f9]], const<i32>(8), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_e]])));
// DEFAULT-NEXT:         do %[[VALUE594:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE595:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE596:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE595]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE596]]));
// DEFAULT-NEXT:                 let %[[VALUE597:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE598:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE597]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE598]]));
// DEFAULT-NEXT:                 let %[[VALUE599:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE600:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE599]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE600]]));
// DEFAULT-NEXT:                 let %[[VALUE601:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE602:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE601]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE602]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f9]], const<i32>(9), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_U]], reason=vararg>(read<@type[[TYPE_U]]>(field3(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_U]]>(field3(%[[VALUE_v]]), copy<@type[[TYPE_U]], reason=assign>(read<@type[[TYPE_U]]>(%[[VALUE_e]])));
// DEFAULT-NEXT:         do %[[VALUE603:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE604:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE605:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE604]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE605]]));
// DEFAULT-NEXT:                 let %[[VALUE606:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE607:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE606]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE607]]));
// DEFAULT-NEXT:                 let %[[VALUE608:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE609:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE608]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE609]]));
// DEFAULT-NEXT:                 let %[[VALUE610:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE611:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE610]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE611]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c) -> void>(%[[VALUE_f10]], const<i32>(0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_f]])));
// DEFAULT-NEXT:         do %[[VALUE612:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE613:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE614:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE613]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE614]]));
// DEFAULT-NEXT:                 let %[[VALUE615:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE616:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE615]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE616]]));
// DEFAULT-NEXT:                 let %[[VALUE617:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE618:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE617]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE618]]));
// DEFAULT-NEXT:                 let %[[VALUE619:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE620:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE619]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE620]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, native_c) -> void>(%[[VALUE_f10]], const<i32>(1), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_f]])));
// DEFAULT-NEXT:         do %[[VALUE621:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE622:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE623:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE622]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE623]]));
// DEFAULT-NEXT:                 let %[[VALUE624:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE625:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE624]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE625]]));
// DEFAULT-NEXT:                 let %[[VALUE626:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE627:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE626]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE627]]));
// DEFAULT-NEXT:                 let %[[VALUE628:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE629:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE628]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE629]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f10]], const<i32>(2), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_f]])));
// DEFAULT-NEXT:         do %[[VALUE630:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE631:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE632:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE631]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE632]]));
// DEFAULT-NEXT:                 let %[[VALUE633:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE634:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE633]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE634]]));
// DEFAULT-NEXT:                 let %[[VALUE635:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE636:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE635]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE636]]));
// DEFAULT-NEXT:                 let %[[VALUE637:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE638:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE637]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE638]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f10]], const<i32>(3), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_f]])));
// DEFAULT-NEXT:         do %[[VALUE639:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE640:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE641:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE640]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE641]]));
// DEFAULT-NEXT:                 let %[[VALUE642:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE643:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE642]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE643]]));
// DEFAULT-NEXT:                 let %[[VALUE644:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE645:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE644]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE645]]));
// DEFAULT-NEXT:                 let %[[VALUE646:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE647:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE646]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE647]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f10]], const<i32>(4), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_f]])));
// DEFAULT-NEXT:         do %[[VALUE648:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE649:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE650:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE649]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE650]]));
// DEFAULT-NEXT:                 let %[[VALUE651:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE652:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE651]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE652]]));
// DEFAULT-NEXT:                 let %[[VALUE653:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE654:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE653]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE654]]));
// DEFAULT-NEXT:                 let %[[VALUE655:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE656:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE655]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE656]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f10]], const<i32>(5), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_f]])));
// DEFAULT-NEXT:         do %[[VALUE657:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE658:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE659:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE658]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE659]]));
// DEFAULT-NEXT:                 let %[[VALUE660:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE661:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE660]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE661]]));
// DEFAULT-NEXT:                 let %[[VALUE662:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE663:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE662]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE663]]));
// DEFAULT-NEXT:                 let %[[VALUE664:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE665:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE664]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE665]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f10]], const<i32>(6), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_f]])));
// DEFAULT-NEXT:         do %[[VALUE666:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE667:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE668:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE667]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE668]]));
// DEFAULT-NEXT:                 let %[[VALUE669:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE670:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE669]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE670]]));
// DEFAULT-NEXT:                 let %[[VALUE671:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE672:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE671]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE672]]));
// DEFAULT-NEXT:                 let %[[VALUE673:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE674:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE673]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE674]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f10]], const<i32>(7), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_f]])));
// DEFAULT-NEXT:         do %[[VALUE675:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE676:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE677:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE676]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE677]]));
// DEFAULT-NEXT:                 let %[[VALUE678:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE679:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE678]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE679]]));
// DEFAULT-NEXT:                 let %[[VALUE680:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE681:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE680]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE681]]));
// DEFAULT-NEXT:                 let %[[VALUE682:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE683:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE682]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE683]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f10]], const<i32>(8), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_f]])));
// DEFAULT-NEXT:         do %[[VALUE684:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE685:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE686:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE685]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE686]]));
// DEFAULT-NEXT:                 let %[[VALUE687:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE688:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE687]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE688]]));
// DEFAULT-NEXT:                 let %[[VALUE689:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE690:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE689]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE690]]));
// DEFAULT-NEXT:                 let %[[VALUE691:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE692:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE691]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE692]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f10]], const<i32>(9), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), const<f64>(0.0), copy<@type[[TYPE_V]], reason=vararg>(read<@type[[TYPE_V]]>(field4(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_V]]>(field4(%[[VALUE_v]]), copy<@type[[TYPE_V]], reason=assign>(read<@type[[TYPE_V]]>(%[[VALUE_f]])));
// DEFAULT-NEXT:         do %[[VALUE693:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(logical_or<bool>(logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field2(field3(%[[VALUE_u]]))), read<f64>(field2(field3(%[[VALUE_v]]))))), ne<f64, exceptions=observable>(read<f64>(field3(field3(%[[VALUE_u]]))), read<f64>(field3(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE694:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE695:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE694]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE695]]));
// DEFAULT-NEXT:                 let %[[VALUE696:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE697:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE696]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE697]]));
// DEFAULT-NEXT:                 let %[[VALUE698:[0-9]+]]: f64 [synthetic] = read<f64>(field2(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE699:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE698]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field2(field3(%[[VALUE_u]])), read<f64>(%[[VALUE699]]));
// DEFAULT-NEXT:                 let %[[VALUE700:[0-9]+]]: f64 [synthetic] = read<f64>(field3(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE701:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE700]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field3(field3(%[[VALUE_u]])), read<f64>(%[[VALUE701]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<f64>(field0(field5(%[[VALUE_u]])), const<f64>(9.5));
// DEFAULT-NEXT:         write<i64>(field1(field5(%[[VALUE_u]])), reinterpret<i64, reason=assign, fits=always>(const<u64>(6148914691236517205)));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(call<@type[[TYPE_W]], signature=fn(i32, ...) -> @type[[TYPE_W]], abi=sysv64(scalar, native_c) -> native_c>(%[[VALUE_f11]], const<i32>(0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE702:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE703:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE704:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE703]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE704]]));
// DEFAULT-NEXT:                 let %[[VALUE705:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE706:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE705]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE706]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(call<@type[[TYPE_W]], signature=fn(i32, ...) -> @type[[TYPE_W]], abi=sysv64(scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f11]], const<i32>(1), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE707:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE708:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE709:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE708]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE709]]));
// DEFAULT-NEXT:                 let %[[VALUE710:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE711:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE710]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE711]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(call<@type[[TYPE_W]], signature=fn(i32, ...) -> @type[[TYPE_W]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f11]], const<i32>(2), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE712:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE713:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE714:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE713]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE714]]));
// DEFAULT-NEXT:                 let %[[VALUE715:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE716:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE715]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE716]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(call<@type[[TYPE_W]], signature=fn(i32, ...) -> @type[[TYPE_W]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f11]], const<i32>(3), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE717:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE718:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE719:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE718]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE719]]));
// DEFAULT-NEXT:                 let %[[VALUE720:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE721:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE720]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE721]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(call<@type[[TYPE_W]], signature=fn(i32, ...) -> @type[[TYPE_W]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f11]], const<i32>(4), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE722:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE723:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE724:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE723]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE724]]));
// DEFAULT-NEXT:                 let %[[VALUE725:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE726:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE725]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE726]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(call<@type[[TYPE_W]], signature=fn(i32, ...) -> @type[[TYPE_W]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f11]], const<i32>(5), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE727:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE728:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE729:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE728]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE729]]));
// DEFAULT-NEXT:                 let %[[VALUE730:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE731:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE730]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE731]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(call<@type[[TYPE_W]], signature=fn(i32, ...) -> @type[[TYPE_W]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f11]], const<i32>(6), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE732:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE733:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE734:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE733]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE734]]));
// DEFAULT-NEXT:                 let %[[VALUE735:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE736:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE735]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE736]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(call<@type[[TYPE_W]], signature=fn(i32, ...) -> @type[[TYPE_W]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f11]], const<i32>(7), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE737:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE738:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE739:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE738]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE739]]));
// DEFAULT-NEXT:                 let %[[VALUE740:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE741:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE740]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE741]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(call<@type[[TYPE_W]], signature=fn(i32, ...) -> @type[[TYPE_W]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f11]], const<i32>(8), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE742:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE743:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE744:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE743]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE744]]));
// DEFAULT-NEXT:                 let %[[VALUE745:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE746:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE745]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE746]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(call<@type[[TYPE_W]], signature=fn(i32, ...) -> @type[[TYPE_W]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f11]], const<i32>(9), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE747:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE748:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE749:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE748]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE749]]));
// DEFAULT-NEXT:                 let %[[VALUE750:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE751:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE750]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE751]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(call<@type[[TYPE_X]], signature=fn(i32, ...) -> @type[[TYPE_X]], abi=sysv64(scalar, native_c) -> native_c>(%[[VALUE_f12]], const<i32>(0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE752:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE753:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE754:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE753]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE754]]));
// DEFAULT-NEXT:                 let %[[VALUE755:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE756:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE755]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE756]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(call<@type[[TYPE_X]], signature=fn(i32, ...) -> @type[[TYPE_X]], abi=sysv64(scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f12]], const<i32>(1), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE757:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE758:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE759:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE758]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE759]]));
// DEFAULT-NEXT:                 let %[[VALUE760:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE761:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE760]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE761]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(call<@type[[TYPE_X]], signature=fn(i32, ...) -> @type[[TYPE_X]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f12]], const<i32>(2), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE762:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE763:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE764:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE763]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE764]]));
// DEFAULT-NEXT:                 let %[[VALUE765:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE766:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE765]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE766]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(call<@type[[TYPE_X]], signature=fn(i32, ...) -> @type[[TYPE_X]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f12]], const<i32>(3), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE767:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE768:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE769:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE768]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE769]]));
// DEFAULT-NEXT:                 let %[[VALUE770:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE771:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE770]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE771]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(call<@type[[TYPE_X]], signature=fn(i32, ...) -> @type[[TYPE_X]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f12]], const<i32>(4), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE772:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE773:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE774:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE773]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE774]]));
// DEFAULT-NEXT:                 let %[[VALUE775:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE776:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE775]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE776]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(call<@type[[TYPE_X]], signature=fn(i32, ...) -> @type[[TYPE_X]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f12]], const<i32>(5), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE777:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE778:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE779:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE778]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE779]]));
// DEFAULT-NEXT:                 let %[[VALUE780:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE781:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE780]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE781]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(call<@type[[TYPE_X]], signature=fn(i32, ...) -> @type[[TYPE_X]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f12]], const<i32>(6), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE782:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE783:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE784:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE783]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE784]]));
// DEFAULT-NEXT:                 let %[[VALUE785:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE786:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE785]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE786]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(call<@type[[TYPE_X]], signature=fn(i32, ...) -> @type[[TYPE_X]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f12]], const<i32>(7), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE787:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE788:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE789:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE788]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE789]]));
// DEFAULT-NEXT:                 let %[[VALUE790:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE791:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE790]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE791]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(call<@type[[TYPE_X]], signature=fn(i32, ...) -> @type[[TYPE_X]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f12]], const<i32>(8), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE792:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE793:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE794:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE793]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE794]]));
// DEFAULT-NEXT:                 let %[[VALUE795:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE796:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE795]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE796]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(call<@type[[TYPE_X]], signature=fn(i32, ...) -> @type[[TYPE_X]], abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> native_c>(%[[VALUE_f12]], const<i32>(9), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))))));
// DEFAULT-NEXT:         do %[[VALUE797:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE798:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE799:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE798]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE799]]));
// DEFAULT-NEXT:                 let %[[VALUE800:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE801:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE800]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE801]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c) -> void>(%[[VALUE_f13]], const<i32>(0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_g]])));
// DEFAULT-NEXT:         do %[[VALUE802:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE803:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE804:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE803]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE804]]));
// DEFAULT-NEXT:                 let %[[VALUE805:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE806:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE805]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE806]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f13]], const<i32>(1), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_g]])));
// DEFAULT-NEXT:         do %[[VALUE807:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE808:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE809:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE808]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE809]]));
// DEFAULT-NEXT:                 let %[[VALUE810:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE811:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE810]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE811]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f13]], const<i32>(2), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_g]])));
// DEFAULT-NEXT:         do %[[VALUE812:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE813:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE814:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE813]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE814]]));
// DEFAULT-NEXT:                 let %[[VALUE815:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE816:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE815]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE816]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f13]], const<i32>(3), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_g]])));
// DEFAULT-NEXT:         do %[[VALUE817:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE818:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE819:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE818]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE819]]));
// DEFAULT-NEXT:                 let %[[VALUE820:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE821:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE820]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE821]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f13]], const<i32>(4), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_g]])));
// DEFAULT-NEXT:         do %[[VALUE822:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE823:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE824:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE823]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE824]]));
// DEFAULT-NEXT:                 let %[[VALUE825:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE826:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE825]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE826]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f13]], const<i32>(5), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_g]])));
// DEFAULT-NEXT:         do %[[VALUE827:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE828:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE829:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE828]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE829]]));
// DEFAULT-NEXT:                 let %[[VALUE830:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE831:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE830]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE831]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f13]], const<i32>(6), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_g]])));
// DEFAULT-NEXT:         do %[[VALUE832:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE833:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE834:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE833]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE834]]));
// DEFAULT-NEXT:                 let %[[VALUE835:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE836:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE835]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE836]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f13]], const<i32>(7), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_g]])));
// DEFAULT-NEXT:         do %[[VALUE837:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE838:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE839:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE838]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE839]]));
// DEFAULT-NEXT:                 let %[[VALUE840:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE841:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE840]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE841]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f13]], const<i32>(8), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_g]])));
// DEFAULT-NEXT:         do %[[VALUE842:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE843:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE844:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE843]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE844]]));
// DEFAULT-NEXT:                 let %[[VALUE845:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE846:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE845]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE846]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f13]], const<i32>(9), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_W]], reason=vararg>(read<@type[[TYPE_W]]>(field5(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_W]]>(field5(%[[VALUE_v]]), copy<@type[[TYPE_W]], reason=assign>(read<@type[[TYPE_W]]>(%[[VALUE_g]])));
// DEFAULT-NEXT:         do %[[VALUE847:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE848:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE849:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE848]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE849]]));
// DEFAULT-NEXT:                 let %[[VALUE850:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE851:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE850]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE851]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, native_c) -> void>(%[[VALUE_f14]], const<i32>(0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_h]])));
// DEFAULT-NEXT:         do %[[VALUE852:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE853:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE854:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE853]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE854]]));
// DEFAULT-NEXT:                 let %[[VALUE855:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE856:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE855]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE856]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f14]], const<i32>(1), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_h]])));
// DEFAULT-NEXT:         do %[[VALUE857:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE858:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE859:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE858]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE859]]));
// DEFAULT-NEXT:                 let %[[VALUE860:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE861:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE860]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE861]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f14]], const<i32>(2), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_h]])));
// DEFAULT-NEXT:         do %[[VALUE862:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE863:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE864:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE863]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE864]]));
// DEFAULT-NEXT:                 let %[[VALUE865:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE866:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE865]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE866]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f14]], const<i32>(3), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_h]])));
// DEFAULT-NEXT:         do %[[VALUE867:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE868:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE869:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE868]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE869]]));
// DEFAULT-NEXT:                 let %[[VALUE870:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE871:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE870]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE871]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f14]], const<i32>(4), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_h]])));
// DEFAULT-NEXT:         do %[[VALUE872:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE873:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE874:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE873]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE874]]));
// DEFAULT-NEXT:                 let %[[VALUE875:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE876:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE875]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE876]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f14]], const<i32>(5), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_h]])));
// DEFAULT-NEXT:         do %[[VALUE877:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE878:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE879:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE878]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE879]]));
// DEFAULT-NEXT:                 let %[[VALUE880:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE881:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE880]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE881]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f14]], const<i32>(6), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_h]])));
// DEFAULT-NEXT:         do %[[VALUE882:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE883:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE884:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE883]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE884]]));
// DEFAULT-NEXT:                 let %[[VALUE885:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE886:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE885]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE886]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f14]], const<i32>(7), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_h]])));
// DEFAULT-NEXT:         do %[[VALUE887:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE888:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE889:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE888]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE889]]));
// DEFAULT-NEXT:                 let %[[VALUE890:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE891:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE890]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE891]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f14]], const<i32>(8), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_h]])));
// DEFAULT-NEXT:         do %[[VALUE892:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE893:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE894:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE893]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE894]]));
// DEFAULT-NEXT:                 let %[[VALUE895:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE896:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE895]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE896]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         call<void, signature=fn(i32, ...) -> void, abi=sysv64(scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, scalar, native_c) -> void>(%[[VALUE_f14]], const<i32>(9), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), const<i32>(0), const<f64>(0.0), copy<@type[[TYPE_X]], reason=vararg>(read<@type[[TYPE_X]]>(field6(%[[VALUE_u]]))));
// DEFAULT-NEXT:         write<@type[[TYPE_X]]>(field6(%[[VALUE_v]]), copy<@type[[TYPE_X]], reason=assign>(read<@type[[TYPE_X]]>(%[[VALUE_h]])));
// DEFAULT-NEXT:         do %[[VALUE897:[0-9]+]]
// DEFAULT-NEXT:             {
// DEFAULT-NEXT:                 if logical_or<bool>(ne<f64, exceptions=observable>(read<f64>(field0(field3(%[[VALUE_u]]))), read<f64>(field0(field3(%[[VALUE_v]])))), ne<f64, exceptions=observable>(read<f64>(field1(field3(%[[VALUE_u]]))), read<f64>(field1(field3(%[[VALUE_v]])))))
// DEFAULT-NEXT:                     call<void, signature=fn() -> void>(%[[VALUE___builtin_abort]]);
// DEFAULT-NEXT:                 let %[[VALUE898:[0-9]+]]: f64 [synthetic] = read<f64>(field0(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE899:[0-9]+]]: f64 [synthetic] = add<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE898]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field0(field3(%[[VALUE_u]])), read<f64>(%[[VALUE899]]));
// DEFAULT-NEXT:                 let %[[VALUE900:[0-9]+]]: f64 [synthetic] = read<f64>(field1(field3(%[[VALUE_u]])));
// DEFAULT-NEXT:                 let %[[VALUE901:[0-9]+]]: f64 [synthetic] = sub<f64, rounding=nearest_even, exceptions=observable, contract=fast>(read<f64>(%[[VALUE900]]), int_to_float<f64, reason=usual_arith, exact=true, rounding=nearest_even, exceptions=observable>(const<i32>(1)));
// DEFAULT-NEXT:                 write<f64>(field1(field3(%[[VALUE_u]])), read<f64>(%[[VALUE901]]));
// DEFAULT-NEXT:             }
// DEFAULT-NEXT:         while ne<i32>(const<i32>(0), const<i32>(0));
// DEFAULT-NEXT:         return const<i32>(0);
// DEFAULT-NEXT:     }
// DEFAULT-NEXT: }
// SLATE-FILECHECK-END DEFAULT
