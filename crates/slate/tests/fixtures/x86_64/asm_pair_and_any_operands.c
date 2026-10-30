#include <stdio.h>

typedef struct { long a, b, c; } big_t;

static unsigned __int128 pair_out(void) {
  unsigned __int128 r;
  __asm__("movq $7, %%rax\n\tmovq $9, %%rdx # %0" : "=A"(r));
  return r;
}

static unsigned __int128 pair_inout(unsigned __int128 x) {
  __asm__("addq $1, %q0\n\tadcq $0, %%rdx" : "+A"(x));
  return x;
}

static unsigned long pair_in_high(unsigned __int128 x) {
  unsigned long hi;
  __asm__("movq %%rdx, %0" : "=r"(hi) : "A"(x));
  return hi;
}

static unsigned long single_a(unsigned long x) {
  unsigned long r;
  __asm__("leaq 3(%1), %0" : "=A"(r) : "l"(x));
  return r;
}

static int any_operands(int x, big_t *big) {
  __asm__ volatile("" : : "X"(x), "X"(5), "X"(*big));
  int r;
  __asm__("movl $42, %0" : "=X"(r));
  return r + x - 40;
}

int main(void) {
  unsigned __int128 a = pair_out();
  unsigned __int128 b = pair_inout(((unsigned __int128)4 << 64) | ~0UL);
  big_t big = {1, 2, 3};
  printf("%lu %lu\n", (unsigned long)(a >> 64), (unsigned long)a);
  printf("%lu %lu\n", (unsigned long)(b >> 64), (unsigned long)b);
  printf("%lu %lu %d\n", pair_in_high(((unsigned __int128)11 << 64) | 12),
         single_a(39), any_operands(40, &big));
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct __SlateRecord_asm_pair_and_any_operands_c_3 {
// CHECK-NEXT:     a: i64,
// CHECK-NEXT:     b: i64,
// CHECK-NEXT:     c: i64,
// CHECK-NEXT: }
// CHECK-NEXT: fn pair_out() -> u128 {
// CHECK-NEXT:     let mut r: u128 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u64;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movq $7, %rax\n\tmovq $9, %rdx # %rax", lateout("ax") [[T_0]], lateout("dx")
// CHECK-NEXT:             [[T_1]], options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r = (([[T_1]] as u128) << 64 | ([[T_0]] as u128)) as u128;
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn pair_inout(mut x: u128) -> u128 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u128 = x as u128;
// CHECK-NEXT:         let [[T_1]]: u64;
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "addq $1, %rax\n\tadcq $0, %rdx", inlateout("ax") [[T_0]] as u64 => [[T_1]],
// CHECK-NEXT:             inlateout("dx") ([[T_0]] >> 64) as u64 => [[T_2]], options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         x = (([[T_2]] as u128) << 64 | ([[T_1]] as u128)) as u128;
// CHECK-NEXT:     }
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-NEXT: fn pair_in_high(x: u128) -> u64 {
// CHECK-NEXT:     let mut hi: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         let [[T_1]]: u128 = x as u128;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movq %rdx, {0:r}", lateout(reg) [[T_0]], in ("ax") [[T_1]] as u64, in ("dx") ([[T_1]]
// CHECK-NEXT:             >> 64) as u64, options(pure, nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         hi = unsafe { std::mem::transmute::<u64, u64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     hi
// CHECK-NEXT: }
// CHECK-NEXT: fn single_a(mut x: u64) -> u64 {
// CHECK-NEXT:     let mut r: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "leaq 3({0:r}), %rax", in (reg) x as u64, lateout("ax") [[T_0]], options(pure,
// CHECK-NEXT:             nomem, nostack, att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r = unsafe { std::mem::transmute::<u64, u64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     r
// CHECK-NEXT: }
// CHECK-NEXT: fn any_operands(
// CHECK-NEXT:     mut x: i32,
// CHECK-NEXT:     mut big: &mut __SlateRecord_asm_pair_and_any_operands_c_3,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "\n# {0:e}\n# {1}\n# {2}", in (reg) x as u32, const 5 as i32, in (reg) big as
// CHECK-NEXT:             * mut __SlateRecord_asm_pair_and_any_operands_c_3, options(nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut r: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "movl $42, {0:e}", lateout(reg) [[T_0]], options(pure, nomem, nostack,
// CHECK-NEXT:             att_syntax)
// CHECK-NEXT:         );
// CHECK-NEXT:         r = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     r + x - 40
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: u128 = pair_out();
// CHECK-NEXT:     let b: u128 = pair_inout((4 as u128) << 64i32 | (18446744073709551615u64 as u128));
// CHECK-NEXT:     let mut big: __SlateRecord_asm_pair_and_any_operands_c_3 = __SlateRecord_asm_pair_and_any_operands_c_3 {
// CHECK-NEXT:         a: 1,
// CHECK-NEXT:         b: 2,
// CHECK-NEXT:         c: 3,
// CHECK-NEXT:     };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lu %lu\n".as_ptr(), (a >> 64i32) as u64, a as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lu %lu\n".as_ptr(), (b >> 64i32) as u64, b as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"%lu %lu %d\n".as_ptr(),
// CHECK-NEXT:             pair_in_high((11 as u128) << 64i32 | 12),
// CHECK-NEXT:             single_a(39u64),
// CHECK-NEXT:             any_operands(40i32, &mut (*std::ptr::addr_of_mut!(big))),
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
