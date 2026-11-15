// { dg-options "-O2" }
#include <stdio.h>

volatile int calls;

__attribute__((noinline)) static void *caller_site(void) {
  calls++;
  return __builtin_return_address(0);
}

static inline __attribute__((always_inline)) void *inlined_site(void) {
  return __builtin_return_address(0);
}

int main(void) {
  void *first = caller_site();
  void *second = caller_site();
  void *third = inlined_site();
  void *fourth = inlined_site();
  printf("%d %d\n", first != second, third == fourth);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-O2" }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.returnaddress.p0"]
// CHECK-NEXT:     fn __slate_intrinsic_ded315013ea6523([[ARG_0:arg[0-9]+]]: i32) -> *mut ();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: static mut calls: i32 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[inline(never)]
// CHECK-NEXT: fn caller_site() -> *mut () {
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: i32 = unsafe { core::ptr::read_volatile(core::ptr::addr_of!(calls)) };
// CHECK-NEXT:     unsafe { core::ptr::write_volatile(core::ptr::addr_of_mut!(calls), [[V_0]] + 1) };
// CHECK-NEXT:     unsafe { __slate_intrinsic_ded315013ea6523(const { ((0 as i32) as u32) as i32 }) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[inline(always)]
// CHECK-NEXT: fn inlined_site() -> *mut () {
// CHECK-NEXT:     unsafe { __slate_intrinsic_ded315013ea6523(const { ((0 as i32) as u32) as i32 }) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let first: *mut () = caller_site();
// CHECK-NEXT:     let second: *mut () = caller_site();
// CHECK-NEXT:     let third: *mut () = inlined_site();
// CHECK-NEXT:     let fourth: *mut () = inlined_site();
// CHECK-NEXT:     println!("{} {}", (first != second) as i32, (third == fourth) as i32);
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
