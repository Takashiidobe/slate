#include <stdio.h>

typedef unsigned int U32;

static int add_via_asm(int x) {
  __asm__ volatile("addl $5, %0" : "+r"(x));
  return x;
}

static U32 cpuid_leaf7_feature(void) {
  U32 eax, ebx, ecx;
  __asm__("cpuid" : "=a"(eax), "=b"(ebx), "=c"(ecx) : "a"(7), "c"(0) : "edx");
  return ebx != 0 || eax != 0 || ecx != 0;
}

int main(void) {
  printf("%d %u\n", add_via_asm(37), cpuid_leaf7_feature());
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn add_via_asm(mut x: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl $5, {0:e}", inlateout(reg) x as u32 => [[T_0]], options(nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn cpuid_leaf7_feature() -> u32 {
// CHECK-NEXT:     let mut eax: u32 = 0;
// CHECK-NEXT:     let mut ebx: u32 = 0;
// CHECK-NEXT:     let mut ecx: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "xchg {0:r}, %rbx\ncpuid\nxchg {0:r}, %rbx", out(reg) [[T_1]], lateout("ax")
// CHECK-NEXT:             [[T_0]], lateout("cx") [[T_2]], in ("ax") (7 as i32) as u32, in ("cx") (0 as i32)
// CHECK-NEXT:             as u32, lateout("dx") _, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         eax = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:         ebx = unsafe { std::mem::transmute::<u32, u32>([[T_1]] as u32) };
// CHECK-NEXT:         ecx = unsafe { std::mem::transmute::<u32, u32>([[T_2]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     (ebx != 0 || eax != 0 || ecx != 0) as u32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%d %u\n".as_ptr(), add_via_asm(37 as i32), cpuid_leaf7_feature())
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
