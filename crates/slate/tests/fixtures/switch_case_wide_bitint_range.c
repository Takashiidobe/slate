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
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: fn classify(seed: i32) -> i32 {
// CHECK-NEXT:     let s: bitint::BInt<256, 4, 32, bitint::A8> = <bitint::BInt<256, 4, 32, bitint::A8>>::from_bint(
// CHECK-NEXT:             <bitint::BInt<128, 2, 16, bitint::A8>>::from_u128(
// CHECK-NEXT:                 170141183460469231731687303715884105727,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:         .wrapping_add(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(seed as i128));
// CHECK-NEXT:     let [[T_0:__t[0-9]+]]: bitint::BInt<256, 4, 32, bitint::A8> = s;
// CHECK-NEXT:     let [[T_1:__t[0-9]+]]: i64 = if [[T_0]]
// CHECK-NEXT:         .compare(
// CHECK-NEXT:             <bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(
// CHECK-NEXT:                 170141183460469231731687303715884105727,
// CHECK-NEXT:             ),
// CHECK-NEXT:         ) >= 0
// CHECK-NEXT:         && [[T_0]]
// CHECK-NEXT:             .compare(
// CHECK-NEXT:                 <bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(
// CHECK-NEXT:                     170141183460469231731687303715884105730,
// CHECK-NEXT:                 ),
// CHECK-NEXT:             ) <= 0 {
// CHECK-NEXT:         0
// CHECK-NEXT:     } else if [[T_0]]
// CHECK-NEXT:         .compare(
// CHECK-NEXT:             <bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(
// CHECK-NEXT:                 170141183460469231731687303715884105740,
// CHECK-NEXT:             ),
// CHECK-NEXT:         ) == 0
// CHECK-NEXT:         || [[T_0]]
// CHECK-NEXT:             .compare(
// CHECK-NEXT:                 <bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(
// CHECK-NEXT:                     170141183460469231731687303715884105745,
// CHECK-NEXT:                 ),
// CHECK-NEXT:             ) >= 0
// CHECK-NEXT:             && [[T_0]]
// CHECK-NEXT:                 .compare(
// CHECK-NEXT:                     <bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(
// CHECK-NEXT:                         170141183460469231731687303715884105747,
// CHECK-NEXT:                     ),
// CHECK-NEXT:                 ) <= 0 {
// CHECK-NEXT:         1
// CHECK-NEXT:     } else {
// CHECK-NEXT:         2
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
// CHECK-NEXT:     unsafe { core::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn probe(seed: i32) -> i32 {
// CHECK-NEXT:     let s: bitint::BInt<256, 4, 32, bitint::A8> = <bitint::BInt<256, 4, 32, bitint::A8>>::from_bint(
// CHECK-NEXT:             <bitint::BInt<128, 2, 16, bitint::A8>>::from_u128(
// CHECK-NEXT:                 170141183460469231731687303715884105727,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:         .wrapping_add(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(seed as i128));
// CHECK-NEXT:     let v: i32 = 42;
// CHECK-NEXT:     let [[T_0]]: bitint::BInt<256, 4, 32, bitint::A8> = s;
// CHECK-NEXT:     let [[T_1]]: i64 = if [[T_0]]
// CHECK-NEXT:         .compare(
// CHECK-NEXT:             <bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(
// CHECK-NEXT:                 170141183460469231731687303715884105727,
// CHECK-NEXT:             ),
// CHECK-NEXT:         ) >= 0
// CHECK-NEXT:         && [[T_0]]
// CHECK-NEXT:             .compare(
// CHECK-NEXT:                 <bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(
// CHECK-NEXT:                     170141183460469231731687303715884105730,
// CHECK-NEXT:                 ),
// CHECK-NEXT:             ) <= 0 {
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
// CHECK-NEXT:     unsafe { core::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{} {} {} {} {} {}",
// CHECK-NEXT:         classify(0i32),
// CHECK-NEXT:         classify(13i32),
// CHECK-NEXT:         classify(19i32),
// CHECK-NEXT:         classify(5i32),
// CHECK-NEXT:         probe(2i32),
// CHECK-NEXT:         probe(9i32)
// CHECK-NEXT:     );
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
