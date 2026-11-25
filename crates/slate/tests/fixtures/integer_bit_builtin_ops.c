#include <stdio.h>

int main(void) {
  unsigned int u  = 0x12345678u;
  unsigned int z  = 0u;
  int          s  = -12345;
  unsigned int sh = 5u;

  unsigned int rev            = __builtin_bitreverse32(u);
  unsigned int swapped        = __builtin_bswap32(u);
  int          leading        = __builtin_clz(u);
  int          trailing       = __builtin_ctz(u);
  int          first_set      = __builtin_ffs((int)u);
  int          zero_first     = __builtin_ffs((int)z);
  int          ones           = __builtin_popcount(u);
  int          odd            = __builtin_parity(u);
  int          redundant_sign = __builtin_clrsb(s);
  unsigned int left           = __builtin_rotateleft32(u, sh);
  unsigned int right          = __builtin_rotateright32(u, sh);

  printf("%u %u %d %d %d %d %d %d %d %u %u\n", rev, swapped, leading, trailing,
         first_set, zero_first, ones, odd, redundant_sign, left, right);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu i686-unknown-linux-gnu aarch64-unknown-linux-gnu armv7-unknown-linux-gnueabihf
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(link_llvm_intrinsics)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn __builtin_rotateleft32([[ARG_0:arg[0-9]+]]: u32, [[ARG_1:arg[0-9]+]]: u32) -> u32;
// CHECK-NEXT:     fn __builtin_rotateright32([[ARG_0]]: u32, [[ARG_1]]: u32) -> u32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "llvm-intrinsic" {
// CHECK-NEXT:     #[link_name = "llvm.bswap.i32"]
// CHECK-NEXT:     fn __slate_intrinsic_e94e9cf815168b51([[ARG_0]]: u32) -> u32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let u: u32 = 305419896;
// CHECK-NEXT:     let z: u32 = 0;
// CHECK-NEXT:     let s: i32 = -12345;
// CHECK-NEXT:     let sh: u32 = 5;
// CHECK-NEXT:     let rev: u32 = u32::reverse_bits(u);
// CHECK-NEXT:     let swapped: u32 = unsafe { __slate_intrinsic_e94e9cf815168b51(u) };
// CHECK-NEXT:     let leading: i32 = u.leading_zeros() as i32;
// CHECK-NEXT:     let trailing: i32 = u.trailing_zeros() as i32;
// CHECK-NEXT:     let first_set: i32 = {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = u as i32;
// CHECK-NEXT:         if [[T_0]] == 0 { 0i32 } else { ([[T_0]].trailing_zeros() as i32) + 1 }
// CHECK-NEXT:     };
// CHECK-NEXT:     let zero_first: i32 = {
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = z as i32;
// CHECK-NEXT:         if [[T_1]] == 0 { 0i32 } else { ([[T_1]].trailing_zeros() as i32) + 1 }
// CHECK-NEXT:     };
// CHECK-NEXT:     let ones: i32 = u.count_ones() as i32;
// CHECK-NEXT:     let odd: i32 = {
// CHECK-NEXT:         let [[T_2:__t[0-9]+]] = u;
// CHECK-NEXT:         ([[T_2]].count_ones() & 1) as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let redundant_sign: i32 = {
// CHECK-NEXT:         let [[T_3:__t[0-9]+]] = s;
// CHECK-NEXT:         ((if [[T_3]] < 0 { (![[T_3]]).leading_zeros() } else { [[T_3]].leading_zeros() }) - 1)
// CHECK-NEXT:             as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     let left: u32 = u.rotate_left(sh as u32);
// CHECK-NEXT:     let right: u32 = u.rotate_right(sh as u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%u %u %d %d %d %d %d %d %d %u %u\n".as_ptr(),
// CHECK-NEXT:             rev,
// CHECK-NEXT:             swapped,
// CHECK-NEXT:             leading,
// CHECK-NEXT:             trailing,
// CHECK-NEXT:             first_set,
// CHECK-NEXT:             zero_first,
// CHECK-NEXT:             ones,
// CHECK-NEXT:             odd,
// CHECK-NEXT:             redundant_sign,
// CHECK-NEXT:             left,
// CHECK-NEXT:             right,
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
