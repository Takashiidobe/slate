#include <stdio.h>

typedef _BitInt(256) big;

int classify(int seed) {
  big s = 170141183460469231731687303715884105727wb + (big)seed;
  switch (s) {
  case 170141183460469231731687303715884105727wb ... 170141183460469231731687303715884105730wb:
    return 1;
  case 170141183460469231731687303715884105740wb:
  case 170141183460469231731687303715884105745wb ... 170141183460469231731687303715884105747wb:
    return 2;
  default:
    return 0;
  }
}

int probe(int seed) {
  big s = 170141183460469231731687303715884105727wb + (big)seed;
  int v = 42;
  switch (s) {
  case 170141183460469231731687303715884105727wb ... 170141183460469231731687303715884105730wb:
    return v;
  default:
    return 0;
  }
}

int main(void) {
  printf("%d %d %d %d %d %d\n", classify(0), classify(13), classify(19),
         classify(5), probe(2), probe(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(seed: i32) -> i32 {
// CHECK-NEXT:     let s: bitint::BInt<256, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         256,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(
// CHECK-NEXT:             <bitint::BInt<
// CHECK-NEXT:                 128,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_u128(170141183460469231731687303715884105727),
// CHECK-NEXT:         )
// CHECK-NEXT:         .wrapping_add(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(seed as i128));
// CHECK-NEXT:     let [[T_0:__t[0-9]+]]: bitint::BInt<256, 4, 32, bitint::A8> = s;
// CHECK-NEXT:     let [[T_1:__t[0-9]+]]: i64 = if [[T_0]]
// CHECK-NEXT:         .compare(
// CHECK-NEXT:             <bitint::BInt<
// CHECK-NEXT:                 256,
// CHECK-NEXT:                 4,
// CHECK-NEXT:                 32,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_u128(170141183460469231731687303715884105727),
// CHECK-NEXT:         ) >= 0
// CHECK-NEXT:         && [[T_0]]
// CHECK-NEXT:             .compare(
// CHECK-NEXT:                 <bitint::BInt<
// CHECK-NEXT:                     256,
// CHECK-NEXT:                     4,
// CHECK-NEXT:                     32,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_u128(170141183460469231731687303715884105730),
// CHECK-NEXT:             ) <= 0
// CHECK-NEXT:     {
// CHECK-NEXT:         0
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if [[T_0]]
// CHECK-NEXT:             .compare(
// CHECK-NEXT:                 <bitint::BInt<
// CHECK-NEXT:                     256,
// CHECK-NEXT:                     4,
// CHECK-NEXT:                     32,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_u128(170141183460469231731687303715884105740),
// CHECK-NEXT:             ) == 0
// CHECK-NEXT:             || [[T_0]]
// CHECK-NEXT:                 .compare(
// CHECK-NEXT:                     <bitint::BInt<
// CHECK-NEXT:                         256,
// CHECK-NEXT:                         4,
// CHECK-NEXT:                         32,
// CHECK-NEXT:                         bitint::A8,
// CHECK-NEXT:                     >>::from_u128(170141183460469231731687303715884105745),
// CHECK-NEXT:                 ) >= 0
// CHECK-NEXT:                 && [[T_0]]
// CHECK-NEXT:                     .compare(
// CHECK-NEXT:                         <bitint::BInt<
// CHECK-NEXT:                             256,
// CHECK-NEXT:                             4,
// CHECK-NEXT:                             32,
// CHECK-NEXT:                             bitint::A8,
// CHECK-NEXT:                         >>::from_u128(170141183460469231731687303715884105747),
// CHECK-NEXT:                     ) <= 0
// CHECK-NEXT:         {
// CHECK-NEXT:             1
// CHECK-NEXT:         } else {
// CHECK-NEXT:             2
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     loop {
// CHECK-NEXT:         match [[T_1]] {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 return 1;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 return 2;
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 return 0;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn probe(seed: i32) -> i32 {
// CHECK-NEXT:     let s: bitint::BInt<256, 4, 32, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:         256,
// CHECK-NEXT:         4,
// CHECK-NEXT:         32,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(
// CHECK-NEXT:             <bitint::BInt<
// CHECK-NEXT:                 128,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_u128(170141183460469231731687303715884105727),
// CHECK-NEXT:         )
// CHECK-NEXT:         .wrapping_add(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(seed as i128));
// CHECK-NEXT:     let v: i32 = 42;
// CHECK-NEXT:     let [[T_0]]: bitint::BInt<256, 4, 32, bitint::A8> = s;
// CHECK-NEXT:     let [[T_1]]: i64 = if [[T_0]]
// CHECK-NEXT:         .compare(
// CHECK-NEXT:             <bitint::BInt<
// CHECK-NEXT:                 256,
// CHECK-NEXT:                 4,
// CHECK-NEXT:                 32,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_u128(170141183460469231731687303715884105727),
// CHECK-NEXT:         ) >= 0
// CHECK-NEXT:         && [[T_0]]
// CHECK-NEXT:             .compare(
// CHECK-NEXT:                 <bitint::BInt<
// CHECK-NEXT:                     256,
// CHECK-NEXT:                     4,
// CHECK-NEXT:                     32,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_u128(170141183460469231731687303715884105730),
// CHECK-NEXT:             ) <= 0
// CHECK-NEXT:     {
// CHECK-NEXT:         0
// CHECK-NEXT:     } else {
// CHECK-NEXT:         1
// CHECK-NEXT:     };
// CHECK-NEXT:     loop {
// CHECK-NEXT:         match [[T_1]] {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 return v;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 return 0;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}", classify(0i32), classify(13i32), classify(19i32),
// CHECK-NEXT:         classify(5i32), probe(2i32), probe(9i32)
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
