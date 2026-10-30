#include <stdio.h>

typedef unsigned _BitInt(5) u5;
typedef unsigned _BitInt(9) u9;
typedef unsigned _BitInt(33) u33;
typedef unsigned _BitInt(64) u64b;
typedef unsigned _BitInt(100) u100;
typedef unsigned _BitInt(128) u128b;

static const int counts[] = {0, 1, 2, 3, 4, 5, 7, 9, 31, 33, 63, 64, 65, 100, 127, 130, 199, 257, -1, -3, -5, -64, -101, 1000};

#define ROT_NARROW(NAME, T, SEED)                                              \
  static void NAME(void) {                                                     \
    T x = SEED;                                                                \
    for (unsigned i = 0; i < sizeof counts / sizeof counts[0]; i++) {          \
      T l = __builtin_stdc_rotate_left(x, counts[i]);                          \
      T r = __builtin_stdc_rotate_right(x, counts[i]);                         \
      printf("%llu %llu\n", (unsigned long long)l, (unsigned long long)r);    \
    }                                                                          \
  }

#define ROT_WIDE(NAME, T, SEED)                                                \
  static void NAME(void) {                                                     \
    T x = SEED;                                                                \
    for (unsigned i = 0; i < sizeof counts / sizeof counts[0]; i++) {          \
      T l = __builtin_stdc_rotate_left(x, counts[i]);                          \
      T r = __builtin_stdc_rotate_right(x, counts[i]);                         \
      printf("%llu %llu %llu %llu\n", (unsigned long long)l,                  \
             (unsigned long long)(l >> 64), (unsigned long long)r,             \
             (unsigned long long)(r >> 64));                                   \
    }                                                                          \
  }

ROT_NARROW(r_uchar, unsigned char, 0xa5)
ROT_NARROW(r_ushort, unsigned short, 0x8c31)
ROT_NARROW(r_uint, unsigned int, 0x80c0ffeeu)
ROT_NARROW(r_ull, unsigned long long, 0x8123456789abcdefULL)
ROT_NARROW(r_u5, u5, 0x15)
ROT_NARROW(r_u9, u9, 0x1a5)
ROT_NARROW(r_u33, u33, 0x1c0ffee11uwb)
ROT_NARROW(r_u64, u64b, 0xf00dfacecafebeefuwb)
ROT_WIDE(r_u100, u100, (((u100)1 << 99) | 0x1234567890abcdefuwb))
ROT_WIDE(r_u128b, u128b, (((u128b)0xdeadbeefULL << 90) | 7))

int main(void) {
  r_uchar();
  r_ushort();
  r_uint();
  r_ull();
  r_u5();
  r_u9();
  r_u33();
  r_u64();
  r_u100();
  r_u128b();
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut counts: __SlateAlign16<[i32; 24]> = __SlateAlign16([
// CHECK-NEXT:     0i32,
// CHECK-NEXT:     1i32,
// CHECK-NEXT:     2i32,
// CHECK-NEXT:     3i32,
// CHECK-NEXT:     4i32,
// CHECK-NEXT:     5i32,
// CHECK-NEXT:     7i32,
// CHECK-NEXT:     9i32,
// CHECK-NEXT:     31i32,
// CHECK-NEXT:     33i32,
// CHECK-NEXT:     63i32,
// CHECK-NEXT:     64i32,
// CHECK-NEXT:     65i32,
// CHECK-NEXT:     100i32,
// CHECK-NEXT:     127i32,
// CHECK-NEXT:     130i32,
// CHECK-NEXT:     199i32,
// CHECK-NEXT:     257i32,
// CHECK-NEXT:     -1i32,
// CHECK-NEXT:     -3i32,
// CHECK-NEXT:     -5i32,
// CHECK-NEXT:     -64i32,
// CHECK-NEXT:     -101i32,
// CHECK-NEXT:     1000i32,
// CHECK-NEXT: ]);
// CHECK-NEXT: fn r_uchar() {
// CHECK-NEXT:     let x: u8 = 165u8;
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < 96 / 4 {
// CHECK-NEXT:         let l: u8 = x
// CHECK-NEXT:             .rotate_left(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(8) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let r: u8 = x
// CHECK-NEXT:             .rotate_right(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(8) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%llu %llu\n".as_ptr(), l as u64, r as u64) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn r_ushort() {
// CHECK-NEXT:     let x: u16 = 35889u16;
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < 96 / 4 {
// CHECK-NEXT:         let l: u16 = x
// CHECK-NEXT:             .rotate_left(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(16) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let r: u16 = x
// CHECK-NEXT:             .rotate_right(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(16) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%llu %llu\n".as_ptr(), l as u64, r as u64) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn r_uint() {
// CHECK-NEXT:     let x: u32 = 2160132078;
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < 96 / 4 {
// CHECK-NEXT:         let l: u32 = x
// CHECK-NEXT:             .rotate_left(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(32) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let r: u32 = x
// CHECK-NEXT:             .rotate_right(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(32) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%llu %llu\n".as_ptr(), l as u64, r as u64) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn r_ull() {
// CHECK-NEXT:     let x: u64 = 9305357566071262703;
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < 96 / 4 {
// CHECK-NEXT:         let l: u64 = x
// CHECK-NEXT:             .rotate_left(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(64) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let r: u64 = x
// CHECK-NEXT:             .rotate_right(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(64) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%llu %llu\n".as_ptr(), l, r) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn r_u5() {
// CHECK-NEXT:     let x: u8 = 21u8;
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (i as u64) < 96 / 4 {
// CHECK-NEXT:         let l: u8 = {
// CHECK-NEXT:             let [[T_0:__t[0-9]+]] = x;
// CHECK-NEXT:             let [[T_1:__t[0-9]+]] = ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(5) as u32;
// CHECK-NEXT:             (([[T_0]] << [[T_1]] | [[T_0]] >> (5 - [[T_1]]) % 5) as u8) & 31
// CHECK-NEXT:         };
// CHECK-NEXT:         let r: u8 = {
// CHECK-NEXT:             let [[T_2:__t[0-9]+]] = x;
// CHECK-NEXT:             let [[T_3:__t[0-9]+]] = ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(5) as u32;
// CHECK-NEXT:             (([[T_2]] >> [[T_3]] | [[T_2]] << (5 - [[T_3]]) % 5) as u8) & 31
// CHECK-NEXT:         };
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%llu %llu\n".as_ptr(), l as u64, r as u64) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn r_u9() {
// CHECK-NEXT:     let x: u16 = 421u16;
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0]]: while (i as u64) < 96 / 4 {
// CHECK-NEXT:         let l: u16 = {
// CHECK-NEXT:             let [[T_0]] = x;
// CHECK-NEXT:             let [[T_1]] = ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(9) as u32;
// CHECK-NEXT:             (([[T_0]] << [[T_1]] | [[T_0]] >> (9 - [[T_1]]) % 9) as u16) & 511
// CHECK-NEXT:         };
// CHECK-NEXT:         let r: u16 = {
// CHECK-NEXT:             let [[T_2]] = x;
// CHECK-NEXT:             let [[T_3]] = ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(9) as u32;
// CHECK-NEXT:             (([[T_2]] >> [[T_3]] | [[T_2]] << (9 - [[T_3]]) % 9) as u16) & 511
// CHECK-NEXT:         };
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%llu %llu\n".as_ptr(), l as u64, r as u64) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn r_u33() {
// CHECK-NEXT:     let x: u64 = 7532965393;
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0]]: while (i as u64) < 96 / 4 {
// CHECK-NEXT:         let l: u64 = {
// CHECK-NEXT:             let [[T_0]] = x;
// CHECK-NEXT:             let [[T_1]] = ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(33) as u32;
// CHECK-NEXT:             (([[T_0]] << [[T_1]] | [[T_0]] >> (33 - [[T_1]]) % 33) as u64) & 8589934591
// CHECK-NEXT:         };
// CHECK-NEXT:         let r: u64 = {
// CHECK-NEXT:             let [[T_2]] = x;
// CHECK-NEXT:             let [[T_3]] = ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(33) as u32;
// CHECK-NEXT:             (([[T_2]] >> [[T_3]] | [[T_2]] << (33 - [[T_3]]) % 33) as u64) & 8589934591
// CHECK-NEXT:         };
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%llu %llu\n".as_ptr(), l as u64, r as u64) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn r_u64() {
// CHECK-NEXT:     let x: u64 = 17297757509875842799;
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < 96 / 4 {
// CHECK-NEXT:         let l: u64 = x
// CHECK-NEXT:             .rotate_left(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(64) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let r: u64 = x
// CHECK-NEXT:             .rotate_right(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(64) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"%llu %llu\n".as_ptr(), l as u64, r as u64) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn r_u100() {
// CHECK-NEXT:     let mut x: bitint::BUint<100, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:         100,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_bint(<bitint::BInt<100, 2, 16, bitint::A8>>::from_i128(1 as i128))
// CHECK-NEXT:         .shl_bits(99 as usize)
// CHECK-NEXT:         .bitor_bits(
// CHECK-NEXT:             <bitint::BUint<
// CHECK-NEXT:                 100,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_u128(1311768467294899695 as u128),
// CHECK-NEXT:         );
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < 96 / 4 {
// CHECK-NEXT:         let mut l: bitint::BUint<100, 2, 16, bitint::A8> = x
// CHECK-NEXT:             .rotate_left_bits(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(100) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let mut r: bitint::BUint<100, 2, 16, bitint::A8> = x
// CHECK-NEXT:             .rotate_right_bits(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(100) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%llu %llu %llu %llu\n".as_ptr(),
// CHECK-NEXT:                 l.to_u128() as u64,
// CHECK-NEXT:                 l.shr_bits(64 as usize).to_u128() as u64,
// CHECK-NEXT:                 r.to_u128() as u64,
// CHECK-NEXT:                 r.shr_bits(64 as usize).to_u128() as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn r_u128b() {
// CHECK-NEXT:     let mut x: bitint::BUint<128, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:         128,
// CHECK-NEXT:         2,
// CHECK-NEXT:         16,
// CHECK-NEXT:         bitint::A8,
// CHECK-NEXT:     >>::from_u128(3735928559 as u128)
// CHECK-NEXT:         .shl_bits(90 as usize)
// CHECK-NEXT:         .bitor_bits(
// CHECK-NEXT:             <bitint::BUint<
// CHECK-NEXT:                 128,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_bint(<bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(7 as i128)),
// CHECK-NEXT:         );
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64) < 96 / 4 {
// CHECK-NEXT:         let mut l: bitint::BUint<128, 2, 16, bitint::A8> = x
// CHECK-NEXT:             .rotate_left_bits(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(128) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let mut r: bitint::BUint<128, 2, 16, bitint::A8> = x
// CHECK-NEXT:             .rotate_right_bits(
// CHECK-NEXT:                 ((unsafe { counts.0[i as usize] }) as i64).rem_euclid(128) as u32,
// CHECK-NEXT:             );
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             printf(
// CHECK-NEXT:                 c"%llu %llu %llu %llu\n".as_ptr(),
// CHECK-NEXT:                 l.to_u128() as u64,
// CHECK-NEXT:                 l.shr_bits(64 as usize).to_u128() as u64,
// CHECK-NEXT:                 r.to_u128() as u64,
// CHECK-NEXT:                 r.shr_bits(64 as usize).to_u128() as u64,
// CHECK-NEXT:             )
// CHECK-NEXT:         };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     r_uchar();
// CHECK-NEXT:     r_ushort();
// CHECK-NEXT:     r_uint();
// CHECK-NEXT:     r_ull();
// CHECK-NEXT:     r_u5();
// CHECK-NEXT:     r_u9();
// CHECK-NEXT:     r_u33();
// CHECK-NEXT:     r_u64();
// CHECK-NEXT:     r_u100();
// CHECK-NEXT:     r_u128b();
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
