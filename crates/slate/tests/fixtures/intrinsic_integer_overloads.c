#include <stdio.h>

static int calls;

static unsigned int input(void) {
  ++calls;
  return 0x89abcdefu;
}

int main(void) {
  unsigned short small = __builtin_bswap16(0x89abu);
  unsigned int medium = __builtin_bswap32(input());
  unsigned long long large = __builtin_bswap64(0x89abcdef01234567ull);
  printf("%x %x %llx %d\n", small, medium, large, calls);
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
// CHECK-NEXT:     #[link_name = "llvm.bswap.i16"]
// CHECK-NEXT:     fn __slate_intrinsic_1a0c079c71348b00([[ARG_0:arg[0-9]+]]: u16) -> u16;
// CHECK-NEXT:     #[link_name = "llvm.bswap.i64"]
// CHECK-NEXT:     fn __slate_intrinsic_5c27152fac4a123f([[ARG_0]]: u64) -> u64;
// CHECK-NEXT:     #[link_name = "llvm.bswap.i32"]
// CHECK-NEXT:     fn __slate_intrinsic_e94e9cf815168b51([[ARG_0]]: u32) -> u32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut calls: i32 = unsafe { std::mem::zeroed() };
// CHECK-NEXT: fn input() -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = (unsafe { calls }) + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     2309737967
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let small: u16 = unsafe { __slate_intrinsic_1a0c079c71348b00(35243 as u16) };
// CHECK-NEXT:     let medium: u32 = unsafe { __slate_intrinsic_e94e9cf815168b51(input()) };
// CHECK-NEXT:     let large: u64 = unsafe {
// CHECK-NEXT:         __slate_intrinsic_5c27152fac4a123f(9920249030613615975 as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%x %x %llx %d\n".as_ptr(),
// CHECK-NEXT:             (small as u32) as i32,
// CHECK-NEXT:             medium,
// CHECK-NEXT:             large,
// CHECK-NEXT:             unsafe { calls },
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
