#include <stdio.h>

static int carry_in(int value) {
  int flag = value;
  __asm__("nop" : "+@cccs"(flag));
  return flag;
}

static int zero_in(unsigned value) {
  unsigned flag = value;
  __asm__("nop" : "+@cceq"(flag));
  return flag;
}

static long wide_in(long value) {
  long flag = value;
  __asm__("nop" : "+@ccmi"(flag));
  return flag;
}

static unsigned long flags_name(int left, int right) {
  int flag;
  unsigned long bits;
  __asm__("cmp %w2, %w3\n\tmrs %1, %w0" : "=@cceq"(flag), "=r"(bits) : "r"(left), "r"(right));
  return bits >> 28;
}

int main(void) {
  printf("%d %d %d\n", carry_in(0x20000000), carry_in(0), carry_in(-1));
  printf("%d %d\n", zero_in(0x40000000u), zero_in(5));
  printf("%ld %ld\n", wide_in(0x80000000L), wide_in(1L));
  printf("%lx %lx\n", flags_name(3, 3), flags_name(1, 2));
  return 0;
}

// SLATE-FILECHECK-BEGIN aarch64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn carry_in(mut flag: i32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "msr nzcv, {1:x}\nnop\ncset {0:w}, cs", out(reg) [[T_0]], in (reg) (flag as u32)
// CHECK-NEXT:             as u64, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn zero_in(mut flag: u32) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "msr nzcv, {1:x}\nnop\ncset {0:w}, eq", out(reg) [[T_0]], in (reg) (flag as u32)
// CHECK-NEXT:             as u64, options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, u32>([[T_0]] as u32) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn wide_in(mut flag: i64) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "msr nzcv, {1:x}\nnop\ncset {0:w}, mi", out(reg) [[T_0]], in (reg) flag as u64,
// CHECK-NEXT:             options(pure, nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u64, i64>([[T_0]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     flag
// CHECK-NEXT: }
// CHECK-NEXT: fn flags_name(mut left: i32, mut right: i32) -> u64 {
// CHECK-NEXT:     let mut flag: i32 = 0;
// CHECK-NEXT:     let mut bits: u64 = 0;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         let [[T_0]]: u32;
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: u64;
// CHECK-NEXT:         core::arch::asm!(
// CHECK-NEXT:             "cmp {2:w}, {3:w}\n\tmrs {1:x}, nzcv\ncset {0:w}, eq", lateout(reg) [[T_0]],
// CHECK-NEXT:             lateout(reg) [[T_1]], in (reg) left as u32, in (reg) right as u32, options(pure,
// CHECK-NEXT:             nomem, nostack)
// CHECK-NEXT:         );
// CHECK-NEXT:         flag = unsafe { std::mem::transmute::<u32, i32>([[T_0]] as u32) };
// CHECK-NEXT:         bits = unsafe { std::mem::transmute::<u64, u64>([[T_1]] as u64) };
// CHECK-NEXT:     }
// CHECK-NEXT:     bits >> 28i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!("{} {} {}", carry_in(536870912i32), carry_in(0i32), carry_in(- 1i32));
// CHECK-NEXT:     println!("{} {}", zero_in(1073741824u32), zero_in(5u32));
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld %ld\n".as_ptr(),
// CHECK-NEXT:             wide_in(2147483648i64),
// CHECK-NEXT:             wide_in(1i64),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lx %lx\n".as_ptr(),
// CHECK-NEXT:             flags_name(3i32, 3i32),
// CHECK-NEXT:             flags_name(1i32, 2i32),
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
