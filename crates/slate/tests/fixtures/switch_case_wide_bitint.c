#include <stdio.h>

typedef _BitInt(256) big;

int classify(int seed) {
  big v = 170141183460469231731687303715884105727wb + (big)seed;
  switch (v) {
  case 170141183460469231731687303715884105727wb:
  case 170141183460469231731687303715884105728wb:
  case 170141183460469231731687303715884105729wb:
    return 1;
  case 170141183460469231731687303715884105736wb:
    return 2;
  case -170141183460469231731687303715884105727wb:
    return 3;
  default:
    return 0;
  }
}

int main(void) {
  printf("%d %d %d %d\n", classify(0), classify(2), classify(9), classify(5));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(seed: i32) -> i32 {
// CHECK-NEXT:     let v: bitint::BInt<256, 4, 32, bitint::A8> = <bitint::BInt<
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
// CHECK-NEXT:     let [[T_0:__t[0-9]+]]: bitint::BInt<256, 4, 32, bitint::A8> = v;
// CHECK-NEXT:     let [[T_1:__t[0-9]+]]: i64 = if [[T_0]]
// CHECK-NEXT:         .compare(
// CHECK-NEXT:             <bitint::BInt<
// CHECK-NEXT:                 256,
// CHECK-NEXT:                 4,
// CHECK-NEXT:                 32,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_u128(170141183460469231731687303715884105727),
// CHECK-NEXT:         ) == 0
// CHECK-NEXT:         || [[T_0]]
// CHECK-NEXT:             .compare(
// CHECK-NEXT:                 <bitint::BInt<
// CHECK-NEXT:                     256,
// CHECK-NEXT:                     4,
// CHECK-NEXT:                     32,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_u128(170141183460469231731687303715884105728),
// CHECK-NEXT:             ) == 0
// CHECK-NEXT:         || [[T_0]]
// CHECK-NEXT:             .compare(
// CHECK-NEXT:                 <bitint::BInt<
// CHECK-NEXT:                     256,
// CHECK-NEXT:                     4,
// CHECK-NEXT:                     32,
// CHECK-NEXT:                     bitint::A8,
// CHECK-NEXT:                 >>::from_u128(170141183460469231731687303715884105729),
// CHECK-NEXT:             ) == 0
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
// CHECK-NEXT:                 >>::from_u128(170141183460469231731687303715884105736),
// CHECK-NEXT:             ) == 0
// CHECK-NEXT:         {
// CHECK-NEXT:             1
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if [[T_0]]
// CHECK-NEXT:                 .compare(
// CHECK-NEXT:                     <bitint::BInt<
// CHECK-NEXT:                         256,
// CHECK-NEXT:                         4,
// CHECK-NEXT:                         32,
// CHECK-NEXT:                         bitint::A8,
// CHECK-NEXT:                     >>::from_i128(-170141183460469231731687303715884105727),
// CHECK-NEXT:                 ) == 0
// CHECK-NEXT:             {
// CHECK-NEXT:                 2
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 3
// CHECK-NEXT:             }
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
// CHECK-NEXT:                 return 3;
// CHECK-NEXT:             }
// CHECK-NEXT:             3 => {
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
// CHECK-NEXT:         "{} {} {} {}", classify(0i32), classify(2i32), classify(9i32), classify(5i32)
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
