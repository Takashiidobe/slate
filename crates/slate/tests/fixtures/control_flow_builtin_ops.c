#include <stdio.h>

static int likely_nonzero(int x) {
  if (__builtin_expect(x != 0, 1)) {
    __builtin_assume(x != 0);
    return x + 10;
  }
  return -1;
}

static int assume_true(int x) {
  __builtin_assume(1);
  return x + 1;
}

static int guarded_trap(int x) {
  if (x < 0) {
    __builtin_trap();
  }
  return x;
}

static int guarded_unreachable(int x) {
  if (x < 0) {
    __builtin_unreachable();
  }
  return x * 2;
}

int main(void) {
  volatile int input = 5;
  int          a     = likely_nonzero(input);
  int          b     = assume_true(input);
  int          c     = guarded_trap(input);
  int          d     = guarded_unreachable(input);
  printf("%d %d %d %d\n", a, b, c, d);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn __builtin_expect([[ARG_0]]: i64, [[ARG_1:arg[0-9]+]]: i64) -> i64;
// CHECK-NEXT:     fn __builtin_assume([[ARG_0]]: bool);
// CHECK-NEXT:     fn __builtin_unreachable();
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.trap"]
// CHECK-NEXT:     fn __slate_intrinsic_46d8fd79c9d1f86c();
// CHECK-NEXT: }
// CHECK-NEXT: fn likely_nonzero(x: i32) -> i32 {
// CHECK-NEXT:     if x != 0 {
// CHECK-NEXT:         unsafe { std::hint::assert_unchecked(x != 0) };
// CHECK-NEXT:         return x + 10;
// CHECK-NEXT:     }
// CHECK-NEXT:     -1
// CHECK-NEXT: }
// CHECK-NEXT: fn assume_true(x: i32) -> i32 {
// CHECK-NEXT:     unsafe { std::hint::assert_unchecked(true) };
// CHECK-NEXT:     x + 1
// CHECK-NEXT: }
// CHECK-NEXT: fn guarded_trap(x: i32) -> i32 {
// CHECK-NEXT:     if x < 0 {
// CHECK-NEXT:         unsafe { __slate_intrinsic_46d8fd79c9d1f86c() };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn guarded_unreachable(x: i32) -> i32 {
// CHECK-NEXT:     if x < 0 {
// CHECK-NEXT:         unsafe { std::hint::unreachable_unchecked() };
// CHECK-NEXT:     }
// CHECK-NEXT:     x * 2
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let input: i32 = 5;
// CHECK-NEXT:     let a: i32 = likely_nonzero(unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(input))
// CHECK-NEXT:     });
// CHECK-NEXT:     let b: i32 = assume_true(unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(input))
// CHECK-NEXT:     });
// CHECK-NEXT:     let c: i32 = guarded_trap(unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(input))
// CHECK-NEXT:     });
// CHECK-NEXT:     let d: i32 = guarded_unreachable(unsafe {
// CHECK-NEXT:         std::ptr::read_volatile(std::ptr::addr_of!(input))
// CHECK-NEXT:     });
// CHECK-NEXT:     println!("{} {} {} {}", a, b, c, d);
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
