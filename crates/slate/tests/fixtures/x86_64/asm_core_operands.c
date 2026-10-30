#include <stdio.h>

static int core(int a, int b) {
  int output, tied, early;
  __asm__ volatile("leal 3(%1), %0" : "=r"(output) : "r"(a) : "cc");
  __asm__ volatile("addl $2, %0" : "+r"(output) : : "cc");
  __asm__("imull %2, %0" : "=r"(tied) : "0"(output), "r"(b) : "cc");
  __asm__("movl %1, %0\n\taddl %2, %0" : "=&r"(early) : "r"(tied), "r"(1) : "cc");
  __asm__("addl $1, %0" : "+&r"(early) : : "cc");
  return early;
}

int main(void) {
  int x = 3;
  int *p = &x;
  volatile int y = 4;
  __asm__ volatile("addl $2, %0" : "+r"(*p++) : : "memory", "cc");
  __asm__ volatile("addl $3, %0" : "+r"(y) : : "cc");
  __asm__ volatile("movl $19, %%eax; movl %%eax, %0" : "=r"(x) : : "eax", "cc");
  __asm__ volatile("# %% literal %{braces%}" : : : "memory");
  __asm__ volatile("nop");
  __asm__ volatile("" : : : "unwind");
  int constant;
  __asm__("movl %1, %0" : "=r"(constant) : "i"(7));
  printf("%d %d %d %d %ld\n", core(7, 2), x, y, constant, (long)(p - &x));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(asm_unwind)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn core(mut a: i32, mut b: i32) -> i32 {
// CHECK-NEXT:     let mut output: i32 = 0;
// CHECK-NEXT:     let mut tied: i32 = 0;
// CHECK-NEXT:     let mut early: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leal 3({1:e}), {0:e}", lateout(reg) [[T_0]], in (reg) a as u32,
// CHECK-NEXT:             options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         output = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl $2, {0:e}", inlateout(reg) output as u32 => [[T_1]], options(nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         output = unsafe { std::mem::transmute::<u32, i32>([[T_1]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "imull {1:e}, {0:e}", inlateout(reg) output as u32 => [[T_2]], in (reg) b as
// CHECK-NEXT:             u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         tied = unsafe { std::mem::transmute::<u32, i32>([[T_2]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl {1:e}, {0:e}\n\taddl {2:e}, {0:e}", out(reg) [[T_3]], in (reg) tied as
// CHECK-NEXT:             u32, in (reg) (1 as i32) as u32, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         early = unsafe { std::mem::transmute::<u32, i32>([[T_3]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl $1, {0:e}", inout(reg) early as u32 => [[T_4]], options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         early = unsafe { std::mem::transmute::<u32, i32>([[T_4]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     early
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: i32 = 3;
// CHECK-NEXT:     let mut p: *mut i32 = std::ptr::addr_of_mut!(x);
// CHECK-NEXT:     let mut y: i32 = 4;
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: *mut i32 = p;
// CHECK-NEXT:     p = unsafe { [[V_0]].add(1) };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl $2, {0:e}", inlateout(reg) * [[V_0]] as u32 => [[T_0]], options(nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         *[[V_0]] = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_1]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addl $3, {0:e}", inlateout(reg)
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(y)) as u32 => [[T_1]],
// CHECK-NEXT:             options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         std::ptr::write_volatile(
// CHECK-NEXT:             std::ptr::addr_of_mut!(y),
// CHECK-NEXT:             unsafe { std::mem::transmute::<u32, i32>([[T_1]] as u32) },
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_2]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl $19, %eax; movl %eax, {0:e}", lateout(reg) [[T_2]], lateout("ax") _,
// CHECK-NEXT:             options(nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = unsafe { std::mem::transmute::<u32, i32>([[T_2]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("# % literal {{[{][{]}}braces}}", options(nostack, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("nop", options(att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!("", options(nostack, may_unwind, att_syntax));
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut constant: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_3]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl ${1}, {0:e}", lateout(reg) [[T_3]], const 7 as i32, options(pure, nomem,
// CHECK-NEXT:             nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         constant = unsafe { std::mem::transmute::<u32, i32>([[T_3]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%d %d %d %d %ld\n".as_ptr(),
// CHECK-NEXT:             core(7i32, 2i32),
// CHECK-NEXT:             x,
// CHECK-NEXT:             std::ptr::read_volatile(std::ptr::addr_of!(y)),
// CHECK-NEXT:             constant,
// CHECK-NEXT:             p.offset_from(std::ptr::addr_of_mut!(x) as *mut i32) as i64,
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
