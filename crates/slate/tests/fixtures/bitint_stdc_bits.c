#include <stdio.h>

typedef unsigned _BitInt(1) u1;
typedef unsigned _BitInt(7) u7;
typedef unsigned _BitInt(13) u13;
typedef unsigned _BitInt(37) u37;
typedef unsigned _BitInt(64) u64b;
typedef unsigned _BitInt(65) u65;
typedef unsigned _BitInt(100) u100;
typedef unsigned _BitInt(128) u128b;

#define REPORT(T, X)                                                           \
  do {                                                                         \
    T x = (X);                                                                 \
    printf("%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n",                  \
           __builtin_stdc_leading_zeros(x), __builtin_stdc_leading_ones(x),    \
           __builtin_stdc_trailing_zeros(x), __builtin_stdc_trailing_ones(x),  \
           __builtin_stdc_first_leading_zero(x),                               \
           __builtin_stdc_first_leading_one(x),                                \
           __builtin_stdc_first_trailing_zero(x),                              \
           __builtin_stdc_first_trailing_one(x), __builtin_stdc_count_zeros(x),\
           __builtin_stdc_count_ones(x),                                       \
           (unsigned)__builtin_stdc_has_single_bit(x),                         \
           __builtin_stdc_bit_width(x),                                        \
           (unsigned long long)__builtin_stdc_bit_floor(x),                    \
           (unsigned long long)__builtin_stdc_bit_ceil(x));                    \
  } while (0)

static const unsigned long long seeds[] = {
    0, 1, 2, 3, 5, 8, 64, 100, 127, 128, 129, 255, 256, 4097, 65535, 65536,
    0x7fffffffULL, 0x80000000ULL, 0xffffffffULL, 0x100000000ULL,
    0x123456789abcdefULL, 0x7fffffffffffffffULL, 0x8000000000000000ULL,
    0xffffffffffffffffULL};

static void wide(void) {
  for (unsigned i = 0; i < sizeof seeds / sizeof seeds[0]; i++) {
    u100 w = (u100)seeds[i];
    REPORT(u100, w);
    REPORT(u100, w << 30);
    REPORT(u100, ~w);
    REPORT(u65, (u65)seeds[i]);
    REPORT(u65, ~(u65)seeds[i]);
    REPORT(u128b, (u128b)seeds[i] << 64);
    REPORT(u128b, ~(u128b)seeds[i]);
  }
}

int main(void) {
  for (unsigned i = 0; i < sizeof seeds / sizeof seeds[0]; i++) {
    unsigned long long s = seeds[i];
    REPORT(unsigned char, s);
    REPORT(unsigned short, s);
    REPORT(unsigned int, s);
    REPORT(unsigned long long, s);
    REPORT(u1, s);
    REPORT(u7, s);
    REPORT(u13, s);
    REPORT(u37, s);
    REPORT(u64b, s);
    REPORT(u7, ~(u7)s);
    REPORT(u13, ~(u13)s);
    REPORT(u37, ~(u37)s);
  }
  wide();
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: static mut seeds: __SlateAlign16<[u64; 24]> = __SlateAlign16([
// CHECK-NEXT:     ((0 as i32) as i64) as u64,
// CHECK-NEXT:     ((1 as i32) as i64) as u64,
// CHECK-NEXT:     ((2 as i32) as i64) as u64,
// CHECK-NEXT:     ((3 as i32) as i64) as u64,
// CHECK-NEXT:     ((5 as i32) as i64) as u64,
// CHECK-NEXT:     ((8 as i32) as i64) as u64,
// CHECK-NEXT:     ((64 as i32) as i64) as u64,
// CHECK-NEXT:     ((100 as i32) as i64) as u64,
// CHECK-NEXT:     ((127 as i32) as i64) as u64,
// CHECK-NEXT:     ((128 as i32) as i64) as u64,
// CHECK-NEXT:     ((129 as i32) as i64) as u64,
// CHECK-NEXT:     ((255 as i32) as i64) as u64,
// CHECK-NEXT:     ((256 as i32) as i64) as u64,
// CHECK-NEXT:     ((4097 as i32) as i64) as u64,
// CHECK-NEXT:     ((65535 as i32) as i64) as u64,
// CHECK-NEXT:     ((65536 as i32) as i64) as u64,
// CHECK-NEXT:     2147483647 as u64,
// CHECK-NEXT:     2147483648 as u64,
// CHECK-NEXT:     4294967295 as u64,
// CHECK-NEXT:     4294967296 as u64,
// CHECK-NEXT:     81985529216486895 as u64,
// CHECK-NEXT:     9223372036854775807 as u64,
// CHECK-NEXT:     9223372036854775808 as u64,
// CHECK-NEXT:     18446744073709551615 as u64,
// CHECK-NEXT: ]);
// CHECK-NEXT: fn wide() {
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (i as u64) < 192 / 8 {
// CHECK-NEXT:         let mut w: bitint::BUint<100, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_u128((unsafe { seeds.0[i as usize] }) as u128);
// CHECK-NEXT:         '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: bitint::BUint<100, 2, 16, bitint::A8> = w;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_0:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_0]].leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_1:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_1]]).leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_2:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_2]].trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_3:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_3]]).trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_4:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_5:__t[0-9]+]] = (![[T_4]]).leading_zeros();
// CHECK-NEXT:                             if [[T_5]] == 100 { 0 as u32 } else { [[T_5]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_6:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_7:__t[0-9]+]] = [[T_6]].leading_zeros();
// CHECK-NEXT:                             if [[T_7]] == 100 { 0 as u32 } else { [[T_7]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_8:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_9:__t[0-9]+]] = (![[T_8]]).trailing_zeros();
// CHECK-NEXT:                             if [[T_9]] == 100 { 0 as u32 } else { [[T_9]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_10:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_11:__t[0-9]+]] = [[T_10]].trailing_zeros();
// CHECK-NEXT:                             if [[T_11]] == 100 { 0 as u32 } else { [[T_11]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_12:__t[0-9]+]] = x;
// CHECK-NEXT:                         (100 - [[T_12]].count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_13:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_13]].count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_14:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_14]].count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_15:__t[0-9]+]] = x;
// CHECK-NEXT:                         (100 - [[T_15]].leading_zeros()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_16:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_16]].leading_zeros() == 100 {
// CHECK-NEXT:                             <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(0)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                 .shl_bits((99 - [[T_16]].leading_zeros()) as usize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_17:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_17]].count_ones() == 0 {
// CHECK-NEXT:                             <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_17]].count_ones() == 1 {
// CHECK-NEXT:                                 [[T_17]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                     .shl_bits((100 - [[T_17]].leading_zeros()) as usize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_2:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: bitint::BUint<100, 2, 16, bitint::A8> = w.shl_bits(30 as usize);
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_18:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_18]].leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_19:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_19]]).leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_20:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_20]].trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_21:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_21]]).trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_22:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_23:__t[0-9]+]] = (![[T_22]]).leading_zeros();
// CHECK-NEXT:                             if [[T_23]] == 100 { 0 as u32 } else { [[T_23]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_24:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_25:__t[0-9]+]] = [[T_24]].leading_zeros();
// CHECK-NEXT:                             if [[T_25]] == 100 { 0 as u32 } else { [[T_25]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_26:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_27:__t[0-9]+]] = (![[T_26]]).trailing_zeros();
// CHECK-NEXT:                             if [[T_27]] == 100 { 0 as u32 } else { [[T_27]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_28:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_29:__t[0-9]+]] = [[T_28]].trailing_zeros();
// CHECK-NEXT:                             if [[T_29]] == 100 { 0 as u32 } else { [[T_29]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_30:__t[0-9]+]] = x;
// CHECK-NEXT:                         (100 - [[T_30]].count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_31:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_31]].count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_32:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_32]].count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_33:__t[0-9]+]] = x;
// CHECK-NEXT:                         (100 - [[T_33]].leading_zeros()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_34:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_34]].leading_zeros() == 100 {
// CHECK-NEXT:                             <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(0)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                 .shl_bits((99 - [[T_34]].leading_zeros()) as usize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_35:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_35]].count_ones() == 0 {
// CHECK-NEXT:                             <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_35]].count_ones() == 1 {
// CHECK-NEXT:                                 [[T_35]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                     .shl_bits((100 - [[T_35]].leading_zeros()) as usize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_3:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: bitint::BUint<100, 2, 16, bitint::A8> = w.bitnot();
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_36:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_36]].leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_37:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_37]]).leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_38:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_38]].trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_39:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_39]]).trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_40:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_41:__t[0-9]+]] = (![[T_40]]).leading_zeros();
// CHECK-NEXT:                             if [[T_41]] == 100 { 0 as u32 } else { [[T_41]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_42:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_43:__t[0-9]+]] = [[T_42]].leading_zeros();
// CHECK-NEXT:                             if [[T_43]] == 100 { 0 as u32 } else { [[T_43]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_44:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_45:__t[0-9]+]] = (![[T_44]]).trailing_zeros();
// CHECK-NEXT:                             if [[T_45]] == 100 { 0 as u32 } else { [[T_45]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_46:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_47:__t[0-9]+]] = [[T_46]].trailing_zeros();
// CHECK-NEXT:                             if [[T_47]] == 100 { 0 as u32 } else { [[T_47]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_48:__t[0-9]+]] = x;
// CHECK-NEXT:                         (100 - [[T_48]].count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_49:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_49]].count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_50:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_50]].count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_51:__t[0-9]+]] = x;
// CHECK-NEXT:                         (100 - [[T_51]].leading_zeros()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_52:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_52]].leading_zeros() == 100 {
// CHECK-NEXT:                             <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(0)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                 .shl_bits((99 - [[T_52]].leading_zeros()) as usize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_53:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_53]].count_ones() == 0 {
// CHECK-NEXT:                             <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_53]].count_ones() == 1 {
// CHECK-NEXT:                                 [[T_53]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 <bitint::BUint<100, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                     .shl_bits((100 - [[T_53]].leading_zeros()) as usize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_4:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: bitint::BUint<65, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:                 65,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_u128((unsafe { seeds.0[i as usize] }) as u128);
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_54:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_54]].leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_55:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_55]]).leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_56:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_56]].trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_57:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_57]]).trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_58:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_59:__t[0-9]+]] = (![[T_58]]).leading_zeros();
// CHECK-NEXT:                             if [[T_59]] == 65 { 0 as u32 } else { [[T_59]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_60:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_61:__t[0-9]+]] = [[T_60]].leading_zeros();
// CHECK-NEXT:                             if [[T_61]] == 65 { 0 as u32 } else { [[T_61]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_62:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_63:__t[0-9]+]] = (![[T_62]]).trailing_zeros();
// CHECK-NEXT:                             if [[T_63]] == 65 { 0 as u32 } else { [[T_63]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_64:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_65:__t[0-9]+]] = [[T_64]].trailing_zeros();
// CHECK-NEXT:                             if [[T_65]] == 65 { 0 as u32 } else { [[T_65]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_66:__t[0-9]+]] = x;
// CHECK-NEXT:                         (65 - [[T_66]].count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_67:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_67]].count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_68:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_68]].count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_69:__t[0-9]+]] = x;
// CHECK-NEXT:                         (65 - [[T_69]].leading_zeros()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_70:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_70]].leading_zeros() == 65 {
// CHECK-NEXT:                             <bitint::BUint<65, 2, 16, bitint::A8>>::from_u128(0)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             <bitint::BUint<65, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                 .shl_bits((64 - [[T_70]].leading_zeros()) as usize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_71:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_71]].count_ones() == 0 {
// CHECK-NEXT:                             <bitint::BUint<65, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_71]].count_ones() == 1 {
// CHECK-NEXT:                                 [[T_71]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 <bitint::BUint<65, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                     .shl_bits((65 - [[T_71]].leading_zeros()) as usize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_5:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: bitint::BUint<65, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:                 65,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_u128((unsafe { seeds.0[i as usize] }) as u128)
// CHECK-NEXT:                 .bitnot();
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_72:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_72]].leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_73:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_73]]).leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_74:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_74]].trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_75:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_75]]).trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_76:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_77:__t[0-9]+]] = (![[T_76]]).leading_zeros();
// CHECK-NEXT:                             if [[T_77]] == 65 { 0 as u32 } else { [[T_77]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_78:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_79:__t[0-9]+]] = [[T_78]].leading_zeros();
// CHECK-NEXT:                             if [[T_79]] == 65 { 0 as u32 } else { [[T_79]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_80:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_81:__t[0-9]+]] = (![[T_80]]).trailing_zeros();
// CHECK-NEXT:                             if [[T_81]] == 65 { 0 as u32 } else { [[T_81]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_82:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_83:__t[0-9]+]] = [[T_82]].trailing_zeros();
// CHECK-NEXT:                             if [[T_83]] == 65 { 0 as u32 } else { [[T_83]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_84:__t[0-9]+]] = x;
// CHECK-NEXT:                         (65 - [[T_84]].count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_85:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_85]].count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_86:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_86]].count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_87:__t[0-9]+]] = x;
// CHECK-NEXT:                         (65 - [[T_87]].leading_zeros()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_88:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_88]].leading_zeros() == 65 {
// CHECK-NEXT:                             <bitint::BUint<65, 2, 16, bitint::A8>>::from_u128(0)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             <bitint::BUint<65, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                 .shl_bits((64 - [[T_88]].leading_zeros()) as usize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_89:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_89]].count_ones() == 0 {
// CHECK-NEXT:                             <bitint::BUint<65, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_89]].count_ones() == 1 {
// CHECK-NEXT:                                 [[T_89]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 <bitint::BUint<65, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                     .shl_bits((65 - [[T_89]].leading_zeros()) as usize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_6:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: bitint::BUint<128, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:                 128,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_u128((unsafe { seeds.0[i as usize] }) as u128)
// CHECK-NEXT:                 .shl_bits(64 as usize);
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_90:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_90]].leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_91:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_91]]).leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_92:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_92]].trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_93:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_93]]).trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_94:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_95:__t[0-9]+]] = (![[T_94]]).leading_zeros();
// CHECK-NEXT:                             if [[T_95]] == 128 { 0 as u32 } else { [[T_95]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_96:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_97:__t[0-9]+]] = [[T_96]].leading_zeros();
// CHECK-NEXT:                             if [[T_97]] == 128 { 0 as u32 } else { [[T_97]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_98:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_99:__t[0-9]+]] = (![[T_98]]).trailing_zeros();
// CHECK-NEXT:                             if [[T_99]] == 128 { 0 as u32 } else { [[T_99]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_100:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_101:__t[0-9]+]] = [[T_100]].trailing_zeros();
// CHECK-NEXT:                             if [[T_101]] == 128 { 0 as u32 } else { [[T_101]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_102:__t[0-9]+]] = x;
// CHECK-NEXT:                         (128 - [[T_102]].count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_103:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_103]].count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_104:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_104]].count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_105:__t[0-9]+]] = x;
// CHECK-NEXT:                         (128 - [[T_105]].leading_zeros()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_106:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_106]].leading_zeros() == 128 {
// CHECK-NEXT:                             <bitint::BUint<128, 2, 16, bitint::A8>>::from_u128(0)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             <bitint::BUint<128, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                 .shl_bits((127 - [[T_106]].leading_zeros()) as usize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_107:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_107]].count_ones() == 0 {
// CHECK-NEXT:                             <bitint::BUint<128, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_107]].count_ones() == 1 {
// CHECK-NEXT:                                 [[T_107]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 <bitint::BUint<128, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                     .shl_bits((128 - [[T_107]].leading_zeros()) as usize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_7:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: bitint::BUint<128, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:                 128,
// CHECK-NEXT:                 2,
// CHECK-NEXT:                 16,
// CHECK-NEXT:                 bitint::A8,
// CHECK-NEXT:             >>::from_u128((unsafe { seeds.0[i as usize] }) as u128)
// CHECK-NEXT:                 .bitnot();
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_108:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_108]].leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_109:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_109]]).leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_110:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_110]].trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_111:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_111]]).trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_112:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_113:__t[0-9]+]] = (![[T_112]]).leading_zeros();
// CHECK-NEXT:                             if [[T_113]] == 128 { 0 as u32 } else { [[T_113]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_114:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_115:__t[0-9]+]] = [[T_114]].leading_zeros();
// CHECK-NEXT:                             if [[T_115]] == 128 { 0 as u32 } else { [[T_115]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_116:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_117:__t[0-9]+]] = (![[T_116]]).trailing_zeros();
// CHECK-NEXT:                             if [[T_117]] == 128 { 0 as u32 } else { [[T_117]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_118:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_119:__t[0-9]+]] = [[T_118]].trailing_zeros();
// CHECK-NEXT:                             if [[T_119]] == 128 { 0 as u32 } else { [[T_119]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_120:__t[0-9]+]] = x;
// CHECK-NEXT:                         (128 - [[T_120]].count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_121:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_121]].count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_122:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_122]].count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_123:__t[0-9]+]] = x;
// CHECK-NEXT:                         (128 - [[T_123]].leading_zeros()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_124:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_124]].leading_zeros() == 128 {
// CHECK-NEXT:                             <bitint::BUint<128, 2, 16, bitint::A8>>::from_u128(0)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             <bitint::BUint<128, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                 .shl_bits((127 - [[T_124]].leading_zeros()) as usize)
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_125:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_125]].count_ones() == 0 {
// CHECK-NEXT:                             <bitint::BUint<128, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_125]].count_ones() == 1 {
// CHECK-NEXT:                                 [[T_125]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 <bitint::BUint<128, 2, 16, bitint::A8>>::from_u128(1)
// CHECK-NEXT:                                     .shl_bits((128 - [[T_125]].leading_zeros()) as usize)
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .to_u128() as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     '[[SLATE_BREAK_8:__slate_break_[0-9]+]]: while (i as u64) < 192 / 8 {
// CHECK-NEXT:         let s: u64 = unsafe { seeds.0[i as usize] };
// CHECK-NEXT:         '[[SLATE_BREAK_9:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: u8 = s as u8;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_0]] = x;
// CHECK-NEXT:                         [[T_0]].leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_1]] = x;
// CHECK-NEXT:                         (![[T_1]] as u8).leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_2]] = x;
// CHECK-NEXT:                         [[T_2]].trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_3]] = x;
// CHECK-NEXT:                         (![[T_3]] as u8).trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_4]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_5]] = (![[T_4]] as u8).leading_zeros();
// CHECK-NEXT:                             if [[T_5]] == 8 { 0 as u32 } else { [[T_5]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_6]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_7]] = [[T_6]].leading_zeros();
// CHECK-NEXT:                             if [[T_7]] == 8 { 0 as u32 } else { [[T_7]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_8]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_9]] = (![[T_8]] as u8).trailing_zeros();
// CHECK-NEXT:                             if [[T_9]] == 8 { 0 as u32 } else { [[T_9]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_10]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_11]] = [[T_10]].trailing_zeros();
// CHECK-NEXT:                             if [[T_11]] == 8 { 0 as u32 } else { [[T_11]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_12]] = x;
// CHECK-NEXT:                         (8 - [[T_12]].count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_13]] = x;
// CHECK-NEXT:                         [[T_13]].count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_14]] = x;
// CHECK-NEXT:                         [[T_14]].count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_15]] = x;
// CHECK-NEXT:                         (8 - [[T_15]].leading_zeros()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_16]] = x;
// CHECK-NEXT:                         if [[T_16]].leading_zeros() == 8 {
// CHECK-NEXT:                             0 as u8
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             (1 as u8)
// CHECK-NEXT:                                 .checked_shl(7 - [[T_16]].leading_zeros())
// CHECK-NEXT:                                 .unwrap_or(0 as u8) as u8
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_17]] = x;
// CHECK-NEXT:                         if [[T_17]].count_ones() == 0 {
// CHECK-NEXT:                             1 as u8
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_17]].count_ones() == 1 {
// CHECK-NEXT:                                 [[T_17]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 (1 as u8)
// CHECK-NEXT:                                     .checked_shl(8 - [[T_17]].leading_zeros())
// CHECK-NEXT:                                     .unwrap_or(0 as u8) as u8
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_10:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: u16 = s as u16;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_18]] = x;
// CHECK-NEXT:                         [[T_18]].leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_19]] = x;
// CHECK-NEXT:                         (![[T_19]] as u16).leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_20]] = x;
// CHECK-NEXT:                         [[T_20]].trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_21]] = x;
// CHECK-NEXT:                         (![[T_21]] as u16).trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_22]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_23]] = (![[T_22]] as u16).leading_zeros();
// CHECK-NEXT:                             if [[T_23]] == 16 { 0 as u32 } else { [[T_23]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_24]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_25]] = [[T_24]].leading_zeros();
// CHECK-NEXT:                             if [[T_25]] == 16 { 0 as u32 } else { [[T_25]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_26]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_27]] = (![[T_26]] as u16).trailing_zeros();
// CHECK-NEXT:                             if [[T_27]] == 16 { 0 as u32 } else { [[T_27]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_28]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_29]] = [[T_28]].trailing_zeros();
// CHECK-NEXT:                             if [[T_29]] == 16 { 0 as u32 } else { [[T_29]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_30]] = x;
// CHECK-NEXT:                         (16 - [[T_30]].count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_31]] = x;
// CHECK-NEXT:                         [[T_31]].count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_32]] = x;
// CHECK-NEXT:                         [[T_32]].count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_33]] = x;
// CHECK-NEXT:                         (16 - [[T_33]].leading_zeros()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_34]] = x;
// CHECK-NEXT:                         if [[T_34]].leading_zeros() == 16 {
// CHECK-NEXT:                             0 as u16
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             (1 as u16)
// CHECK-NEXT:                                 .checked_shl(15 - [[T_34]].leading_zeros())
// CHECK-NEXT:                                 .unwrap_or(0 as u16) as u16
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_35]] = x;
// CHECK-NEXT:                         if [[T_35]].count_ones() == 0 {
// CHECK-NEXT:                             1 as u16
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_35]].count_ones() == 1 {
// CHECK-NEXT:                                 [[T_35]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 (1 as u16)
// CHECK-NEXT:                                     .checked_shl(16 - [[T_35]].leading_zeros())
// CHECK-NEXT:                                     .unwrap_or(0 as u16) as u16
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_11:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: u32 = s as u32;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_36]] = x;
// CHECK-NEXT:                         [[T_36]].leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_37]] = x;
// CHECK-NEXT:                         (![[T_37]] as u32).leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_38]] = x;
// CHECK-NEXT:                         [[T_38]].trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_39]] = x;
// CHECK-NEXT:                         (![[T_39]] as u32).trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_40]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_41]] = (![[T_40]] as u32).leading_zeros();
// CHECK-NEXT:                             if [[T_41]] == 32 { 0 as u32 } else { [[T_41]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_42]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_43]] = [[T_42]].leading_zeros();
// CHECK-NEXT:                             if [[T_43]] == 32 { 0 as u32 } else { [[T_43]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_44]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_45]] = (![[T_44]] as u32).trailing_zeros();
// CHECK-NEXT:                             if [[T_45]] == 32 { 0 as u32 } else { [[T_45]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_46]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_47]] = [[T_46]].trailing_zeros();
// CHECK-NEXT:                             if [[T_47]] == 32 { 0 as u32 } else { [[T_47]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_48]] = x;
// CHECK-NEXT:                         (32 - [[T_48]].count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_49]] = x;
// CHECK-NEXT:                         [[T_49]].count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_50]] = x;
// CHECK-NEXT:                         [[T_50]].count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_51]] = x;
// CHECK-NEXT:                         (32 - [[T_51]].leading_zeros()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_52]] = x;
// CHECK-NEXT:                         if [[T_52]].leading_zeros() == 32 {
// CHECK-NEXT:                             0 as u32
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             (1 as u32)
// CHECK-NEXT:                                 .checked_shl(31 - [[T_52]].leading_zeros())
// CHECK-NEXT:                                 .unwrap_or(0 as u32) as u32
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_53]] = x;
// CHECK-NEXT:                         if [[T_53]].count_ones() == 0 {
// CHECK-NEXT:                             1 as u32
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_53]].count_ones() == 1 {
// CHECK-NEXT:                                 [[T_53]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 (1 as u32)
// CHECK-NEXT:                                     .checked_shl(32 - [[T_53]].leading_zeros())
// CHECK-NEXT:                                     .unwrap_or(0 as u32) as u32
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_12:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: u64 = s;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_54]] = x;
// CHECK-NEXT:                         [[T_54]].leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_55]] = x;
// CHECK-NEXT:                         (![[T_55]] as u64).leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_56]] = x;
// CHECK-NEXT:                         [[T_56]].trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_57]] = x;
// CHECK-NEXT:                         (![[T_57]] as u64).trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_58]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_59]] = (![[T_58]] as u64).leading_zeros();
// CHECK-NEXT:                             if [[T_59]] == 64 { 0 as u32 } else { [[T_59]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_60]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_61]] = [[T_60]].leading_zeros();
// CHECK-NEXT:                             if [[T_61]] == 64 { 0 as u32 } else { [[T_61]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_62]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_63]] = (![[T_62]] as u64).trailing_zeros();
// CHECK-NEXT:                             if [[T_63]] == 64 { 0 as u32 } else { [[T_63]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_64]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_65]] = [[T_64]].trailing_zeros();
// CHECK-NEXT:                             if [[T_65]] == 64 { 0 as u32 } else { [[T_65]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_66]] = x;
// CHECK-NEXT:                         (64 - [[T_66]].count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_67]] = x;
// CHECK-NEXT:                         [[T_67]].count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_68]] = x;
// CHECK-NEXT:                         [[T_68]].count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_69]] = x;
// CHECK-NEXT:                         (64 - [[T_69]].leading_zeros()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_70]] = x;
// CHECK-NEXT:                         if [[T_70]].leading_zeros() == 64 {
// CHECK-NEXT:                             0 as u64
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             (1 as u64)
// CHECK-NEXT:                                 .checked_shl(63 - [[T_70]].leading_zeros())
// CHECK-NEXT:                                 .unwrap_or(0 as u64) as u64
// CHECK-NEXT:                         }
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_71]] = x;
// CHECK-NEXT:                         if [[T_71]].count_ones() == 0 {
// CHECK-NEXT:                             1 as u64
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_71]].count_ones() == 1 {
// CHECK-NEXT:                                 [[T_71]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 (1 as u64)
// CHECK-NEXT:                                     .checked_shl(64 - [[T_71]].leading_zeros())
// CHECK-NEXT:                                     .unwrap_or(0 as u64) as u64
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     },
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_13:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: u8 = (s as u8) & 1;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_72]] = x;
// CHECK-NEXT:                         ((([[T_72]] as u8) & 1).leading_zeros() - 7) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_73]] = x;
// CHECK-NEXT:                         (((((![[T_73]] as u8) & 1) as u8) & 1).leading_zeros() - 7) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_74]] = x;
// CHECK-NEXT:                         (([[T_74]] as u8) & 1).trailing_zeros().min(1 as u32) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_75]] = x;
// CHECK-NEXT:                         ((((![[T_75]] as u8) & 1) as u8) & 1).trailing_zeros().min(1 as u32)
// CHECK-NEXT:                             as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_76]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_77]] = ((((![[T_76]] as u8) & 1) as u8) & 1)
// CHECK-NEXT:                                 .leading_zeros() - 7;
// CHECK-NEXT:                             if [[T_77]] == 1 { 0 as u32 } else { [[T_77]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_78]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_79]] = (([[T_78]] as u8) & 1).leading_zeros() - 7;
// CHECK-NEXT:                             if [[T_79]] == 1 { 0 as u32 } else { [[T_79]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_80]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_81]] = ((((![[T_80]] as u8) & 1) as u8) & 1)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(1 as u32);
// CHECK-NEXT:                             if [[T_81]] == 1 { 0 as u32 } else { [[T_81]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_82]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_83]] = (([[T_82]] as u8) & 1)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(1 as u32);
// CHECK-NEXT:                             if [[T_83]] == 1 { 0 as u32 } else { [[T_83]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_84]] = x;
// CHECK-NEXT:                         (1 - (([[T_84]] as u8) & 1).count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_85]] = x;
// CHECK-NEXT:                         (([[T_85]] as u8) & 1).count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_86]] = x;
// CHECK-NEXT:                         (([[T_86]] as u8) & 1).count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_87]] = x;
// CHECK-NEXT:                         (1 - ((([[T_87]] as u8) & 1).leading_zeros() - 7)) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_88]] = x;
// CHECK-NEXT:                         if (([[T_88]] as u8) & 1).leading_zeros() - 7 == 1 {
// CHECK-NEXT:                             0 as u8
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             ((1 as u8)
// CHECK-NEXT:                                 .checked_shl(0 - ((([[T_88]] as u8) & 1).leading_zeros() - 7))
// CHECK-NEXT:                                 .unwrap_or(0 as u8) as u8) & 1
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_89]] = x;
// CHECK-NEXT:                         if (([[T_89]] as u8) & 1).count_ones() == 0 {
// CHECK-NEXT:                             1 as u8
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if (([[T_89]] as u8) & 1).count_ones() == 1 {
// CHECK-NEXT:                                 [[T_89]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 ((1 as u8)
// CHECK-NEXT:                                     .checked_shl(1 - ((([[T_89]] as u8) & 1).leading_zeros() - 7))
// CHECK-NEXT:                                     .unwrap_or(0 as u8) as u8) & 1
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_14:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: u8 = (s as u8) & 127;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_90]] = x;
// CHECK-NEXT:                         ((([[T_90]] as u8) & 127).leading_zeros() - 1) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_91]] = x;
// CHECK-NEXT:                         (((((![[T_91]] as u8) & 127) as u8) & 127).leading_zeros() - 1)
// CHECK-NEXT:                             as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_92]] = x;
// CHECK-NEXT:                         (([[T_92]] as u8) & 127).trailing_zeros().min(7 as u32) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_93]] = x;
// CHECK-NEXT:                         ((((![[T_93]] as u8) & 127) as u8) & 127)
// CHECK-NEXT:                             .trailing_zeros()
// CHECK-NEXT:                             .min(7 as u32) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_94]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_95]] = ((((![[T_94]] as u8) & 127) as u8) & 127)
// CHECK-NEXT:                                 .leading_zeros() - 1;
// CHECK-NEXT:                             if [[T_95]] == 7 { 0 as u32 } else { [[T_95]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_96]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_97]] = (([[T_96]] as u8) & 127).leading_zeros() - 1;
// CHECK-NEXT:                             if [[T_97]] == 7 { 0 as u32 } else { [[T_97]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_98]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_99]] = ((((![[T_98]] as u8) & 127) as u8) & 127)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(7 as u32);
// CHECK-NEXT:                             if [[T_99]] == 7 { 0 as u32 } else { [[T_99]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_100]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_101]] = (([[T_100]] as u8) & 127)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(7 as u32);
// CHECK-NEXT:                             if [[T_101]] == 7 { 0 as u32 } else { [[T_101]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_102]] = x;
// CHECK-NEXT:                         (7 - (([[T_102]] as u8) & 127).count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_103]] = x;
// CHECK-NEXT:                         (([[T_103]] as u8) & 127).count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_104]] = x;
// CHECK-NEXT:                         (([[T_104]] as u8) & 127).count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_105]] = x;
// CHECK-NEXT:                         (7 - ((([[T_105]] as u8) & 127).leading_zeros() - 1)) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_106]] = x;
// CHECK-NEXT:                         if (([[T_106]] as u8) & 127).leading_zeros() - 1 == 7 {
// CHECK-NEXT:                             0 as u8
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             ((1 as u8)
// CHECK-NEXT:                                 .checked_shl(
// CHECK-NEXT:                                     6 - ((([[T_106]] as u8) & 127).leading_zeros() - 1),
// CHECK-NEXT:                                 )
// CHECK-NEXT:                                 .unwrap_or(0 as u8) as u8) & 127
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_107]] = x;
// CHECK-NEXT:                         if (([[T_107]] as u8) & 127).count_ones() == 0 {
// CHECK-NEXT:                             1 as u8
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if (([[T_107]] as u8) & 127).count_ones() == 1 {
// CHECK-NEXT:                                 [[T_107]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 ((1 as u8)
// CHECK-NEXT:                                     .checked_shl(
// CHECK-NEXT:                                         7 - ((([[T_107]] as u8) & 127).leading_zeros() - 1),
// CHECK-NEXT:                                     )
// CHECK-NEXT:                                     .unwrap_or(0 as u8) as u8) & 127
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_15:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: u16 = (s as u16) & 8191;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_108]] = x;
// CHECK-NEXT:                         ((([[T_108]] as u16) & 8191).leading_zeros() - 3) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_109]] = x;
// CHECK-NEXT:                         (((((![[T_109]] as u16) & 8191) as u16) & 8191).leading_zeros() - 3)
// CHECK-NEXT:                             as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_110]] = x;
// CHECK-NEXT:                         (([[T_110]] as u16) & 8191).trailing_zeros().min(13 as u32) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_111]] = x;
// CHECK-NEXT:                         ((((![[T_111]] as u16) & 8191) as u16) & 8191)
// CHECK-NEXT:                             .trailing_zeros()
// CHECK-NEXT:                             .min(13 as u32) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_112]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_113]] = ((((![[T_112]] as u16) & 8191) as u16) & 8191)
// CHECK-NEXT:                                 .leading_zeros() - 3;
// CHECK-NEXT:                             if [[T_113]] == 13 { 0 as u32 } else { [[T_113]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_114]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_115]] = (([[T_114]] as u16) & 8191).leading_zeros() - 3;
// CHECK-NEXT:                             if [[T_115]] == 13 { 0 as u32 } else { [[T_115]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_116]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_117]] = ((((![[T_116]] as u16) & 8191) as u16) & 8191)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(13 as u32);
// CHECK-NEXT:                             if [[T_117]] == 13 { 0 as u32 } else { [[T_117]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_118]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_119]] = (([[T_118]] as u16) & 8191)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(13 as u32);
// CHECK-NEXT:                             if [[T_119]] == 13 { 0 as u32 } else { [[T_119]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_120]] = x;
// CHECK-NEXT:                         (13 - (([[T_120]] as u16) & 8191).count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_121]] = x;
// CHECK-NEXT:                         (([[T_121]] as u16) & 8191).count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_122]] = x;
// CHECK-NEXT:                         (([[T_122]] as u16) & 8191).count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_123]] = x;
// CHECK-NEXT:                         (13 - ((([[T_123]] as u16) & 8191).leading_zeros() - 3)) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_124]] = x;
// CHECK-NEXT:                         if (([[T_124]] as u16) & 8191).leading_zeros() - 3 == 13 {
// CHECK-NEXT:                             0 as u16
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             ((1 as u16)
// CHECK-NEXT:                                 .checked_shl(
// CHECK-NEXT:                                     12 - ((([[T_124]] as u16) & 8191).leading_zeros() - 3),
// CHECK-NEXT:                                 )
// CHECK-NEXT:                                 .unwrap_or(0 as u16) as u16) & 8191
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_125]] = x;
// CHECK-NEXT:                         if (([[T_125]] as u16) & 8191).count_ones() == 0 {
// CHECK-NEXT:                             1 as u16
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if (([[T_125]] as u16) & 8191).count_ones() == 1 {
// CHECK-NEXT:                                 [[T_125]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 ((1 as u16)
// CHECK-NEXT:                                     .checked_shl(
// CHECK-NEXT:                                         13 - ((([[T_125]] as u16) & 8191).leading_zeros() - 3),
// CHECK-NEXT:                                     )
// CHECK-NEXT:                                     .unwrap_or(0 as u16) as u16) & 8191
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_16:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: u64 = (s as u64) & 137438953471;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_126:__t[0-9]+]] = x;
// CHECK-NEXT:                         ((([[T_126]] as u64) & 137438953471).leading_zeros() - 27) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_127:__t[0-9]+]] = x;
// CHECK-NEXT:                         (((((![[T_127]] as u64) & 137438953471) as u64) & 137438953471)
// CHECK-NEXT:                             .leading_zeros() - 27) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_128:__t[0-9]+]] = x;
// CHECK-NEXT:                         (([[T_128]] as u64) & 137438953471).trailing_zeros().min(37 as u32)
// CHECK-NEXT:                             as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_129:__t[0-9]+]] = x;
// CHECK-NEXT:                         ((((![[T_129]] as u64) & 137438953471) as u64) & 137438953471)
// CHECK-NEXT:                             .trailing_zeros()
// CHECK-NEXT:                             .min(37 as u32) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_130:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_131:__t[0-9]+]] = ((((![[T_130]] as u64) & 137438953471) as u64)
// CHECK-NEXT:                                 & 137438953471)
// CHECK-NEXT:                                 .leading_zeros() - 27;
// CHECK-NEXT:                             if [[T_131]] == 37 { 0 as u32 } else { [[T_131]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_132:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_133:__t[0-9]+]] = (([[T_132]] as u64) & 137438953471).leading_zeros()
// CHECK-NEXT:                                 - 27;
// CHECK-NEXT:                             if [[T_133]] == 37 { 0 as u32 } else { [[T_133]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_134:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_135:__t[0-9]+]] = ((((![[T_134]] as u64) & 137438953471) as u64)
// CHECK-NEXT:                                 & 137438953471)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(37 as u32);
// CHECK-NEXT:                             if [[T_135]] == 37 { 0 as u32 } else { [[T_135]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_136:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_137:__t[0-9]+]] = (([[T_136]] as u64) & 137438953471)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(37 as u32);
// CHECK-NEXT:                             if [[T_137]] == 37 { 0 as u32 } else { [[T_137]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_138:__t[0-9]+]] = x;
// CHECK-NEXT:                         (37 - (([[T_138]] as u64) & 137438953471).count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_139:__t[0-9]+]] = x;
// CHECK-NEXT:                         (([[T_139]] as u64) & 137438953471).count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_140:__t[0-9]+]] = x;
// CHECK-NEXT:                         (([[T_140]] as u64) & 137438953471).count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_141:__t[0-9]+]] = x;
// CHECK-NEXT:                         (37 - ((([[T_141]] as u64) & 137438953471).leading_zeros() - 27))
// CHECK-NEXT:                             as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_142:__t[0-9]+]] = x;
// CHECK-NEXT:                         if (([[T_142]] as u64) & 137438953471).leading_zeros() - 27 == 37 {
// CHECK-NEXT:                             0 as u64
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             ((1 as u64)
// CHECK-NEXT:                                 .checked_shl(
// CHECK-NEXT:                                     36 - ((([[T_142]] as u64) & 137438953471).leading_zeros() - 27),
// CHECK-NEXT:                                 )
// CHECK-NEXT:                                 .unwrap_or(0 as u64) as u64) & 137438953471
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_143:__t[0-9]+]] = x;
// CHECK-NEXT:                         if (([[T_143]] as u64) & 137438953471).count_ones() == 0 {
// CHECK-NEXT:                             1 as u64
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if (([[T_143]] as u64) & 137438953471).count_ones() == 1 {
// CHECK-NEXT:                                 [[T_143]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 ((1 as u64)
// CHECK-NEXT:                                     .checked_shl(
// CHECK-NEXT:                                         37 - ((([[T_143]] as u64) & 137438953471).leading_zeros() - 27),
// CHECK-NEXT:                                     )
// CHECK-NEXT:                                     .unwrap_or(0 as u64) as u64) & 137438953471
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_17:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: u64 = s as u64;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_144:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_144]].leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_145:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_145]] as u64).leading_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_146:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_146]].trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_147:__t[0-9]+]] = x;
// CHECK-NEXT:                         (![[T_147]] as u64).trailing_zeros() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_148:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_149:__t[0-9]+]] = (![[T_148]] as u64).leading_zeros();
// CHECK-NEXT:                             if [[T_149]] == 64 { 0 as u32 } else { [[T_149]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_150:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_151:__t[0-9]+]] = [[T_150]].leading_zeros();
// CHECK-NEXT:                             if [[T_151]] == 64 { 0 as u32 } else { [[T_151]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_152:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_153:__t[0-9]+]] = (![[T_152]] as u64).trailing_zeros();
// CHECK-NEXT:                             if [[T_153]] == 64 { 0 as u32 } else { [[T_153]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_154:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_155:__t[0-9]+]] = [[T_154]].trailing_zeros();
// CHECK-NEXT:                             if [[T_155]] == 64 { 0 as u32 } else { [[T_155]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_156:__t[0-9]+]] = x;
// CHECK-NEXT:                         (64 - [[T_156]].count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_157:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_157]].count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_158:__t[0-9]+]] = x;
// CHECK-NEXT:                         [[T_158]].count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_159:__t[0-9]+]] = x;
// CHECK-NEXT:                         (64 - [[T_159]].leading_zeros()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_160:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_160]].leading_zeros() == 64 {
// CHECK-NEXT:                             0 as u64
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             (1 as u64)
// CHECK-NEXT:                                 .checked_shl(63 - [[T_160]].leading_zeros())
// CHECK-NEXT:                                 .unwrap_or(0 as u64) as u64
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_161:__t[0-9]+]] = x;
// CHECK-NEXT:                         if [[T_161]].count_ones() == 0 {
// CHECK-NEXT:                             1 as u64
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if [[T_161]].count_ones() == 1 {
// CHECK-NEXT:                                 [[T_161]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 (1 as u64)
// CHECK-NEXT:                                     .checked_shl(64 - [[T_161]].leading_zeros())
// CHECK-NEXT:                                     .unwrap_or(0 as u64) as u64
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_18:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: u8 = (!((s as u8) & 127) as u8) & 127;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_162:__t[0-9]+]] = x;
// CHECK-NEXT:                         ((([[T_162]] as u8) & 127).leading_zeros() - 1) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_163:__t[0-9]+]] = x;
// CHECK-NEXT:                         (((((![[T_163]] as u8) & 127) as u8) & 127).leading_zeros() - 1)
// CHECK-NEXT:                             as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_164:__t[0-9]+]] = x;
// CHECK-NEXT:                         (([[T_164]] as u8) & 127).trailing_zeros().min(7 as u32) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_165:__t[0-9]+]] = x;
// CHECK-NEXT:                         ((((![[T_165]] as u8) & 127) as u8) & 127)
// CHECK-NEXT:                             .trailing_zeros()
// CHECK-NEXT:                             .min(7 as u32) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_166:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_167:__t[0-9]+]] = ((((![[T_166]] as u8) & 127) as u8) & 127)
// CHECK-NEXT:                                 .leading_zeros() - 1;
// CHECK-NEXT:                             if [[T_167]] == 7 { 0 as u32 } else { [[T_167]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_168:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_169:__t[0-9]+]] = (([[T_168]] as u8) & 127).leading_zeros() - 1;
// CHECK-NEXT:                             if [[T_169]] == 7 { 0 as u32 } else { [[T_169]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_170:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_171:__t[0-9]+]] = ((((![[T_170]] as u8) & 127) as u8) & 127)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(7 as u32);
// CHECK-NEXT:                             if [[T_171]] == 7 { 0 as u32 } else { [[T_171]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_172:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_173:__t[0-9]+]] = (([[T_172]] as u8) & 127)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(7 as u32);
// CHECK-NEXT:                             if [[T_173]] == 7 { 0 as u32 } else { [[T_173]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_174:__t[0-9]+]] = x;
// CHECK-NEXT:                         (7 - (([[T_174]] as u8) & 127).count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_175:__t[0-9]+]] = x;
// CHECK-NEXT:                         (([[T_175]] as u8) & 127).count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_176:__t[0-9]+]] = x;
// CHECK-NEXT:                         (([[T_176]] as u8) & 127).count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_177:__t[0-9]+]] = x;
// CHECK-NEXT:                         (7 - ((([[T_177]] as u8) & 127).leading_zeros() - 1)) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_178:__t[0-9]+]] = x;
// CHECK-NEXT:                         if (([[T_178]] as u8) & 127).leading_zeros() - 1 == 7 {
// CHECK-NEXT:                             0 as u8
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             ((1 as u8)
// CHECK-NEXT:                                 .checked_shl(
// CHECK-NEXT:                                     6 - ((([[T_178]] as u8) & 127).leading_zeros() - 1),
// CHECK-NEXT:                                 )
// CHECK-NEXT:                                 .unwrap_or(0 as u8) as u8) & 127
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_179:__t[0-9]+]] = x;
// CHECK-NEXT:                         if (([[T_179]] as u8) & 127).count_ones() == 0 {
// CHECK-NEXT:                             1 as u8
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if (([[T_179]] as u8) & 127).count_ones() == 1 {
// CHECK-NEXT:                                 [[T_179]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 ((1 as u8)
// CHECK-NEXT:                                     .checked_shl(
// CHECK-NEXT:                                         7 - ((([[T_179]] as u8) & 127).leading_zeros() - 1),
// CHECK-NEXT:                                     )
// CHECK-NEXT:                                     .unwrap_or(0 as u8) as u8) & 127
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_19:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: u16 = (!((s as u16) & 8191) as u16) & 8191;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_180:__t[0-9]+]] = x;
// CHECK-NEXT:                         ((([[T_180]] as u16) & 8191).leading_zeros() - 3) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_181:__t[0-9]+]] = x;
// CHECK-NEXT:                         (((((![[T_181]] as u16) & 8191) as u16) & 8191).leading_zeros() - 3)
// CHECK-NEXT:                             as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_182:__t[0-9]+]] = x;
// CHECK-NEXT:                         (([[T_182]] as u16) & 8191).trailing_zeros().min(13 as u32) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_183:__t[0-9]+]] = x;
// CHECK-NEXT:                         ((((![[T_183]] as u16) & 8191) as u16) & 8191)
// CHECK-NEXT:                             .trailing_zeros()
// CHECK-NEXT:                             .min(13 as u32) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_184:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_185:__t[0-9]+]] = ((((![[T_184]] as u16) & 8191) as u16) & 8191)
// CHECK-NEXT:                                 .leading_zeros() - 3;
// CHECK-NEXT:                             if [[T_185]] == 13 { 0 as u32 } else { [[T_185]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_186:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_187:__t[0-9]+]] = (([[T_186]] as u16) & 8191).leading_zeros() - 3;
// CHECK-NEXT:                             if [[T_187]] == 13 { 0 as u32 } else { [[T_187]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_188:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_189:__t[0-9]+]] = ((((![[T_188]] as u16) & 8191) as u16) & 8191)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(13 as u32);
// CHECK-NEXT:                             if [[T_189]] == 13 { 0 as u32 } else { [[T_189]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_190:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_191:__t[0-9]+]] = (([[T_190]] as u16) & 8191)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(13 as u32);
// CHECK-NEXT:                             if [[T_191]] == 13 { 0 as u32 } else { [[T_191]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_192:__t[0-9]+]] = x;
// CHECK-NEXT:                         (13 - (([[T_192]] as u16) & 8191).count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_193:__t[0-9]+]] = x;
// CHECK-NEXT:                         (([[T_193]] as u16) & 8191).count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_194:__t[0-9]+]] = x;
// CHECK-NEXT:                         (([[T_194]] as u16) & 8191).count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_195:__t[0-9]+]] = x;
// CHECK-NEXT:                         (13 - ((([[T_195]] as u16) & 8191).leading_zeros() - 3)) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_196:__t[0-9]+]] = x;
// CHECK-NEXT:                         if (([[T_196]] as u16) & 8191).leading_zeros() - 3 == 13 {
// CHECK-NEXT:                             0 as u16
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             ((1 as u16)
// CHECK-NEXT:                                 .checked_shl(
// CHECK-NEXT:                                     12 - ((([[T_196]] as u16) & 8191).leading_zeros() - 3),
// CHECK-NEXT:                                 )
// CHECK-NEXT:                                 .unwrap_or(0 as u16) as u16) & 8191
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_197:__t[0-9]+]] = x;
// CHECK-NEXT:                         if (([[T_197]] as u16) & 8191).count_ones() == 0 {
// CHECK-NEXT:                             1 as u16
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if (([[T_197]] as u16) & 8191).count_ones() == 1 {
// CHECK-NEXT:                                 [[T_197]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 ((1 as u16)
// CHECK-NEXT:                                     .checked_shl(
// CHECK-NEXT:                                         13 - ((([[T_197]] as u16) & 8191).leading_zeros() - 3),
// CHECK-NEXT:                                     )
// CHECK-NEXT:                                     .unwrap_or(0 as u16) as u16) & 8191
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         '[[SLATE_BREAK_20:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:             let x: u64 = (!((s as u64) & 137438953471) as u64) & 137438953471;
// CHECK-NEXT:             let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 printf(
// CHECK-NEXT:                     c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n".as_ptr(),
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_198:__t[0-9]+]] = x;
// CHECK-NEXT:                         ((([[T_198]] as u64) & 137438953471).leading_zeros() - 27) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_199:__t[0-9]+]] = x;
// CHECK-NEXT:                         (((((![[T_199]] as u64) & 137438953471) as u64) & 137438953471)
// CHECK-NEXT:                             .leading_zeros() - 27) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_200:__t[0-9]+]] = x;
// CHECK-NEXT:                         (([[T_200]] as u64) & 137438953471).trailing_zeros().min(37 as u32)
// CHECK-NEXT:                             as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_201:__t[0-9]+]] = x;
// CHECK-NEXT:                         ((((![[T_201]] as u64) & 137438953471) as u64) & 137438953471)
// CHECK-NEXT:                             .trailing_zeros()
// CHECK-NEXT:                             .min(37 as u32) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_202:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_203:__t[0-9]+]] = ((((![[T_202]] as u64) & 137438953471) as u64)
// CHECK-NEXT:                                 & 137438953471)
// CHECK-NEXT:                                 .leading_zeros() - 27;
// CHECK-NEXT:                             if [[T_203]] == 37 { 0 as u32 } else { [[T_203]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_204:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_205:__t[0-9]+]] = (([[T_204]] as u64) & 137438953471).leading_zeros()
// CHECK-NEXT:                                 - 27;
// CHECK-NEXT:                             if [[T_205]] == 37 { 0 as u32 } else { [[T_205]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_206:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_207:__t[0-9]+]] = ((((![[T_206]] as u64) & 137438953471) as u64)
// CHECK-NEXT:                                 & 137438953471)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(37 as u32);
// CHECK-NEXT:                             if [[T_207]] == 37 { 0 as u32 } else { [[T_207]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_208:__t[0-9]+]] = x;
// CHECK-NEXT:                         ({
// CHECK-NEXT:                             let [[T_209:__t[0-9]+]] = (([[T_208]] as u64) & 137438953471)
// CHECK-NEXT:                                 .trailing_zeros()
// CHECK-NEXT:                                 .min(37 as u32);
// CHECK-NEXT:                             if [[T_209]] == 37 { 0 as u32 } else { [[T_209]] + 1 }
// CHECK-NEXT:                         }) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_210:__t[0-9]+]] = x;
// CHECK-NEXT:                         (37 - (([[T_210]] as u64) & 137438953471).count_ones()) as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_211:__t[0-9]+]] = x;
// CHECK-NEXT:                         (([[T_211]] as u64) & 137438953471).count_ones() as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_212:__t[0-9]+]] = x;
// CHECK-NEXT:                         (([[T_212]] as u64) & 137438953471).count_ones() == 1
// CHECK-NEXT:                     }) as u32,
// CHECK-NEXT:                     {
// CHECK-NEXT:                         let [[T_213:__t[0-9]+]] = x;
// CHECK-NEXT:                         (37 - ((([[T_213]] as u64) & 137438953471).leading_zeros() - 27))
// CHECK-NEXT:                             as u32
// CHECK-NEXT:                     },
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_214:__t[0-9]+]] = x;
// CHECK-NEXT:                         if (([[T_214]] as u64) & 137438953471).leading_zeros() - 27 == 37 {
// CHECK-NEXT:                             0 as u64
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             ((1 as u64)
// CHECK-NEXT:                                 .checked_shl(
// CHECK-NEXT:                                     36 - ((([[T_214]] as u64) & 137438953471).leading_zeros() - 27),
// CHECK-NEXT:                                 )
// CHECK-NEXT:                                 .unwrap_or(0 as u64) as u64) & 137438953471
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                     ({
// CHECK-NEXT:                         let [[T_215:__t[0-9]+]] = x;
// CHECK-NEXT:                         if (([[T_215]] as u64) & 137438953471).count_ones() == 0 {
// CHECK-NEXT:                             1 as u64
// CHECK-NEXT:                         } else {
// CHECK-NEXT:                             if (([[T_215]] as u64) & 137438953471).count_ones() == 1 {
// CHECK-NEXT:                                 [[T_215]]
// CHECK-NEXT:                             } else {
// CHECK-NEXT:                                 ((1 as u64)
// CHECK-NEXT:                                     .checked_shl(
// CHECK-NEXT:                                         37 - ((([[T_215]] as u64) & 137438953471).leading_zeros() - 27),
// CHECK-NEXT:                                     )
// CHECK-NEXT:                                     .unwrap_or(0 as u64) as u64) & 137438953471
// CHECK-NEXT:                             }
// CHECK-NEXT:                         }
// CHECK-NEXT:                     }) as u64,
// CHECK-NEXT:                 )
// CHECK-NEXT:             };
// CHECK-NEXT:             unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:             if !((0 as i32) != (0 as i32)) {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT:     wide();
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
