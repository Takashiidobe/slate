#include <fenv.h>
#include <stdio.h>

int main(void) {
  volatile double x = 3.0;
  volatile double y = 7.0;
  double          before;
  double          contended;
  double          after;

  before = x + y;

  fesetround(FE_DOWNWARD);
  {
#pragma STDC FENV_ACCESS ON
    contended = x / y;
  }
  fesetround(FE_TONEAREST);

  after = x + y;

  printf("%.20e %.20e %.20e\n", before, contended, after);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fesetround(__rounding_direction: i32) -> i32;
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let x: f64 = 3.0f64;
// CHECK-NEXT:     let y: f64 = 7.0f64;
// CHECK-NEXT:     let before: f64 = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) })
// CHECK-NEXT:         + unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// CHECK-NEXT:     let mut contended: f64 = 0 as f64;
// CHECK-NEXT:     let mut after: f64 = 0 as f64;
// CHECK-NEXT:     unsafe { fesetround(1024i32) };
// CHECK-NEXT:     contended = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) })
// CHECK-NEXT:         / unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// CHECK-NEXT:     unsafe { fesetround(0i32) };
// CHECK-NEXT:     after = (unsafe { std::ptr::read_volatile(std::ptr::addr_of!(x)) })
// CHECK-NEXT:         + unsafe { std::ptr::read_volatile(std::ptr::addr_of!(y)) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%.20e %.20e %.20e\n".as_ptr(), before, contended, after) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
