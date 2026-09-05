/* Test SNAN macros.  Runtime exceptions test, to verify NaN is
   signaling.  */
/* { dg-do run } */
/* { dg-require-effective-target fenv_exceptions } */
/* { dg-options "-std=c23 -pedantic-errors -fsignaling-nans" } */
/* { dg-add-options ieee } */

#include <fenv.h>
#include <float.h>

/* This should be defined if and only if signaling NaNs is supported
   for the given type.  If the testsuite gains effective-target
   support for targets not supporting signaling NaNs, this test
   should be made appropriately conditional.  */
#ifndef FLT_SNAN
#error "FLT_SNAN undefined"
#endif

volatile float f = FLT_SNAN;

extern void abort(void);
extern void exit(int);

/* @lowering-fn-begin */
/* @rewrite-fn-begin */
int main(void) {
  feclearexcept(FE_ALL_EXCEPT);
  f += f;
  if (!fetestexcept(FE_INVALID))
    abort();
  exit(0);
}
/* @rewrite-fn-end */
/* @lowering-fn-end */

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 32;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 16;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 8;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { feclearexcept({{__v[0-9]+}} as i32) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), {{__v[0-9]+}}) };
// LOWERING-DAG:     {
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = 1;
// LOWERING-DAG:         let {{__v[0-9]+}}: i32 = unsafe { fetestexcept({{__v[0-9]+}} as i32) };
// LOWERING-DAG:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
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
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 32;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 4;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 16;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 8;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// REWRITES-DAG:     unsafe { feclearexcept({{__v[0-9]+}} as i32) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(f)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(f), {{__v[0-9]+}}) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 1;
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { fetestexcept({{__v[0-9]+}} as i32) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !{{__v[0-9]+}};
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = 0;
// REWRITES-DAG:     unsafe { exit({{__v[0-9]+}} as i32) };
// REWRITES-DAG:     std::process::exit({{__v[0-9]+}} as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
