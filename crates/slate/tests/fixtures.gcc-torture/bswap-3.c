/* { dg-require-effective-target int32plus } */

int f(unsigned int a) __attribute__((noipa));
int f(unsigned int a) { return ((__builtin_bswap32(a)) >> 24) & 0x3; }

int g(unsigned int a) __attribute__((noipa));
int g(unsigned int a) { return a & 0x3; }

int main(void) {
  for (int b = 0; b <= 0xF; b++) {
    if (f(b) != g(b))
      __builtin_abort();
  }
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.bswap.i32"]
// CHECK-NEXT:     fn __slate_intrinsic_e94e9cf815168b51([[ARG_0:arg[0-9]+]]: u32) -> u32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f(mut a: u32) -> i32 {
// CHECK-NEXT:     ((unsafe { __slate_intrinsic_e94e9cf815168b51(a) }) >> (24 as i32) & ((3 as i32) as u32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn g(mut a: u32) -> i32 {
// CHECK-NEXT:     (a & ((3 as i32) as u32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut b: i32 = 0 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while b <= (15 as i32) {
// CHECK-NEXT:         if f(b as u32) != g(b as u32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = b;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]] + (1 as i32);
// CHECK-NEXT:         b = [[V_1]];
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
