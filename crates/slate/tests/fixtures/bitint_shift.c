#include <stdio.h>

void abort(void);

int shift_by_promoted_types(int seed) {
  int                i   = 4;
  unsigned           u   = 3;
  long               l   = 5;
  unsigned long long ull = 6;
  short              s   = 2;

  unsigned _BitInt(129) a   = 1;
  a                         = a << i;
  a                         = a >> u;
  a                       <<= l;
  a                       >>= s;
  a                         = a << ull;
  a                         = a + (unsigned _BitInt(129))seed;
  return (int)a;
}

int shift_across_limbs(int seed) {
  unsigned _BitInt(129) wide = 1;
  wide                       = wide << 128;
  wide                       = wide >> 127;
  return (int)wide + seed;
}

int shift_signed_arithmetic(int seed) {
  _BitInt(256) n = -1024;
  n              = n >> 3;
  n              = n << 2;
  return (int)n + seed;
}

int shift_by_bitint(int seed) {
  _BitInt(256) amount = 4;
  _BitInt(256) v      = 3;
  v                   = v << amount;
  v                   = v >> amount;
  return (int)v + seed;
}

int main(void) {
  if (shift_by_promoted_types(0) != 1024)
    abort();
  if (shift_across_limbs(0) != 2)
    abort();
  if (shift_signed_arithmetic(0) != -512)
    abort();
  if (shift_by_bitint(0) != 3)
    abort();
  printf("%d %d %d %d\n", shift_by_promoted_types(1), shift_across_limbs(2),
         shift_signed_arithmetic(3), shift_by_bitint(4));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn shift_by_promoted_types(seed: i32) -> i32 {
// CHECK-NEXT:     let i: i32 = 4;
// CHECK-NEXT:     let u: u32 = 3;
// CHECK-NEXT:     let l: i64 = 5;
// CHECK-NEXT:     let ull: u64 = 6;
// CHECK-NEXT:     let s: i16 = 2;
// CHECK-NEXT:     let mut a: bitint::BUint<129, 3, 24, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:         129,
// CHECK-NEXT:         3,
// CHECK-NEXT:         24,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(<bitint::BInt<129, 3, 24, bitint::A8>>::from_i128(1 as i128));
// CHECK-NEXT:     a = a.shl_bits(i as usize);
// CHECK-NEXT:     a = a.shr_bits(u as usize);
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bitint::BUint<129, 3, 24, bitint::A8> = a;
// CHECK-NEXT:     a = [[V_0]].shl_bits(l as usize);
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: bitint::BUint<129, 3, 24, bitint::A8> = a;
// CHECK-NEXT:     a = [[V_1]].shr_bits((s as i32) as usize);
// CHECK-NEXT:     a = a.shl_bits(ull as usize);
// CHECK-NEXT:     a = a
// CHECK-NEXT:         .wrapping_add(
// CHECK-NEXT:             <bitint::BUint<
// CHECK-NEXT:                 129,
// CHECK-NEXT:                 3,
// CHECK-NEXT:                 24,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_bint(
// CHECK-NEXT:                 <bitint::BInt<129, 3, 24, bitint::A8>>::from_i128(seed as i128),
// CHECK-NEXT:             ),
// CHECK-NEXT:         );
// CHECK-NEXT:     (a.to_u128() as u32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn shift_across_limbs(seed: i32) -> i32 {
// CHECK-NEXT:     let mut wide: bitint::BUint<129, 3, 24, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:         129,
// CHECK-NEXT:         3,
// CHECK-NEXT:         24,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(<bitint::BInt<129, 3, 24, bitint::A8>>::from_i128(1 as i128));
// CHECK-NEXT:     wide = wide.shl_bits(128 as usize);
// CHECK-NEXT:     wide = wide.shr_bits(127 as usize);
// CHECK-NEXT:     ((wide.to_u128() as u32) as i32) + seed
// CHECK-NEXT: }
// CHECK-NEXT: fn shift_signed_arithmetic(seed: i32) -> i32 {
// CHECK-NEXT:     let mut n: bitint::BInt<256, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         256,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(-1024 as i128);
// CHECK-NEXT:     n = n.shr_bits(3 as usize);
// CHECK-NEXT:     n = n.shl_bits(2 as usize);
// CHECK-NEXT:     (n.to_i128() as i32) + seed
// CHECK-NEXT: }
// CHECK-NEXT: fn shift_by_bitint(seed: i32) -> i32 {
// CHECK-NEXT:     let mut amount: bitint::BInt<256, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         256,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(4 as i128);
// CHECK-NEXT:     let mut v: bitint::BInt<256, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         256,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_i128(3 as i128);
// CHECK-NEXT:     v = v.shl_bits(amount.to_i128() as usize);
// CHECK-NEXT:     v = v.shr_bits(amount.to_i128() as usize);
// CHECK-NEXT:     (v.to_i128() as i32) + seed
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if shift_by_promoted_types(0 as i32) != 1024 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if shift_across_limbs(0 as i32) != 2 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if shift_signed_arithmetic(0 as i32) != -512 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if shift_by_bitint(0 as i32) != 3 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {}", shift_by_promoted_types(1 as i32), shift_across_limbs(2 as i32),
// CHECK-NEXT:         shift_signed_arithmetic(3 as i32), shift_by_bitint(4 as i32)
// CHECK-NEXT:     );
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
