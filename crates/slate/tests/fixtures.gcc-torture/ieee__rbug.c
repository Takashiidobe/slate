/* { dg-do run }

   # AVR doubles are floats
   { dg-skip-if "AVR doubles are floats" { avr-*-* } }
   { dg-skip-if "AVR doubles are floats" { d10v-*-* } { "*" } { "*-mdouble64*" }
   } */
#if defined(__i386__) && defined(__FreeBSD__)
#include <ieeefp.h>
#endif

void abort(void);
void exit(int);

double d(unsigned long long k) {
  double x;

  x = (double)k;
  return x;
}

float s(unsigned long long k) {
  float x;

  x = (float)k;
  return x;
}

int main(void) {
  unsigned long long int k;
  double                 x;

#if defined(__i386__) && defined(__FreeBSD__)
  /* This test case assumes extended-precision, but FreeBSD defaults to
     double-precision.  Make it so.  */
  fpsetprec(FP_PE);
#endif

  if (sizeof(double) >= 8) {
    k = 0x8693ba6d7d220401ULL;
    x = d(k);
    k = (unsigned long long)x;
    if (k != 0x8693ba6d7d220800ULL)
      abort();
  }

  k = 0x8234508000000001ULL;
  x = s(k);
  k = (unsigned long long)x;
  if (k != 0x8234510000000000ULL)
    abort();

  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! # AVR doubles are floats
// CHECK-NEXT: //! { dg-skip-if "AVR doubles are floats" { avr-*-* } }
// CHECK-NEXT: //! { dg-skip-if "AVR doubles are floats" { d10v-*-* } { "*" } { "*-mdouble64*" }
// CHECK-NEXT: //! }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn d(mut k: u64) -> f64 {
// CHECK-NEXT:     let mut x: f64 = k as f64;
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn s(mut k: u64) -> f32 {
// CHECK-NEXT:     let mut x: f32 = k as f32;
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut k: u64 = 0 as u64;
// CHECK-NEXT:     let mut x: f64 = 0 as f64;
// CHECK-NEXT:     if (8 as u64) >= (((8 as i32) as i64) as u64) {
// CHECK-NEXT:         k = 9697299402072392705 as u64;
// CHECK-NEXT:         x = d(k);
// CHECK-NEXT:         k = x as u64;
// CHECK-NEXT:         if k != (9697299402072393728 as u64) {
// CHECK-NEXT:             unsafe { abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     k = 9382212434405621761 as u64;
// CHECK-NEXT:     x = s(k) as f64;
// CHECK-NEXT:     k = x as u64;
// CHECK-NEXT:     if k != (9382212984161435648 as u64) {
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
