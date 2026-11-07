#include <stdio.h>
__attribute__((target("+sme"))) static long za_roundtrip(long v) {
  long r;
  __asm__ volatile("smstart za\n\tzero {za}\n\tsmstop za\n\tmov %0, %1" : "=r"(r) : "r"(v) : "za", "memory");
  return r;
}
__attribute__((target("+fp8"))) static long fpmr_rw(long v) {
  long r;
  __asm__ volatile("msr fpmr, %1\n\tmrs %0, fpmr" : "=r"(r) : "r"(v) : "memory");
  return r;
}
int main(void){ printf("%ld %ld\n", za_roundtrip(7), fpmr_rw(5)); return 0; }

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(aarch64_unstable_target_feature)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "sme")]
// CHECK-NEXT: unsafe fn za_roundtrip(mut v: i64) -> i64 {
// CHECK-NEXT:     let mut r: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "smstart za\n\tzero {{[{][{]}}za}}\n\tsmstop za\n\tmov {0:x}, {1:x}", lateout(reg)
// CHECK-NEXT:             [[T_0]], in (reg) v as u64, options(nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         r = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: #[target_feature(enable = "fp8")]
// CHECK-NEXT: unsafe fn fpmr_rw(mut v: i64) -> i64 {
// CHECK-NEXT:     let mut r: i64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "msr fpmr, {1:x}\n\tmrs {0:x}, fpmr", lateout(reg) [[T_0]], in (reg) v as u64,
// CHECK-NEXT:             options(nostack, preserves_flags)
// CHECK-NEXT:         );
// CHECK-NEXT:         r = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"%ld %ld\n".as_ptr(), za_roundtrip(7i64), fpmr_rw(5i64))
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
