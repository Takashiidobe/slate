#include <stdio.h>

static unsigned cpuid_vendor_ebx(void) {
  unsigned eax = 0, ebx, ecx = 0, edx;
  __asm__("cpuid" : "+a"(eax), "=b"(ebx), "+c"(ecx), "=d"(edx));
  return ebx;
}

static unsigned add_via_ebx(unsigned a, unsigned b) {
  __asm__("addl %%ebx, %0" : "+r"(a) : "b"(b));
  return a;
}

static unsigned clobber_ebx(unsigned x) {
  __asm__("movl %0, %%ebx\n\tshll $1, %%ebx\n\tmovl %%ebx, %0" : "+r"(x) : : "ebx");
  return x;
}

static unsigned load_via_address(unsigned *p) {
  unsigned v;
  __asm__("movl %a1, %0" : "=r"(v) : "D"(p));
  return v;
}

int main(void) {
  unsigned word = 77;
  printf("%d %u %u %u\n", cpuid_vendor_ebx() != 0, add_via_ebx(40, 2),
         clobber_ebx(21), load_via_address(&word));
  return 0;
}

// SLATE-FILECHECK-BEGIN i686-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn cpuid_vendor_ebx() -> u32 {
// CHECK-NEXT:     let mut eax: u32 = 0;
// CHECK-NEXT:     let mut ebx: u32 = 0;
// CHECK-NEXT:     let mut ecx: u32 = 0;
// CHECK-NEXT:     let mut edx: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u32;
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cpuid", inlateout("ax") eax as u32 => [[T_0]], lateout("bx") [[T_1]],
// CHECK-NEXT:             inlateout("cx") ecx as u32 => [[T_2]], lateout("dx") [[T_3]], options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         eax = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:         ebx = unsafe { core::mem::transmute::<u32, u32>([[T_1]] as u32) };
// CHECK-NEXT:         ecx = unsafe { core::mem::transmute::<u32, u32>([[T_2]] as u32) };
// CHECK-NEXT:         edx = unsafe { core::mem::transmute::<u32, u32>([[T_3]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     ebx
// CHECK-NEXT: }
// CHECK-NEXT: fn add_via_ebx(mut a: u32, mut b: u32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl %ebx, {0:e}", inlateout(reg) a as u32 => [[T_0]], in ("bx") b as u32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         a = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     a
// CHECK-NEXT: }
// CHECK-NEXT: fn clobber_ebx(mut x: u32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl {0:e}, %ebx\n\tshll $1, %ebx\n\tmovl %ebx, {0:e}", inlateout(reg) x as
// CHECK-NEXT:             u32 => [[T_0]], out("bx") _, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn load_via_address(mut p: &u32) -> u32 {
// CHECK-NEXT:     let mut v: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl (%edi), {0:e}", lateout(reg) [[T_0]], in ("di") (p as * const u32) as u32,
// CHECK-NEXT:             options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         v = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     v
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut word: u32 = 77;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%d %u %u %u\n".as_ptr(),
// CHECK-NEXT:             (cpuid_vendor_ebx() != 0) as i32,
// CHECK-NEXT:             add_via_ebx(40u32, 2u32),
// CHECK-NEXT:             clobber_ebx(21u32),
// CHECK-NEXT:             load_via_address(&(*core::ptr::addr_of_mut!(word))),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
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
