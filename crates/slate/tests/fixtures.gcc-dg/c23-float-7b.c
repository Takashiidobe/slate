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

extern void abort(void);
extern void exit(int);

/* @lowering-fn-begin */
/* @rewrite-fn-begin */
int main(void) {
  feclearexcept(FE_ALL_EXCEPT);
  d += d;
  if (!fetestexcept(FE_INVALID))
    abort();
  exit(0);
}
/* @rewrite-fn-end */
/* @lowering-fn-end */

// SLATE-FILECHECK-BEGIN lowering
// LOWERING-DAG: fn main() -> std::process::ExitCode {
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 32;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 16;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 8;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} | {{__v[0-9]+}};
// LOWERING-AARCH64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 31;
// LOWERING-DAG:     let {{__v[0-9]+}}: i32 = unsafe { feclearexcept({{__v[0-9]+}} as i32) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// LOWERING-DAG:     let {{__v[0-9]+}}: f64 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), {{__v[0-9]+}}) };
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
// LOWERING-DAG:     return std::process::ExitCode::SUCCESS;
// LOWERING-DAG: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() -> std::process::ExitCode {
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 4;
// REWRITES-X86_64-GNU-DAG:     let {{__v[0-9]+}}: i32 = 32 | {{__v[0-9]+}} | 16 | 8 | 1;
// REWRITES-X86_64-GNU-DAG:     unsafe { feclearexcept({{__v[0-9]+}} as i32) };
// REWRITES-AARCH64-GNU-DAG:     unsafe { feclearexcept(31 as i32) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// REWRITES-DAG:     let {{__v[0-9]+}}: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(d)) };
// REWRITES-DAG:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(d), {{__v[0-9]+}} + {{__v[0-9]+}}) };
// REWRITES-DAG:     let {{__v[0-9]+}}: i32 = unsafe { fetestexcept(1 as i32) };
// REWRITES-DAG:     let {{__v[0-9]+}}: bool = !({{__v[0-9]+}} != 0);
// REWRITES-DAG:     if {{__v[0-9]+}} {
// REWRITES-DAG:         unsafe { abort() };
// REWRITES-DAG:     }
// REWRITES-DAG:     unsafe { exit(0 as i32) };
// REWRITES-DAG:     return std::process::ExitCode::SUCCESS;
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
