/* { dg-options "--flavor=gcc -O2 -mfma -ffp-contract=off" } */
int printf(const char *, ...);

int main(void) {
  volatile double base = 1.0 + 0x1p-30;
  volatile double square = 1.0 + 0x1p-29;
  double a = base;
  double residual = a * a - square;
  float fa = 1.0f + 0x1p-13f;
  float fresidual = fa * fa - (1.0f + 0x1p-12f);
  printf("%a %a\n", residual, (double)fresidual);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[V_0:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-options "--flavor=gcc -O2 -mfma -ffp-contract=off" }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let base: f64 = 1.0f64 + 9.313225746154785e-10f64;
// CHECK-NEXT:     let square: f64 = 1.0f64 + 1.862645149230957e-9f64;
// CHECK-NEXT:     let a: f64 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(base)) };
// CHECK-NEXT:     let residual: f64 = a * a - unsafe { std::ptr::read_volatile(std::ptr::addr_of!(square)) };
// CHECK-NEXT:     let fa: f32 = 1.0f32 + 0.00012207031f32;
// CHECK-NEXT:     let fresidual: f32 = fa * fa - (1.0f32 + 0.00024414063f32);
// CHECK-NEXT:     unsafe { printf(c"%a %a\n".as_ptr(), residual, fresidual as f64) };
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
