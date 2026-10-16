/* { dg-xfail-if "Can not call system libm.a with -msoft-float" { powerpc-*-aix*
 * rs6000-*-aix* } { "-msoft-float" } { "" } } */
#include <math.h>

void abort(void);
void exit(int);

int main(void) {
  volatile double a;
  double          c;
  a = 32.0;
  c = pow(a, 1.0 / 3.0);
  if (c + 0.1 > 3.174802 && c - 0.1 < 3.174802)
    exit(0);
  else
    abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-xfail-if "Can not call system libm.a with -msoft-float" { powerpc-*-aix*
// CHECK-NEXT: //! rs6000-*-aix* } { "-msoft-float" } { "" } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn pow(__x: f64, __y: f64) -> f64;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: f64 = 0 as f64;
// CHECK-NEXT:     let mut c: f64 = 0 as f64;
// CHECK-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(a), 32.0f64) };
// CHECK-NEXT:     c = unsafe {
// CHECK-NEXT:         pow(
// CHECK-NEXT:             unsafe { std::ptr::read_volatile(std::ptr::addr_of!(a)) },
// CHECK-NEXT:             1.0f64 / 3.0f64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     if c + 0.1f64 > 3.174802f64 && c - 0.1f64 < 3.174802f64 {
// CHECK-NEXT:         unsafe { exit(0 as i32) };
// CHECK-NEXT:     } else {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
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
