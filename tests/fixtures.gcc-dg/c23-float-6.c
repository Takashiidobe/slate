/* Test SNAN macros.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors -fsignaling-nans" } */
/* { dg-add-options ieee } */

#include <float.h>

/* These should be defined if and only if signaling NaNs are supported
   for the given types.  If the testsuite gains effective-target
   support for targets not supporting signaling NaNs, or not
   supporting them for all types, this test should be made
   appropriately conditional.  */
#ifndef FLT_SNAN
#error "FLT_SNAN undefined"
#endif
#ifndef DBL_SNAN
#error "DBL_SNAN undefined"
#endif
#ifndef LDBL_SNAN
#error "LDBL_SNAN undefined"
#endif

volatile float       f  = FLT_SNAN;
volatile double      d  = DBL_SNAN;
volatile long double ld = LDBL_SNAN;

extern void abort(void);
extern void exit(int);

/* @lowering-fn-begin */
/* @rewrite-fn-begin */
int main(void) {
  (void)_Generic(FLT_SNAN, float: 0);
  (void)_Generic(DBL_SNAN, double: 0);
  (void)_Generic(LDBL_SNAN, long double: 0);
  if (!__builtin_isnan(FLT_SNAN))
    abort();
  if (!__builtin_isnan(f))
    abort();
  if (!__builtin_isnan(DBL_SNAN))
    abort();
  if (!__builtin_isnan(d))
    abort();
  if (!__builtin_isnan(LDBL_SNAN))
    abort();
  if (!__builtin_isnan(ld))
    abort();
  exit(0);
}
/* @rewrite-fn-end */
/* @lowering-fn-end */

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = ({{__v[0-9]+}} as f64).is_nan();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_nan();
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// LOWERING-DAG:         if {{__v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 3);
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
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
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !({{__v[0-9]+}} != 0);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = ({{__v[0-9]+}} as f64).is_nan();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !({{__v[0-9]+}} != 0);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}}.is_nan();
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !({{__v[0-9]+}} != 0);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: LongDouble = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(ld)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = __slate_f80_is_fp_class({{__v[0-9]+}}, 3);
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { exit(0 as i32) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
