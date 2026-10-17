#include <stdio.h>

static int classify(_BitInt(200) c) {
  switch (c) {
  case 1:
  case 2:
    break;
  case 123456789012345678901234567890123456789012345wb:
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
  printf("%d\n", classify(123456789012345678901234567890123456789012345wb));
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
// CHECK-NEXT: fn classify(c: bitint::BInt<200, 4, 32, bitint::A8>) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         '[[JOIN_0:__join_[0-9]+]]: {
// CHECK-NEXT:             '[[JOIN_1:__join_[0-9]+]]: {
// CHECK-NEXT:                 '[[JOIN_2:__join_[0-9]+]]: {
// CHECK-NEXT:                     let [[T_0:__t[0-9]+]]: bitint::BInt<200, 4, 32, bitint::A8> = c;
// CHECK-NEXT:                     if [[T_0]] == <bitint::BInt<200, 4, 32, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                     {} else {
// CHECK-NEXT:                         if [[T_0]] == <bitint::BInt<200, 4, 32, bitint::A8>>::from_u128(2)
// CHECK-NEXT:                         {} else {
// CHECK-NEXT:                             if [[T_0]]
// CHECK-NEXT:                                 == <bitint::BInt<
// CHECK-NEXT:                                     200,
// CHECK-NEXT:                                     4,
// CHECK-NEXT:                                     32,
// CHECK-NEXT:                                     bitint::A8,
// CHECK-NEXT:                                 >>::from_le_bytes::<
// CHECK-NEXT:                                     160,
// CHECK-NEXT:                                     20,
// CHECK-NEXT:                                 >([
// CHECK-NEXT:                                     121, 223, 226, 241, 116, 124, 91, 239, 146, 170, 195, 158,
// CHECK-NEXT:                                     103, 173, 39, 229, 54, 137, 5, 0,
// CHECK-NEXT:                                 ])
// CHECK-NEXT:                             {
// CHECK-NEXT:                                 break '[[JOIN_0]];
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 if [[T_0]]
// CHECK-NEXT:                                     == <bitint::BInt<200, 4, 32, bitint::A8>>::from_u128(4)
// CHECK-NEXT:                                 {
// CHECK-NEXT:                                     break '[[JOIN_1]];
// CHECK-NEXT:                                 } else {
// CHECK-NEXT:                                     return -1;
// CHECK-NEXT:                                 }
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                 }
// CHECK-NEXT:                 let [[T_1:__t[0-9]+]]: bitint::BInt<200, 4, 32, bitint::A8> = c;
// CHECK-NEXT:                 if [[T_1]] == <bitint::BInt<200, 4, 32, bitint::A8>>::from_u128(5) {
// CHECK-NEXT:                     break '[[JOIN_0]];
// CHECK-NEXT:                 } else {
// CHECK-NEXT:                     if [[T_1]] == <bitint::BInt<200, 4, 32, bitint::A8>>::from_u128(6)
// CHECK-NEXT:                     {} else {
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
// CHECK-NEXT:         "{}", classify(< bitint::BInt < 200, 4, 32, bitint::A8 >>::from_i128(1 as i128))
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", classify(< bitint::BInt < 200, 4, 32, bitint::A8 >>::from_bint(<
// CHECK-NEXT:         bitint::BInt < 148, 3, 24, bitint::A8 >>::from_le_bytes::< 160, 20 > ([121, 223,
// CHECK-NEXT:         226, 241, 116, 124, 91, 239, 146, 170, 195, 158, 103, 173, 39, 229, 54, 137, 5,
// CHECK-NEXT:         0])))
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", classify(< bitint::BInt < 200, 4, 32, bitint::A8 >>::from_i128(4 as i128))
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", classify(< bitint::BInt < 200, 4, 32, bitint::A8 >>::from_i128(5 as i128))
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", classify(< bitint::BInt < 200, 4, 32, bitint::A8 >>::from_i128(6 as i128))
// CHECK-NEXT:     );
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "{}", classify(< bitint::BInt < 200, 4, 32, bitint::A8 >>::from_i128(9 as i128))
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
