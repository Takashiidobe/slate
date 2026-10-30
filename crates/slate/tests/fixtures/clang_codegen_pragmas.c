#include <stdio.h>

#pragma clang optimize off
static int add_noopt(int a, int b) { return a + b; }
#pragma clang optimize on

#pragma clang attribute push(__attribute__((visibility("hidden"))),            \
                             apply_to = function)
int                     hidden_fn(void) { return 7; }
#pragma clang attribute pop

static int loop_sum(void) {
  int s = 0;
#pragma unroll(4)
#pragma clang loop vectorize(enable) interleave(enable)
  for (int i = 0; i < 8; i++)
    s += i;
  return s;
}

double           fma_like(double a, double b, double c) {
#pragma clang fp contract(on)
  return a * b + c;
}

int main(void) {
  printf("%d %d %d %f\n", add_noopt(1, 2), hidden_fn(), loop_sum(),
         fma_like(2.0, 3.0, 4.0));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn add_noopt(a: i32, b: i32) -> i32 {
// CHECK-NEXT:     a + b
// CHECK-NEXT: }
// CHECK-NEXT: fn hidden_fn() -> i32 {
// CHECK-NEXT:     7
// CHECK-NEXT: }
// CHECK-NEXT: fn loop_sum() -> i32 {
// CHECK-NEXT:     let mut s: i32 = 0;
// CHECK-NEXT:     for i in 0i32..8i32 {
// CHECK-NEXT:         s += i;
// CHECK-NEXT:     }
// CHECK-NEXT:     s
// CHECK-NEXT: }
// CHECK-NEXT: fn fma_like(a: f64, b: f64, c: f64) -> f64 {
// CHECK-NEXT:     a * b + c
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %f\n".as_ptr(),
// CHECK-NEXT:             add_noopt(1i32, 2i32),
// CHECK-NEXT:             hidden_fn(),
// CHECK-NEXT:             loop_sum(),
// CHECK-NEXT:             fma_like(2.0f64, 3.0f64, 4.0f64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
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
