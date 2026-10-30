#include <stdio.h>

static unsigned add_via_rbx(unsigned a, unsigned b) {
  __asm__("addl %%ebx, %0" : "+r"(a) : "b"(b));
  return a;
}

static unsigned double_in_rbx(unsigned x) {
  __asm__("addl %0, %0" : "+b"(x));
  return x;
}

static long scaled_sum(long a, long b) {
  register long r10 __asm__("r10") = b;
  register long r8 __asm__("r8") = a;
  __asm__("leaq (%1,%2,4), %0\n\taddb $1, %b0" : "=r"(r8) : "r"(r8), "r"(r10));
  return r8;
}

static unsigned short low_word(unsigned x) {
  register unsigned short w __asm__("si");
  __asm__("movw %w1, %0" : "=r"(w) : "r"(x));
  return w;
}

int main(void) {
  printf("%u %u %ld %u\n", add_via_rbx(40, 2), double_in_rbx(21),
         scaled_sum(3, 10), low_word(0x12345678u));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn add_via_rbx(mut a: u32, mut b: u32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "xchg {1:r}, %rbx\naddl %ebx, {0:e}\nxchg {1:r}, %rbx", inlateout(reg) a as
// CHECK-NEXT:             u32 => [[T_0]], inout(reg) b as u32 => _, options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         a = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-NEXT: fn double_in_rbx(mut x: u32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "xchg {0:r}, %rbx\naddl %ebx, %ebx\nxchg {0:r}, %rbx", inout(reg) x as u32 =>
// CHECK-NEXT:             [[T_0]], options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn scaled_sum(mut r8: i64, mut r10: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leaq (%r8,%r10,4), %r8\n\taddb $1, %r8b", lateout("r8") [[T_0]], in ("r8") r8
// CHECK-NEXT:             as u64, in ("r10") r10 as u64, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r8 = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     r8
// CHECK-NEXT: }
// CHECK-NEXT: fn low_word(mut x: u32) -> u16 {
// CHECK-NEXT:     let mut w: u16 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u16;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movw {0:x}, %si", in (reg) x as u32, lateout("si") [[T_0]], options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         w = unsafe { std::mem::transmute::<u16, u16>([[T_0]] as u16) };
// CHECK-NEXT:     }
// CHECK-NEXT:     w
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%u %u %ld %u\n".as_ptr(),
// CHECK-NEXT:             add_via_rbx(40u32, 2u32),
// CHECK-NEXT:             double_in_rbx(21u32),
// CHECK-NEXT:             scaled_sum(3i64, 10i64),
// CHECK-NEXT:             (low_word(305419896u32) as u32) as i32,
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
