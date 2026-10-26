/* { dg-options "-ffp-model=strict" } */
#include <fenv.h>
#include <stdio.h>

int main(void) {
  fesetround(FE_UPWARD);
  double third = 1.0 / 3.0;
  fesetround(FE_TONEAREST);
  volatile double a = 0x1.0000001p0, b = 0x1.0000001p0, c = -1.0;
  double fused = a * b + c;
  printf("%a %a\n", third, fused);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-ffp-model=strict" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fesetround(__rounding_direction: i32) -> i32;
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// I686-UNKNOWN-LINUX-GNU-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     fn printf(__format: *const u8, ...) -> i32;
// CHECK-NEXT:     fn __slate_fenv_add_f64(_0: f64, _1: f64) -> f64;
// CHECK-NEXT:     fn __slate_fenv_div_f64(_0: f64, _1: f64) -> f64;
// CHECK-NEXT:     fn __slate_fenv_mul_f64(_0: f64, _1: f64) -> f64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// X86_64-UNKNOWN-LINUX-GNU-NEXT:     unsafe { fesetround(2048i32) };
// I686-UNKNOWN-LINUX-GNU-NEXT:     unsafe { fesetround(2048i32) };
// AARCH64-UNKNOWN-LINUX-GNU-NEXT:     unsafe { fesetround(4194304i32) };
// CHECK-NEXT:     let third: f64 = unsafe { __slate_fenv_div_f64(1.0f64, 3.0f64) };
// CHECK-NEXT:     unsafe { fesetround(0i32) };
// CHECK-NEXT:     let a: f64 = 1.0000000037252903f64;
// CHECK-NEXT:     let b: f64 = 1.0000000037252903f64;
// CHECK-NEXT:     let c: f64 = -1.0f64;
// CHECK-NEXT:     let fused: f64 = unsafe {
// CHECK-NEXT:         __slate_fenv_add_f64(
// CHECK-NEXT:             __slate_fenv_mul_f64(
// CHECK-NEXT:                 std::ptr::read_volatile(std::ptr::addr_of!(a)),
// CHECK-NEXT:                 std::ptr::read_volatile(std::ptr::addr_of!(b)),
// CHECK-NEXT:             ),
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(c)),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%a %a\n".as_ptr(), third, fused) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
