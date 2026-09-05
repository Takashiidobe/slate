/* Test NAN macro.  Runtime exceptions test, to verify NaN is quiet
   not signaling.  */
/* { dg-do run } */
/* { dg-require-effective-target fenv_exceptions } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-add-options ieee } */

#include <fenv.h>
#include <float.h>

/* This should be defined if and only if quiet NaNs are supported for
   type float.  If the testsuite gains effective-target support for
   targets not supporting NaNs, or not supporting them for all types,
   this test should only be run for targets supporting quiet NaNs for
   float.  */
#ifndef NAN
#error "NAN undefined"
#endif

volatile float f = NAN;

extern void abort (void);
extern void exit (int);

/* @lowering-fn-begin */
/* @rewrite-fn-begin */
int
main (void)
{
  f += f;
  if (fetestexcept (FE_INVALID))
    abort ();
  exit (0);
}
/* @rewrite-fn-end */
/* @lowering-fn-end */

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-DAG:     let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-DAG:     let {{_v[0-9]+}}: f32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), {{_v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{_v[0-9]+}}: i32 = unsafe { fetestexcept({{_v[0-9]+}} as i32) };
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
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
// REWRITES-DAG:     let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-DAG:     let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), {{_v[0-9]+}} + {{_v[0-9]+}}) };
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = unsafe { fetestexcept(1 as i32) };
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { exit(0 as i32) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
