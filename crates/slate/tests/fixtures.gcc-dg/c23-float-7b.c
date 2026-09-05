/* Test SNAN macros.  Runtime exceptions test, to verify NaN is
   signaling.  */
/* { dg-do run } */
/* { dg-require-effective-target fenv_exceptions_double } */
/* { dg-options "-std=c23 -pedantic-errors -fsignaling-nans" } */
/* { dg-add-options ieee } */

#include <fenv.h>
#include <float.h>

/* This should be defined if and only if signaling NaNs is supported
   for the given type.  If the testsuite gains effective-target
   support for targets not supporting signaling NaNs, this test
   should be made appropriately conditional.  */
#ifndef DBL_SNAN
#error "DBL_SNAN undefined"
#endif

volatile double d = DBL_SNAN;

extern void abort (void);
extern void exit (int);

/* @lowering-fn-begin */
/* @rewrite-fn-begin */
int
main (void)
{
  feclearexcept (FE_ALL_EXCEPT);
  d += d;
  if (!fetestexcept (FE_INVALID))
    abort ();
  exit (0);
}
/* @rewrite-fn-end */
/* @lowering-fn-end */

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 32;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 16;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 8;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} | {{_v[0-9]+}};
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = unsafe { feclearexcept({{_v[0-9]+}} as i32) };
// LOWERING-DAG:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// LOWERING-DAG:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// LOWERING-DAG:     let {{_v[0-9]+}}: f64 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), {{_v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = unsafe { fetestexcept({{_v[0-9]+}} as i32) };
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     unsafe { exit({{_v[0-9]+}} as i32) };
// LOWERING-DAG:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 4;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = 32 | {{_v[0-9]+}} | 16 | 8 | 1;
// REWRITES-DAG:     unsafe { feclearexcept({{_v[0-9]+}} as i32) };
// REWRITES-DAG:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// REWRITES-DAG:     let {{_v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), {{_v[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = unsafe { fetestexcept(1 as i32) };
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} != 0);
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { exit(0 as i32) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
