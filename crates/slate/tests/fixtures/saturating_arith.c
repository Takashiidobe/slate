#include <limits.h>
#include <stdio.h>

typedef unsigned __int128 U128;
typedef _BitInt(200) S200;
typedef unsigned _BitInt(200) U200;
typedef int v4si __attribute__((vector_size(16)));

int main(void) {
  int add_i = __builtin_elementwise_add_sat(INT_MAX - 5, 10);
  printf("%d\n", add_i);

  int add_i_no_sat = __builtin_elementwise_add_sat(2, 3);
  printf("%d\n", add_i_no_sat);

  short sub_s = __builtin_elementwise_sub_sat((short)(SHRT_MIN + 5), (short)10);
  printf("%d\n", sub_s);

  unsigned sub_u = __builtin_elementwise_sub_sat(5u, 10u);
  printf("%u\n", sub_u);

  U128 u128a    = (U128)0 - 1;
  U128 add_u128 = __builtin_elementwise_add_sat(u128a, (U128)5);
  printf("%d\n", add_u128 == (U128)0 - 1);

  S200 s200a    = -1;
  S200 add_s200 = __builtin_elementwise_add_sat(s200a, (S200)5);
  printf("%d\n", (int)add_s200);

  U200 u200a    = 3;
  U200 sub_u200 = __builtin_elementwise_sub_sat(u200a, (U200)10);
  printf("%d\n", sub_u200 == 0);

  v4si va = {INT_MAX, 1, INT_MIN, 0};
  v4si vb = {10, 1, -10, 0};
  v4si vr = __builtin_elementwise_add_sat(va, vb);
  printf("%d %d %d %d\n", vr[0], vr[1], vr[2], vr[3]);

  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let add_i: i32 = 2147483642i32.saturating_add(10i32);
// CHECK-NEXT:     println!("{}", add_i);
// CHECK-NEXT:     let add_i_no_sat: i32 = 2i32.saturating_add(3i32);
// CHECK-NEXT:     println!("{}", add_i_no_sat);
// CHECK-NEXT:     let sub_s: i16 = (-32763i16).saturating_sub(10i16);
// CHECK-NEXT:     println!("{}", sub_s as i32);
// CHECK-NEXT:     let sub_u: u32 = 5u32.saturating_sub(10u32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%u\n".as_ptr(), sub_u) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let u128a: u128 = (0 as u128).wrapping_sub(1 as u128);
// CHECK-NEXT:     let add_u128: u128 = u128a.saturating_add(5 as u128);
// CHECK-NEXT:     println!("{}", (add_u128 == (0 as u128).wrapping_sub(1 as u128)) as i32);
// CHECK-NEXT:     let mut s200a: bitint::BInt<200, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(-1 as i128);
// CHECK-NEXT:     let mut add_s200: bitint::BInt<200, 4, 32, bitint::A8> = s200a
// CHECK-NEXT:         .saturating_add(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(5 as i128));
// CHECK-NEXT:     println!("{}", add_s200.to_i128() as i32);
// CHECK-NEXT:     let mut u200a: bitint::BUint<200, 4, 32, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:         200,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(3 as i128));
// CHECK-NEXT:     let mut sub_u200: bitint::BUint<200, 4, 32, bitint::A8> = u200a
// CHECK-NEXT:         .saturating_sub(
// CHECK-NEXT:             <bitint::BUint<
// CHECK-NEXT:                 200,
// CHECK-NEXT:                 4,
// CHECK-NEXT:                 32,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_bint(<bitint::BInt<200, 4, 32, bitint::A8>>::from_i128(10 as i128)),
// CHECK-NEXT:         );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", (sub_u200.compare(< bitint::BUint < 200, 4, 32, bitint::A8 >>::from_bint(<
// CHECK-NEXT:         bitint::BInt < 200, 4, 32, bitint::A8 >>::from_i128(0 as i128))) == 0) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let va: std::simd::Simd<i32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         2147483647i32,
// CHECK-NEXT:         1i32,
// CHECK-NEXT:         -2147483648i32,
// CHECK-NEXT:         0i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let vb: std::simd::Simd<i32, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         10i32,
// CHECK-NEXT:         1i32,
// CHECK-NEXT:         -10i32,
// CHECK-NEXT:         0i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let vr: std::simd::Simd<i32, 4> = std::simd::num::SimdInt::saturating_add(va, vb);
// CHECK-NEXT:     println!("{} {} {} {}", vr[0], vr[1], vr[2], vr[3]);
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
