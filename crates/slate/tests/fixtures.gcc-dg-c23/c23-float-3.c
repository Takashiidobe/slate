/* Test INFINITY macro.  Test when infinities supported.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -pedantic-errors" } */
/* { dg-add-options ieee } */
/* { dg-require-effective-target inff } */

#include <float.h>

#ifndef INFINITY
#error "INFINITY undefined"
#endif

volatile float f = INFINITY;

extern void abort (void);
extern void exit (int);

/* @lowering-fn-begin */
/* @rewrite-fn-begin */
int
main (void)
{
  (void) _Generic (INFINITY, float : 0);
  if (!(INFINITY > FLT_MAX))
    abort ();
  if (!(f > FLT_MAX))
    abort ();
  exit (0);
}
/* @rewrite-fn-end */
/* @lowering-fn-end */

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = f32::from_bits(0x7f800000);
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = 340282346999999984391321947108527833088.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = !{{_v[0-9]+}};
// LOWERING-DAG:         if {{_v[0-9]+}} {
// LOWERING-DAG:             unsafe { abort() };
// LOWERING-DAG:         }
// LOWERING-DAG:     }
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-DAG:         let {{_v[0-9]+}}: f32 = 340282346999999984391321947108527833088.0;
// LOWERING-DAG:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} > {{_v[0-9]+}};
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
// REWRITES-DAG:     let {{_v[0-9]+}}: f32 = f32::from_bits(0x7f800000);
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} > 340282346999999984391321947108527833088.0);
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{_v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-DAG:     let {{_v[0-9]+}}: bool = !({{_v[0-9]+}} > 340282346999999984391321947108527833088.0);
// REWRITES-DAG:     if {{_v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { exit(0 as i32) };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
