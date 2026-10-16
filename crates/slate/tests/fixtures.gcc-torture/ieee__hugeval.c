/* { dg-do run }
   { dg-xfail-if "" { *-*-vxworks* } }
   { dg-xfail-if "" { tic6x-*-* && ti_c67x } } */
#include <math.h>

void abort(void);
void exit(int);

static const double zero = 0.0;
static const double pone = 1.0;
static const double none = -1.0;
static const double pinf = 1.0 / 0.0;
static const double ninf = -1.0 / 0.0;

int main() {
  if (pinf != pone / zero)
    abort();

  if (ninf != none / zero)
    abort();

#ifdef HUGE_VAL
  if (HUGE_VAL != pinf)
    abort();

  if (-HUGE_VAL != ninf)
    abort();
#endif

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //! { dg-xfail-if "" { *-*-vxworks* } }
// CHECK-NEXT: //! { dg-xfail-if "" { tic6x-*-* && ti_c67x } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn __builtin_huge_val() -> f64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut zero: f64 = 0.0f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut pone: f64 = 1.0f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut none: f64 = -1.0f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut pinf: f64 = 1.0f64 / 0.0f64;
// CHECK-EMPTY:
// CHECK-NEXT: static mut ninf: f64 = -1.0f64 / 0.0f64;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe { pinf }) != (unsafe { pone }) / unsafe { zero } {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { ninf }) != (unsafe { none }) / unsafe { zero } {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f64::INFINITY != unsafe { pinf } {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if -f64::INFINITY != unsafe { ninf } {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { exit(0 as i32) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
