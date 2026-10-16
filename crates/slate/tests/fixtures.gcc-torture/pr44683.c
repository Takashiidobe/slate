int __attribute__((noinline, noclone)) copysign_bug(double x) {
  if (x != 0.0 && (x * 0.5 == x))
    return 1;
  if (__builtin_copysign(1.0, x) < 0.0)
    return 2;
  else
    return 3;
}
int main(void) {
  double x = -0.0;
  if (copysign_bug(x) != 2)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.copysign.f64"]
// CHECK-NEXT:     fn __slate_intrinsic_90b7711c6b7522c7([[ARG_0:arg[0-9]+]]: f64, [[ARG_1:arg[0-9]+]]: f64) -> f64;
// CHECK-NEXT: }
// CHECK-NEXT: fn copysign_bug(mut x: f64) -> i32 {
// CHECK-NEXT:     if x != 0.0f64 && x * 0.5f64 == x {
// CHECK-NEXT:         return 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { __slate_intrinsic_90b7711c6b7522c7(1.0f64, x) }) < 0.0f64 {
// CHECK-NEXT:         return 2 as i32;
// CHECK-NEXT:     } else {
// CHECK-NEXT:         return 3 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: f64 = -0.0f64;
// CHECK-NEXT:     if copysign_bug(x) != (2 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
