#include <stdio.h>

int main(void) {
  __int128 a = ((__int128)1 << 126), b = ((__int128)1 << 126), r;
  int o = __builtin_add_overflow(a, b, &r);
  printf("%d %d\n", o, (int)(r >> 100));
  unsigned __int128 ua = ~(unsigned __int128)0, ur;
  o = __builtin_add_overflow(ua, (unsigned __int128)1, &ur);
  printf("%d %d\n", o, (int)ur);
  o = __builtin_mul_overflow(ua, (unsigned __int128)2, &ur);
  printf("%d %d\n", o, (int)(ur >> 120));
  o = __builtin_sub_overflow(a, (__int128)-1, &r);
  printf("%d\n", o);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let a: i128 = (1 as i128) << 126i32;
// CHECK-NEXT:     let b: i128 = (1 as i128) << 126i32;
// CHECK-NEXT:     let mut r: i128 = 0;
// CHECK-NEXT:     let mut o: i32 = ({
// CHECK-NEXT:         let [[T_0:__t[0-9]+]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_i128(a as i128);
// CHECK-NEXT:         let [[T_1:__t[0-9]+]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_i128(b as i128);
// CHECK-NEXT:         let [[T_2:__t[0-9]+]]: bitint::BInt<258, 5, 40, bitint::A8> = [[T_0]].wrapping_add([[T_1]]);
// CHECK-NEXT:         let [[T_3:__t[0-9]+]]: i128 = [[T_2]].to_i128() as i128;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_3]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_2]].compare(<bitint::BInt<258, 5, 40, bitint::A8>>::from_i128([[T_3]] as i128))
// CHECK-NEXT:             != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     println!("{} {}", o, (r >> 100i32) as i32);
// CHECK-NEXT:     let ua: u128 = !0;
// CHECK-NEXT:     let mut ur: u128 = 0;
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_4:__t[0-9]+]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_u128(ua as u128);
// CHECK-NEXT:         let [[T_5:__t[0-9]+]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_u128(1 as u128);
// CHECK-NEXT:         let [[T_6:__t[0-9]+]]: bitint::BInt<258, 5, 40, bitint::A8> = [[T_4]].wrapping_add([[T_5]]);
// CHECK-NEXT:         let [[T_7:__t[0-9]+]]: u128 = [[T_6]].to_i128() as u128;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(ur) = [[T_7]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_6]].compare(<bitint::BInt<258, 5, 40, bitint::A8>>::from_u128([[T_7]] as u128))
// CHECK-NEXT:             != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     println!("{} {}", o, (ur as u32) as i32);
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_8:__t[0-9]+]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_u128(ua as u128);
// CHECK-NEXT:         let [[T_9:__t[0-9]+]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_u128(2 as u128);
// CHECK-NEXT:         let [[T_10:__t[0-9]+]]: bitint::BInt<258, 5, 40, bitint::A8> = [[T_8]].wrapping_mul([[T_9]]);
// CHECK-NEXT:         let [[T_11:__t[0-9]+]]: u128 = [[T_10]].to_i128() as u128;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(ur) = [[T_11]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_10]].compare(<bitint::BInt<258, 5, 40, bitint::A8>>::from_u128([[T_11]] as u128))
// CHECK-NEXT:             != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     println!("{} {}", o, ((ur >> 120i32) as u32) as i32);
// CHECK-NEXT:     o = ({
// CHECK-NEXT:         let [[T_12:__t[0-9]+]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_i128(a as i128);
// CHECK-NEXT:         let [[T_13:__t[0-9]+]]: bitint::BInt<258, 5, 40, bitint::A8> = <bitint::BInt<
// CHECK-NEXT:             258,
// CHECK-NEXT:             5,
// CHECK-NEXT:             40,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_i128(-1 as i128);
// CHECK-NEXT:         let [[T_14:__t[0-9]+]]: bitint::BInt<258, 5, 40, bitint::A8> = [[T_12]].wrapping_sub([[T_13]]);
// CHECK-NEXT:         let [[T_15:__t[0-9]+]]: i128 = [[T_14]].to_i128() as i128;
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *std::ptr::addr_of_mut!(r) = [[T_15]];
// CHECK-NEXT:         }
// CHECK-NEXT:         [[T_14]].compare(<bitint::BInt<258, 5, 40, bitint::A8>>::from_i128([[T_15]] as i128))
// CHECK-NEXT:             != 0
// CHECK-NEXT:     }) as i32;
// CHECK-NEXT:     println!("{}", o);
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
