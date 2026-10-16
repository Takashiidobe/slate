/* { dg-do run } */
/* PR tree-optimization/109386 */

static inline float foo(float x, float y) {
  float u = __builtin_fabsf(x);
  float v = __builtin_fabsf(y);
  if (!(u >= v)) {
    if (__builtin_isinf(v))
      return v;
    if (__builtin_isinf(u))
      return u;
  }
  return 42.0f;
}

int main() {
  if (!__builtin_isinf(foo(__builtin_inff(), __builtin_nanf(""))))
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! PR tree-optimization/109386
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_inff() -> f32;
// CHECK-NEXT:     #[link_name = "nanf"]
// CHECK-NEXT:     fn __builtin_nanf([[V_0:__v[0-9]+]]: *const i8) -> f32;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.fabs.f32"]
// CHECK-NEXT:     fn __slate_intrinsic_1a7588489948374([[ARG_0:arg[0-9]+]]: f32) -> f32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: f32, mut y: f32) -> f32 {
// CHECK-NEXT:     let mut u: f32 = unsafe { __slate_intrinsic_1a7588489948374(x) };
// CHECK-NEXT:     let mut v: f32 = unsafe { __slate_intrinsic_1a7588489948374(y) };
// CHECK-NEXT:     if !(u >= v) {
// CHECK-NEXT:         if v.is_infinite() {
// CHECK-NEXT:             return v;
// CHECK-NEXT:         }
// CHECK-NEXT:         if u.is_infinite() {
// CHECK-NEXT:             return u;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     42.0f32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if !foo(f32::INFINITY, unsafe {
// CHECK-NEXT:         __builtin_nanf(c"".as_ptr() as *const i8)
// CHECK-NEXT:     })
// CHECK-NEXT:     .is_infinite()
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
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
