/* Test _Float16 type-generic built-in functions: __builtin_isinf_sign.  */
/* { dg-do run } */
/* { dg-options "" } */
/* { dg-add-options float16 } */
/* { dg-add-options ieee } */
/* { dg-require-effective-target float16_runtime } */

#define WIDTH 16
#define EXT   0
/* Tests for _FloatN / _FloatNx types: compile and execution tests for
   type-generic built-in functions: __builtin_isinf_sign.  Before
   including this file, define WIDTH as the value N; define EXT to 1
   for _FloatNx and 0 for _FloatN.  */

#define __STDC_WANT_IEC_60559_TYPES_EXT__
#include <float.h>

#define CONCATX(X, Y)       X##Y
#define CONCAT(X, Y)        CONCATX(X, Y)
#define CONCAT3(X, Y, Z)    CONCAT(CONCAT(X, Y), Z)
#define CONCAT4(W, X, Y, Z) CONCAT(CONCAT(CONCAT(W, X), Y), Z)

#if EXT
#define TYPE   CONCAT3(_Float, WIDTH, x)
#define CST(C) CONCAT4(C, f, WIDTH, x)
#define MAX    CONCAT3(FLT, WIDTH, X_MAX)
#else
#define TYPE   CONCAT(_Float, WIDTH)
#define CST(C) CONCAT3(C, f, WIDTH)
#define MAX    CONCAT3(FLT, WIDTH, _MAX)
#endif

extern void exit(int);
extern void abort(void);

volatile TYPE inf = __builtin_inf(), nanval = __builtin_nan("");
volatile TYPE neginf = -__builtin_inf(), negnanval = -__builtin_nan("");
volatile TYPE zero = CST(0.0), negzero = -CST(0.0), one = CST(1.0);
volatile TYPE max = MAX, negmax = -MAX;

int
// @lowering-fn-begin
// @rewrite-fn-begin
main(void) {
  if (__builtin_isinf_sign(inf) != 1)
    abort();
  if (__builtin_isinf_sign(neginf) != -1)
    abort();
  if (__builtin_isinf_sign(nanval) != 0)
    abort();
  if (__builtin_isinf_sign(negnanval) != 0)
    abort();
  if (__builtin_isinf_sign(zero) != 0)
    abort();
  if (__builtin_isinf_sign(negzero) != 0)
    abort();
  if (__builtin_isinf_sign(one) != 0)
    abort();
  if (__builtin_isinf_sign(max) != 0)
    abort();
  if (__builtin_isinf_sign(negmax) != 0)
    abort();
  exit(0);
}
// @rewrite-fn-end
// @lowering-fn-end

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() -> std::process::ExitCode {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(neginf)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nanval)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(negnanval)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(negzero)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(max)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(negmax)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = -1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} };
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != {{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     unsafe { exit({{__v[0-9]+}} as i32) };
// LOWERING-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(inf)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (if {{__v[0-9]+}} {
// REWRITES-DAG:         if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// REWRITES-DAG:     } else {
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     }) != 1;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(neginf)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (if {{__v[0-9]+}} {
// REWRITES-DAG:         if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// REWRITES-DAG:     } else {
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     }) != -1;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(nanval)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (if {{__v[0-9]+}} {
// REWRITES-DAG:         if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// REWRITES-DAG:     } else {
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     }) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(negnanval)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (if {{__v[0-9]+}} {
// REWRITES-DAG:         if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// REWRITES-DAG:     } else {
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     }) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(zero)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (if {{__v[0-9]+}} {
// REWRITES-DAG:         if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// REWRITES-DAG:     } else {
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     }) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(negzero)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (if {{__v[0-9]+}} {
// REWRITES-DAG:         if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// REWRITES-DAG:     } else {
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     }) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(one)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (if {{__v[0-9]+}} {
// REWRITES-DAG:         if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// REWRITES-DAG:     } else {
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     }) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(max)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (if {{__v[0-9]+}} {
// REWRITES-DAG:         if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// REWRITES-DAG:     } else {
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     }) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f16 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(negmax)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == f16::NEG_INFINITY || {{__v[0-9]+}} == f16::INFINITY;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_sign_negative();
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = -1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = (if {{__v[0-9]+}} {
// REWRITES-DAG:         if {{__v[0-9]+}} { {{__v[0-9]+}} } else { {{__v[0-9]+}} }
// REWRITES-DAG:     } else {
// REWRITES-DAG:         {{__v[0-9]+}}
// REWRITES-DAG:     }) != 0;
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { exit(0 as i32) };
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
