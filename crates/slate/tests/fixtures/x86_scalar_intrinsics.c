#include <stdio.h>

static int instructions(void) {
  unsigned long long flags = __builtin_ia32_readeflags_u64();
  __builtin_ia32_writeeflags_u64(flags);
  __builtin_ia32_pause();
  __builtin_ia32_lfence();
  unsigned long long first = __builtin_ia32_rdtsc();
  __builtin_ia32_mfence();
  unsigned long long second = __builtin_ia32_rdtsc();
  __builtin_ia32_sfence();
  return first != 0 && second >= first;
}

int main(void) {
  printf("%d\n", instructions());
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.x86.sse2.lfence"]
// CHECK-NEXT:     fn __slate_intrinsic_6f7c5327cf9e8272();
// CHECK-NEXT:     #[link_name = "llvm.x86.sse2.mfence"]
// CHECK-NEXT:     fn __slate_intrinsic_9ae5c42efd200fe6();
// CHECK-NEXT:     #[link_name = "llvm.x86.rdtsc"]
// CHECK-NEXT:     fn __slate_intrinsic_9e72945d5c6a2d09() -> u64;
// CHECK-NEXT:     #[link_name = "llvm.x86.flags.read.u64"]
// CHECK-NEXT:     fn __slate_intrinsic_9f2888a0dbec63c7() -> i64;
// CHECK-NEXT:     #[link_name = "llvm.x86.sse.sfence"]
// CHECK-NEXT:     fn __slate_intrinsic_b2ae1adafd1df521();
// CHECK-NEXT:     #[link_name = "llvm.x86.flags.write.u64"]
// CHECK-NEXT:     fn __slate_intrinsic_c6a72ac15744325c([[ARG_0:arg[0-9]+]]: i64);
// CHECK-NEXT:     #[link_name = "llvm.x86.sse2.pause"]
// CHECK-NEXT:     fn __slate_intrinsic_cf7232a07549d448();
// CHECK-NEXT: }
// CHECK-NEXT: fn instructions() -> i32 {
// CHECK-NEXT:     let flags: u64 = (unsafe { __slate_intrinsic_9f2888a0dbec63c7() }) as u64;
// CHECK-NEXT:     unsafe { __slate_intrinsic_c6a72ac15744325c(flags as i64) };
// CHECK-NEXT:     unsafe { __slate_intrinsic_cf7232a07549d448() };
// CHECK-NEXT:     unsafe { __slate_intrinsic_6f7c5327cf9e8272() };
// CHECK-NEXT:     let first: u64 = unsafe { __slate_intrinsic_9e72945d5c6a2d09() };
// CHECK-NEXT:     unsafe { __slate_intrinsic_9ae5c42efd200fe6() };
// CHECK-NEXT:     let second: u64 = unsafe { __slate_intrinsic_9e72945d5c6a2d09() };
// CHECK-NEXT:     unsafe { __slate_intrinsic_b2ae1adafd1df521() };
// CHECK-NEXT:     (first != 0 && second >= first) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{}", instructions());
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
