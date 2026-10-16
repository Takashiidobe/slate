/* { dg-do run } */
extern void exit(int);
extern void abort(void);
float       x = -1.5f;

float rintf() {
  static const float TWO23 = 8388608.0;

  if (__builtin_fabs(x) < TWO23) {
    if (x > 0.0) {
      x += TWO23;
      x -= TWO23;
    } else if (x < 0.0) {
      x = TWO23 - x;
      x = -(x - TWO23);
    }
  }

  return x;
}

int main(void) {
  if (rintf() != -2.0)
    abort();
  exit(0);
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn exit([[V_0:__v[0-9]+]]: i32);
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.fabs.f64"]
// CHECK-NEXT:     fn __slate_intrinsic_999a0f45e9537850([[ARG_0:arg[0-9]+]]: f64) -> f64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // { dg-do run }
// CHECK-NEXT: static mut x: f32 = -1.5f32;
// CHECK-EMPTY:
// CHECK-NEXT: fn rintf() -> f32 {
// CHECK-NEXT:     if (unsafe { __slate_intrinsic_999a0f45e9537850((unsafe { x }) as f64) })
// CHECK-NEXT:         < ((unsafe { TWO23 }) as f64)
// CHECK-NEXT:     {
// CHECK-NEXT:         if ((unsafe { x }) as f64) > 0.0f64 {
// CHECK-NEXT:             let mut [[V_1:__v[0-9]+]]: f32 = unsafe { x };
// CHECK-NEXT:             let mut [[V_2:__v[0-9]+]]: f32 = [[V_1]] + unsafe { TWO23 };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 x = [[V_2]];
// CHECK-NEXT:             }
// CHECK-NEXT:             let mut [[V_3:__v[0-9]+]]: f32 = unsafe { x };
// CHECK-NEXT:             let mut [[V_4:__v[0-9]+]]: f32 = [[V_3]] - unsafe { TWO23 };
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 x = [[V_4]];
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if ((unsafe { x }) as f64) < 0.0f64 {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     x = (unsafe { TWO23 }) - unsafe { x };
// CHECK-NEXT:                 }
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     x = -((unsafe { x }) - unsafe { TWO23 });
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { x }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut TWO23: f32 = 8388608.0f64 as f32;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (rintf() as f64) != -2.0f64 {
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
