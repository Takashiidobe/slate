// { dg-options "-O2" }
#include <stdio.h>
static inline __attribute__((always_inline)) unsigned add_imm(unsigned x, int n) {
    unsigned out;
    __asm__("addl %2, %0" : "=r"(out) : "0"(x), "i"(n));
    return out;
}
static inline __attribute__((always_inline)) unsigned add_inplace(unsigned x, int n) {
    __asm__("addl %1, %0" : "+r"(x) : "i"(n));
    return x;
}
static inline __attribute__((always_inline)) int counted(int n) {
    static int calls;
    int out;
    calls++;
    __asm__("movl %1, %0" : "=r"(out) : "i"(n));
    return out + calls;
}
static inline __attribute__((always_inline)) unsigned load_g(unsigned *p, int n) {
    unsigned out;
    __asm__("movl %1, %0\n\taddl %2, %0" : "=r"(out) : "g"(*p), "i"(n));
    return out;
}
int main(void) {
    unsigned v = 7;
    printf("%u %u %d %d %d %u\n", add_imm(40, 2), add_inplace(1, 5), counted(3), counted(3),
           counted(4), load_g(&v, 3));
    return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-O2" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn add_imm__const1(mut x: u32, n: i32) -> u32 {
// CHECK-NEXT:     let mut out: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!("addl ${1}, {0:e}", inlateout(reg) x as u32 => [[T_0]], const 2 as i32, options(pure, nomem, nostack, att_syntax));
// CHECK-NEXT:         out = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn add_inplace__const2(mut x: u32, n: i32) -> u32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("addl ${1}, {0:e}", inlateout(reg) x as u32 => [[T_0]], const 5 as i32, options(pure, nomem, nostack, att_syntax));
// CHECK-NEXT:         x = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn counted__const3(n: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = calls + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("movl ${1}, {0:e}", lateout(reg) [[T_0]], const 3 as i32, options(pure, nomem, nostack, att_syntax));
// CHECK-NEXT:         out = unsafe { core::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out + unsafe { calls }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn counted__const4(n: i32) -> i32 {
// CHECK-NEXT:     let mut out: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         calls = calls + 1;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("movl ${1}, {0:e}", lateout(reg) [[T_0]], const 4 as i32, options(pure, nomem, nostack, att_syntax));
// CHECK-NEXT:         out = unsafe { core::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out + unsafe { calls }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut calls: i32 = unsafe { core::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn load_g__const5(mut p: &u32, n: i32) -> u32 {
// CHECK-NEXT:     let mut out: u32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!("movl ({1:r}), {0:e}\n\taddl ${2}, {0:e}", lateout(reg) [[T_0]], in(reg) p as *const u32, const 3 as i32, options(pure, readonly, nostack, att_syntax));
// CHECK-NEXT:         out = unsafe { core::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     out
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut v: u32 = 7;
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%u %u %d %d %d %u\n".as_ptr(),
// CHECK-NEXT:             add_imm__const1(40u32, 2i32),
// CHECK-NEXT:             add_inplace__const2(1u32, 5i32),
// CHECK-NEXT:             counted__const3(3i32),
// CHECK-NEXT:             counted__const3(3i32),
// CHECK-NEXT:             counted__const4(4i32),
// CHECK-NEXT:             load_g__const5(&(*core::ptr::addr_of_mut!(v)), 3i32),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
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
