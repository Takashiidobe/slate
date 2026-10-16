/* { dg-do run } */
/* PR tree-optimization/72824 */

static inline void foo(float *x, float value) {
  int i;
  for (i = 0; i < 32; ++i)
    x[i] = value;
}

int main() {
  float x[32];
  foo(x, -0.f);
  if (__builtin_copysignf(1.0, x[3]) != -1.0f)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-do run }
// CHECK-NEXT: //!
// CHECK-NEXT: //! PR tree-optimization/72824
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.copysign.f32"]
// CHECK-NEXT:     fn __slate_intrinsic_cda5f48326ee31f3([[ARG_0:arg[0-9]+]]: f32, [[ARG_1:arg[0-9]+]]: f32) -> f32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: *mut f32, mut value: f32) {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (32 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { x.offset(i as isize) } = value;
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         i = [[V_1]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: __SlateAlign16<[f32; 32]> = __SlateAlign16([0 as f32; 32]);
// CHECK-NEXT:     foo(x.0.as_mut_ptr() as *mut f32, -0.0f32);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __slate_intrinsic_cda5f48326ee31f3(1.0f64 as f32, unsafe {
// CHECK-NEXT:             *unsafe { (x.0.as_mut_ptr() as *mut f32).offset((3 as i32) as isize) }
// CHECK-NEXT:         })
// CHECK-NEXT:     }) != -1.0f32
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
