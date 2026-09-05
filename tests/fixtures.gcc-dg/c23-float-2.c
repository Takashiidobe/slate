/* Test INFINITY macro.  Generic test.  */
/* { dg-do run } */
/* { dg-options "-std=c23 -w" } */
/* { dg-add-options ieee } */
/* { dg-require-effective-target inff } */

#include <float.h>

#ifndef INFINITY
#error "INFINITY undefined"
#endif

extern void abort (void);
extern void exit (int);

/* @lowering-fn-begin */
/* @rewrite-fn-begin */
int
main (void)
{
  (void) _Generic (INFINITY, float : 0);
  if (!(INFINITY >= FLT_MAX))
    abort ();
  exit (0);
}
/* @rewrite-fn-end */
/* @lowering-fn-end */

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = f32::from_bits(0x7f800000);
// LOWERING-DAG:         let {{__v[0-9]+}}: f32 = 340282346999999984391321947108527833088.0;
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
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
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = f32::from_bits(0x7f800000);
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = 340282346999999984391321947108527833088.0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} >= {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     unsafe { exit({{__v[0-9]+}} as i32) };
// REWRITES-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
