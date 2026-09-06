/* Test _Float16.  */
/* { dg-do run } */
/* { dg-options "-Wno-old-style-definition" } */
/* { dg-add-options float16 } */
/* { dg-require-effective-target float16_runtime } */

#define WIDTH 16
#define EXT   0
/* Basic tests for _FloatN / _FloatNx types: compile and execution
   tests for valid code.  Before including this file, define WIDTH as
   the value N; define EXT to 1 for _FloatNx and 0 for _FloatN.  */

#include <stdarg.h>

#define CONCATX(X, Y)       X##Y
#define CONCAT(X, Y)        CONCATX(X, Y)
#define CONCAT3(X, Y, Z)    CONCAT(CONCAT(X, Y), Z)
#define CONCAT4(W, X, Y, Z) CONCAT(CONCAT(CONCAT(W, X), Y), Z)

#ifndef TYPE
#if EXT
#define TYPE    CONCAT3(_Float, WIDTH, x)
#define CST(C)  CONCAT4(C, f, WIDTH, x)
#define CSTU(C) CONCAT4(C, F, WIDTH, x)
#else
#define TYPE    CONCAT(_Float, WIDTH)
#define CST(C)  CONCAT3(C, f, WIDTH)
#define CSTU(C) CONCAT3(C, F, WIDTH)
#endif
#endif

extern void exit(int);
extern void abort(void);

volatile TYPE a = CST(1.0), b = CSTU(2.5), c = -CST(2.5);
volatile TYPE a2 = CST(1.0), z = CST(0.0), nz = -CST(0.0);

/* These types are not subject to default argument promotions.  */

TYPE vafn(TYPE arg1, ...) {
  va_list ap;
  TYPE    ret;
  va_start(ap, arg1);
  ret = arg1 + va_arg(ap, TYPE);
  va_end(ap);
  return ret;
}

TYPE krfn(TYPE arg) {
  return arg + 1;
}

TYPE krprofn(TYPE);
TYPE krprofn(TYPE arg) {
  return arg * 3;
}

TYPE profn(TYPE arg) { return arg / 4; }

// @lowering-fn-begin
// @rewrite-fn-begin
int main(void) {
  volatile TYPE r;
  r = -b;
  if (r != c)
    abort();
  r = a + b;
  if (r != CST(3.5))
    abort();
  r = a - b;
  if (r != -CST(1.5))
    abort();
  r = 2 * c;
  if (r != -5)
    abort();
  r = b * c;
  if (r != -CST(6.25))
    abort();
  r = b / (a + a);
  if (r != CST(1.25))
    abort();
  r = c * 3;
  if (r != -CST(7.5))
    abort();
  volatile int i = r;
  if (i != -7)
    abort();
  r = vafn(a, c);
  if (r != -CST(1.5))
    abort();
  r = krfn(b);
  if (r != CST(3.5))
    abort();
  r = krprofn(a);
  if (r != CST(3.0))
    abort();
  r = profn(a);
  if (r != CST(0.25))
    abort();
  if ((a < b) != 1)
    abort();
  if ((b < a) != 0)
    abort();
  if ((a < a2) != 0)
    abort();
  if ((nz < z) != 0)
    abort();
  if ((a <= b) != 1)
    abort();
  if ((b <= a) != 0)
    abort();
  if ((a <= a2) != 1)
    abort();
  if ((nz <= z) != 1)
    abort();
  if ((a > b) != 0)
    abort();
  if ((b > a) != 1)
    abort();
  if ((a > a2) != 0)
    abort();
  if ((nz > z) != 0)
    abort();
  if ((a >= b) != 0)
    abort();
  if ((b >= a) != 1)
    abort();
  if ((a >= a2) != 1)
    abort();
  if ((nz >= z) != 1)
    abort();
  i = (nz == z);
  if (i != 1)
    abort();
  i = (a == b);
  if (i != 0)
    abort();
  exit(0);
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let mut r: f16 = 0.0f16;
// LOWERING-DAG:     let mut i: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = -{{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = 3.5;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} - {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = 1.5;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = -{{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 2;
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -5;
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = 6.25;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = -{{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} / {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = 1.25;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = 7.5;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = -{{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(i), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -7;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { vafn({{__v[0-9]+}}, __SlateVaArgs::new(vec![__SlateVaArg::new({{__v[0-9]+}})])) };
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = 1.5;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = -{{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = {{__v[0-9]+}} as f16;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = krfn({{__v[0-9]+}});
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = 3.5;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = krprofn({{__v[0-9]+}});
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = 3.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = profn({{__v[0-9]+}});
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = 0.25;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a2)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nz)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(z)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a2)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nz)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(z)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} <= {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a2)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nz)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(z)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a2)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nz)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(z)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nz)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(z)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(i), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(i), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     unsafe { exit({{__v[0-9]+}} as i32) };
// LOWERING-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut r: f16 = 0.0f16;
// REWRITES-DAG:     let mut i: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), -({{__v[0-9]+}} as f32) as f16) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), ({{__v[0-9]+}} + ({{__v[0-9]+}} as f32)) as f16) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 3.5;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), ({{__v[0-9]+}} - ({{__v[0-9]+}} as f32)) as f16) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != (-((1.5 as f16) as f32) as f16);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         std::ptr::write_volatile(
// REWRITES-DAG:             std::ptr::addr_of_mut!(r),
// REWRITES-DAG:             ((((2 as i32) as f16) as f32) * ({{__v[0-9]+}} as f32)) as f16,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != ((-5 as i32) as f16);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), ({{__v[0-9]+}} * ({{__v[0-9]+}} as f32)) as f16) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != (-((6.25 as f16) as f32) as f16);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} as f32;
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         std::ptr::write_volatile(
// REWRITES-DAG:             std::ptr::addr_of_mut!(r),
// REWRITES-DAG:             ({{__v[0-9]+}} / ({{__v[0-9]+}} + ({{__v[0-9]+}} as f32))) as f16,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 1.25;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         std::ptr::write_volatile(
// REWRITES-DAG:             std::ptr::addr_of_mut!(r),
// REWRITES-DAG:             (({{__v[0-9]+}} as f32) * (((3 as i32) as f16) as f32)) as f16,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != (-((7.5 as f16) as f32) as f16);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         std::ptr::write_volatile(
// REWRITES-DAG:             std::ptr::addr_of_mut!(i),
// REWRITES-DAG:             (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) }) as i32,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != -7;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(c)) };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         std::ptr::write_volatile(std::ptr::addr_of_mut!(r), unsafe {
// REWRITES-DAG:             vafn({{__v[0-9]+}}, __SlateVaArgs::new(vec![__SlateVaArg::new({{__v[0-9]+}})]))
// REWRITES-DAG:         })
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != (-((1.5 as f16) as f32) as f16);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), krfn({{__v[0-9]+}})) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 3.5;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), krprofn({{__v[0-9]+}})) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 3.0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(r), profn({{__v[0-9]+}})) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(r)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0.25;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} < {{__v[0-9]+}}) as i32) != 1;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} < {{__v[0-9]+}}) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a2)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} < {{__v[0-9]+}}) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nz)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(z)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} < {{__v[0-9]+}}) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} <= {{__v[0-9]+}}) as i32) != 1;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} <= {{__v[0-9]+}}) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a2)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} <= {{__v[0-9]+}}) as i32) != 1;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nz)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(z)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} <= {{__v[0-9]+}}) as i32) != 1;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} > {{__v[0-9]+}}) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} > {{__v[0-9]+}}) as i32) != 1;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a2)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} > {{__v[0-9]+}}) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nz)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(z)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} > {{__v[0-9]+}}) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} >= {{__v[0-9]+}}) as i32) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} >= {{__v[0-9]+}}) as i32) != 1;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a2)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} >= {{__v[0-9]+}}) as i32) != 1;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nz)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(z)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (({{__v[0-9]+}} >= {{__v[0-9]+}}) as i32) != 1;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nz)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(z)) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(i), ({{__v[0-9]+}} == {{__v[0-9]+}}) as i32) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 1;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(b)) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(i), ({{__v[0-9]+}} == {{__v[0-9]+}}) as i32) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(i)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { exit(0 as i32) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
