#include <stdio.h>

typedef int v4si __attribute__((vector_size(16)));

int main(void) {
  v4si a = {1, 2, 3, 4};
  v4si b = {5, 6, 7, 8};
  v4si c = a + b;
  c[1]   = 20;
  v4si d = __builtin_shufflevector(c, c, 3, 2, 1, 0);
  printf("%d %d %d\n", c[0], c[1], d[0]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: std::simd::Simd<i32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         1i32, 2i32, 3i32, 4i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let b: std::simd::Simd<i32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         5i32, 6i32, 7i32, 8i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut c: std::simd::Simd<i32, 4> = a + b;
// CHECK-NEXT:     c[1] = 20;
// CHECK-NEXT:     let d: std::simd::Simd<i32, 4> = std::simd::simd_swizzle!(c, c, [3, 2, 1, 0]);
// CHECK-NEXT:     println!("{} {} {}", c[0], c[1], d[0]);
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
