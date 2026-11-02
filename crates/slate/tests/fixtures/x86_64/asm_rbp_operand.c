#include <stdio.h>

static long bp_operand(long x) {
  register long r __asm__("rbp") = x;
  __asm__("addq $5, %0" : "+r"(r));
  return r;
}

static long bp_intel(long x) {
  register long r __asm__("rbp") = x;
  __asm__(".intel_syntax noprefix\n\tadd rbp, 7\n\t.att_syntax" : "+r"(r));
  return r;
}

int main(void) {
  printf("%ld %ld\n", bp_operand(10), bp_intel(20));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn bp_operand(mut r: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov %rbp, {1:r}\nmov {0:r}, %rbp\naddq $5, %rbp\nmov %rbp, {0:r}\nmov {1:r}, %rbp",
// CHECK-NEXT:             inout(reg) r as u64 => [[T_0]], out(reg) _, options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn bp_intel(mut r: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "mov %rbp, {1:r}\nmov {0:r}, %rbp\n.intel_syntax noprefix\n\tadd rbp, 7\n\t.att_syntax\nmov %rbp, {0:r}\nmov {1:r}, %rbp",
// CHECK-NEXT:             inout(reg) r as u64 => [[T_0]], out(reg) _, options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(c"%ld %ld\n".as_ptr(), bp_operand(10i64), bp_intel(20i64))
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
