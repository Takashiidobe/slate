#include <stdio.h>
#include <limits.h>

#define FLAG(name, cond)                                                      \
  static int name(int left, int right) {                                      \
    int flag;                                                                 \
    __asm__("cmp %w1, %w2" : "=@cc" #cond(flag) : "r"(left), "r"(right));     \
    return flag;                                                              \
  }

FLAG(f_eq, eq)
FLAG(f_ne, ne)
FLAG(f_cs, cs)
FLAG(f_cc, cc)
FLAG(f_hs, hs)
FLAG(f_lo, lo)
FLAG(f_mi, mi)
FLAG(f_pl, pl)
FLAG(f_vs, vs)
FLAG(f_vc, vc)
FLAG(f_hi, hi)
FLAG(f_ls, ls)
FLAG(f_ge, ge)
FLAG(f_lt, lt)
FLAG(f_gt, gt)
FLAG(f_le, le)

static int sign_zero(int value) {
  unsigned char minus;
  _Bool nonzero;

  __asm__("cmp %w2, #0" : "=@ccmi"(minus), "=@ccne"(nonzero) : "r"(value));
  return minus * 10 + nonzero;
}

static const int pairs[][2] = {
    {5, 5}, {9, 4}, {4, 9}, {-1, 1}, {1, -1}, {INT_MIN, 1}, {INT_MAX, -1}, {0, 0},
};

int main(void) {
  for (unsigned i = 0; i < sizeof pairs / sizeof pairs[0]; i++) {
    int a = pairs[i][0], b = pairs[i][1];
    printf("%d %d: %d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d\n", a, b, f_eq(a, b),
           f_ne(a, b), f_cs(a, b), f_cc(a, b), f_hs(a, b), f_lo(a, b),
           f_mi(a, b), f_pl(a, b), f_vs(a, b), f_vc(a, b), f_hi(a, b),
           f_ls(a, b), f_ge(a, b), f_lt(a, b), f_gt(a, b), f_le(a, b));
  }
  printf("%d %d %d\n", sign_zero(-3), sign_zero(0), sign_zero(7));
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: fn f_eq(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, eq", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_ne(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, ne", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_cs(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, cs", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_cc(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, cc", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_hs(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, hs", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_lo(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, lo", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_mi(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, mi", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_pl(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, pl", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_vs(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, vs", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_vc(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, vc", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_hi(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, hi", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_ls(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, ls", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_ge(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, ge", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_lt(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, lt", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_gt(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, gt", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn f_le(mut left: i32, mut right: i32) -> i32 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {1:w}, {2:w}\ncset {0:w}, le", lateout(reg) [[T_0]], in (reg) left as u32,
// CHECK-NEXT:             in (reg) right as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn sign_zero(mut value: i32) -> i32 {
// CHECK-NEXT:     let mut minus: u8 = 0;
// CHECK-NEXT:     let mut nonzero: bool = false;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {2:w}, #0\ncset {0:w}, mi\ncset {1:w}, ne", lateout(reg) [[T_0]],
// CHECK-NEXT:             lateout(reg) [[T_1]], in (reg) value as u32, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         minus = unsafe { std::mem::transmute::<u8, u8>([[T_0]] as u8) };
// CHECK-NEXT:         nonzero = ([[T_1]] as u8) != 0;
// CHECK-NEXT:     }
// CHECK-NEXT:     (minus as i32) * 10 + (nonzero as i32)
// CHECK-NEXT: }
// CHECK-NEXT: static mut pairs: {{\[\[}}i32; 2]; 8] = [
// CHECK-NEXT:     [5, 5],
// CHECK-NEXT:     [9, 4],
// CHECK-NEXT:     [4, 9],
// CHECK-NEXT:     [-1, 1],
// CHECK-NEXT:     [1, -1],
// CHECK-NEXT:     [-2147483648, 1],
// CHECK-NEXT:     [2147483647, -1],
// CHECK-NEXT:     [0, 0],
// CHECK-NEXT: ];
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64)
// CHECK-NEXT:         < (std::mem::size_of::<{{\[\[}}i32; 2]; 8]>() as u64)
// CHECK-NEXT:             / (std::mem::size_of::<[i32; 2]>() as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         let a: i32 = unsafe { pairs[i as usize][0] };
// CHECK-NEXT:         let b: i32 = unsafe { pairs[i as usize][1] };
// CHECK-NEXT:         println!(
// CHECK-NEXT:             "{} {}: {}{}{}{}{}{}{}{}{}{}{}{}{}{}{}{}", a, b, f_eq(a, b), f_ne(a, b),
// CHECK-NEXT:             f_cs(a, b), f_cc(a, b), f_hs(a, b), f_lo(a, b), f_mi(a, b), f_pl(a, b),
// CHECK-NEXT:             f_vs(a, b), f_vc(a, b), f_hi(a, b), f_ls(a, b), f_ge(a, b), f_lt(a, b),
// CHECK-NEXT:             f_gt(a, b), f_le(a, b)
// CHECK-NEXT:         );
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     println!("{} {} {}", sign_zero(- 3i32), sign_zero(0i32), sign_zero(7i32));
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
