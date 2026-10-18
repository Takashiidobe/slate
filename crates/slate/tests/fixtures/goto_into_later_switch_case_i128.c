#include <stdio.h>

static int classify(_BitInt(128) c) {
  switch (c) {
  case 1:
  case 2:
    break;
  case 99999999999999999999999999999wb:
    goto low;
  case 4:
    goto high;
  default:
    return -1;
  }
  for (;;) {
    switch (c) {
    case 5:
    low:
      return 100;
    case 6:
    high:
      return 200;
    default:
      return -2;
    }
  }
}

int main() {
  printf("%d\n", classify(1));
  printf("%d\n", classify(99999999999999999999999999999wb));
  printf("%d\n", classify(4));
  printf("%d\n", classify(5));
  printf("%d\n", classify(6));
  printf("%d\n", classify(9));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn classify(mut c: bitint::BInt<128, 2, 16, bitint::A8>) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             '[[JOIN_1:__join_[0-9]+]]: {
// CHECK-NEXT:                 '[[JOIN_2:__join_[0-9]+]]: {
// CHECK-NEXT:                     let [[T_0:__t[0-9]+]]: i128 = c.to_i128();
// CHECK-NEXT:                     if [[T_0]]
// CHECK-NEXT:                         != <bitint::BInt<128, 2, 16, bitint::A8>>::from_u128(1).to_i128()
// CHECK-NEXT:                     {
// CHECK-NEXT:                         if [[T_0]]
// CHECK-NEXT:                             != <bitint::BInt<128, 2, 16, bitint::A8>>::from_u128(2)
// CHECK-NEXT:                                 .to_i128()
// CHECK-NEXT:                         {
// CHECK-NEXT:                             if [[T_0]]
// CHECK-NEXT:                                 == <bitint::BInt<
// CHECK-NEXT:                                     128,
// CHECK-NEXT:                                     2,
// CHECK-NEXT:                                     16,
// CHECK-NEXT:                                     bitint::A8,
// CHECK-NEXT:                                 >>::from_u128(99999999999999999999999999999)
// CHECK-NEXT:                                     .to_i128()
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 break '[[JOIN_0]];
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 if [[T_0]]
// CHECK-NEXT:                                     == <bitint::BInt<128, 2, 16, bitint::A8>>::from_u128(4)
// CHECK-NEXT:                                         .to_i128()
// CHECK-NEXT:                                 {
// CHECK-NEXT:                                     break '[[JOIN_1]];
// CHECK-NEXT:                                 } else {
// CHECK-NEXT:                                     return -1;
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let [[T_1:__t[0-9]+]]: i128 = c.to_i128();
// CHECK-NEXT:                 if [[T_1]] == <bitint::BInt<128, 2, 16, bitint::A8>>::from_u128(5).to_i128()
// CHECK-NEXT:                 {
// CHECK-NEXT:                     break '[[JOIN_0]];
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if [[T_1]]
// CHECK-NEXT:                         != <bitint::BInt<128, 2, 16, bitint::A8>>::from_u128(6).to_i128()
// CHECK-NEXT:                     {
// CHECK-NEXT:                         return -2;
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:             return 200;
// CHECK-NEXT:         }
// CHECK-NEXT:         return 100;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", classify(< bitint::BInt < 128, 2, 16, bitint::A8 >>::from_i128(1 as i128))
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", classify(< bitint::BInt < 128, 2, 16, bitint::A8 >>::from_bint(<
// CHECK-NEXT:         bitint::BInt < 98, 2, 16, bitint::A8
// CHECK-NEXT:         >>::from_u128(99999999999999999999999999999)))
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", classify(< bitint::BInt < 128, 2, 16, bitint::A8 >>::from_i128(4 as i128))
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", classify(< bitint::BInt < 128, 2, 16, bitint::A8 >>::from_i128(5 as i128))
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", classify(< bitint::BInt < 128, 2, 16, bitint::A8 >>::from_i128(6 as i128))
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", classify(< bitint::BInt < 128, 2, 16, bitint::A8 >>::from_i128(9 as i128))
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
