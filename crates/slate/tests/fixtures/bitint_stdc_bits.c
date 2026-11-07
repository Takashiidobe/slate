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
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: macro_rules! REPORT__4 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = $X; let _ = std::io::Write::flush(& mut
// CHECK-NEXT:         std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; __t.leading_zeros() as u32 }, { let [[T_0:__t[0-9]+]] = x; (! [[T_0]]
// CHECK-NEXT:         as $T).leading_zeros() as u32 }, { let [[T_1:__t[0-9]+]] = x; [[T_1]].trailing_zeros() as u32 },
// CHECK-NEXT:         { let [[T_2:__t[0-9]+]] = x; (! [[T_2]] as $T).trailing_zeros() as u32 }, { let [[T_3:__t[0-9]+]] = x; ({ let
// CHECK-NEXT:         [[T_4:__t[0-9]+]] = (! [[T_3]] as $T).leading_zeros(); if [[T_4]] == 64 { 0u32 } else { [[T_4]] + 1 }
// CHECK-NEXT:         }) as u32 }, { let [[T_5:__t[0-9]+]] = x; ({ let [[T_6:__t[0-9]+]] = [[T_5]].leading_zeros(); if [[T_6]] == 64 {
// CHECK-NEXT:         0u32 } else { [[T_6]] + 1 } }) as u32 }, { let [[T_7:__t[0-9]+]] = x; ({ let [[T_8:__t[0-9]+]] = (! [[T_7]] as
// CHECK-NEXT:         $T).trailing_zeros(); if [[T_8]] == 64 { 0u32 } else { [[T_8]] + 1 } }) as u32 }, {
// CHECK-NEXT:         let [[T_9:__t[0-9]+]] = x; ({ let [[T_10:__t[0-9]+]] = [[T_9]].trailing_zeros(); if [[T_10]] == 64 { 0u32 }
// CHECK-NEXT:         else { [[T_10]] + 1 } }) as u32 }, { let [[T_11:__t[0-9]+]] = x; (64 - [[T_11]].count_ones()) as u32
// CHECK-NEXT:         }, { let [[T_12:__t[0-9]+]] = x; [[T_12]].count_ones() as u32 }, ({ let [[T_13:__t[0-9]+]] = x; [[T_13]]
// CHECK-NEXT:         .count_ones() == 1 }) as u32, { let [[T_14:__t[0-9]+]] = x; (64 - [[T_14]].leading_zeros()) as
// CHECK-NEXT:         u32 }, { let [[T_15:__t[0-9]+]] = x; if [[T_15]].leading_zeros() == 64 { 0u64 } else { 1u64
// CHECK-NEXT:         .checked_shl(63 - [[T_15]].leading_zeros()).unwrap_or(0u64) as $T } }, { let [[T_16:__t[0-9]+]] =
// CHECK-NEXT:         x; if [[T_16]].count_ones() == 0 { 1u64 } else { if [[T_16]].count_ones() == 1 { [[T_16]]
// CHECK-NEXT:         } else { 1u64.checked_shl(64 - [[T_16]].leading_zeros()).unwrap_or(0u64) as $T } }
// CHECK-NEXT:         }) }; unsafe { fflush(std::ptr::null_mut()) }; if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT__5 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = (($X) as $T) & 1; let _ =
// CHECK-NEXT:         std::io::Write::flush(& mut std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; (((__t as $T) & 1).leading_zeros() - 7) as u32 }, { let
// CHECK-NEXT:         [[T_0]] = x; (((((! [[T_0]] as $T) & 1) as $T) & 1).leading_zeros() - 7) as u32 }, {
// CHECK-NEXT:         let [[T_1]] = x; (([[T_1]] as $T) & 1).trailing_zeros().min(1u32) as u32 }, { let [[T_2]]
// CHECK-NEXT:         = x; ((((! [[T_2]] as $T) & 1) as $T) & 1).trailing_zeros().min(1u32) as u32 }, {
// CHECK-NEXT:         let [[T_3]] = x; ({ let [[T_4]] = ((((! [[T_3]] as $T) & 1) as $T) & 1).leading_zeros() -
// CHECK-NEXT:         7; if [[T_4]] == 1 { 0u32 } else { [[T_4]] + 1 } }) as u32 }, { let [[T_5]] = x; ({ let
// CHECK-NEXT:         [[T_6]] = (([[T_5]] as $T) & 1).leading_zeros() - 7; if [[T_6]] == 1 { 0u32 } else { [[T_6]]
// CHECK-NEXT:         + 1 } }) as u32 }, { let [[T_7]] = x; ({ let [[T_8]] = ((((! [[T_7]] as $T) & 1) as $T) &
// CHECK-NEXT:         1).trailing_zeros().min(1u32); if [[T_8]] == 1 { 0u32 } else { [[T_8]] + 1 } }) as
// CHECK-NEXT:         u32 }, { let [[T_9]] = x; ({ let [[T_10]] = (([[T_9]] as $T) & 1).trailing_zeros()
// CHECK-NEXT:         .min(1u32); if [[T_10]] == 1 { 0u32 } else { [[T_10]] + 1 } }) as u32 }, { let [[T_11]] =
// CHECK-NEXT:         x; (1 - (([[T_11]] as $T) & 1).count_ones()) as u32 }, { let [[T_12]] = x; (([[T_12]] as
// CHECK-NEXT:         $T) & 1).count_ones() as u32 }, ({ let [[T_13]] = x; (([[T_13]] as $T) & 1)
// CHECK-NEXT:         .count_ones() == 1 }) as u32, { let [[T_14]] = x; (1 - ((([[T_14]] as $T) & 1)
// CHECK-NEXT:         .leading_zeros() - 7)) as u32 }, ({ let [[T_15]] = x; if (([[T_15]] as $T) & 1)
// CHECK-NEXT:         .leading_zeros() - 7 == 1 { 0u8 } else { (1u8.checked_shl(0 - ((([[T_15]] as $T) &
// CHECK-NEXT:         1).leading_zeros() - 7)).unwrap_or(0u8) as $T) & 1 } }) as u64, ({ let [[T_16]] = x;
// CHECK-NEXT:         if (([[T_16]] as $T) & 1).count_ones() == 0 { 1u8 } else { if (([[T_16]] as $T) & 1)
// CHECK-NEXT:         .count_ones() == 1 { [[T_16]] } else { (1u8.checked_shl(1 - ((([[T_16]] as $T) & 1)
// CHECK-NEXT:         .leading_zeros() - 7)).unwrap_or(0u8) as $T) & 1 } } }) as u64) }; unsafe {
// CHECK-NEXT:         fflush(std::ptr::null_mut()) }; if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT__2 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = $X; let _ = std::io::Write::flush(& mut
// CHECK-NEXT:         std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; __t.leading_zeros() as u32 }, { let [[T_0]] = x; (! [[T_0]])
// CHECK-NEXT:         .leading_zeros() as u32 }, { let [[T_1]] = x; [[T_1]].trailing_zeros() as u32 }, { let
// CHECK-NEXT:         [[T_2]] = x; (! [[T_2]]).trailing_zeros() as u32 }, { let [[T_3]] = x; ({ let [[T_4]] = (!
// CHECK-NEXT:         [[T_3]]).leading_zeros(); if [[T_4]] == 128 { 0u32 } else { [[T_4]] + 1 } }) as u32 }, {
// CHECK-NEXT:         let [[T_5]] = x; ({ let [[T_6]] = [[T_5]].leading_zeros(); if [[T_6]] == 128 { 0u32 } else {
// CHECK-NEXT:         [[T_6]] + 1 } }) as u32 }, { let [[T_7]] = x; ({ let [[T_8]] = (! [[T_7]]).trailing_zeros();
// CHECK-NEXT:         if [[T_8]] == 128 { 0u32 } else { [[T_8]] + 1 } }) as u32 }, { let [[T_9]] = x; ({ let
// CHECK-NEXT:         [[T_10]] = [[T_9]].trailing_zeros(); if [[T_10]] == 128 { 0u32 } else { [[T_10]] + 1 } }) as
// CHECK-NEXT:         u32 }, { let [[T_11]] = x; (128 - [[T_11]].count_ones()) as u32 }, { let [[T_12]] = x;
// CHECK-NEXT:         [[T_12]].count_ones() as u32 }, ({ let [[T_13]] = x; [[T_13]].count_ones() == 1 }) as u32,
// CHECK-NEXT:         { let [[T_14]] = x; (128 - [[T_14]].leading_zeros()) as u32 }, { let [[T_15]] = x; if
// CHECK-NEXT:         [[T_15]].leading_zeros() == 128 { < bitint::BUint < 128, 2, 16, bitint::A8
// CHECK-NEXT:         >>::from_u128(0) } else { < bitint::BUint < 128, 2, 16, bitint::A8
// CHECK-NEXT:         >>::from_u128(1).shl_bits((127 - [[T_15]].leading_zeros()) as usize) } } .to_u128()
// CHECK-NEXT:         as u64, { let [[T_16]] = x; if [[T_16]].count_ones() == 0 { < bitint::BUint < 128, 2,
// CHECK-NEXT:         16, bitint::A8 >>::from_u128(1) } else { if [[T_16]].count_ones() == 1 { [[T_16]] }
// CHECK-NEXT:         else { < bitint::BUint < 128, 2, 16, bitint::A8 >>::from_u128(1).shl_bits((128 -
// CHECK-NEXT:         [[T_16]].leading_zeros()) as usize) } } } .to_u128() as u64) }; unsafe {
// CHECK-NEXT:         fflush(std::ptr::null_mut()) }; if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT__6 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = ($X) as $T; let _ =
// CHECK-NEXT:         std::io::Write::flush(& mut std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; __t.leading_zeros() as u32 }, { let [[T_0]] = x; (! [[T_0]]
// CHECK-NEXT:         as $T).leading_zeros() as u32 }, { let [[T_1]] = x; [[T_1]].trailing_zeros() as u32 },
// CHECK-NEXT:         { let [[T_2]] = x; (! [[T_2]] as $T).trailing_zeros() as u32 }, { let [[T_3]] = x; ({ let
// CHECK-NEXT:         [[T_4]] = (! [[T_3]] as $T).leading_zeros(); if [[T_4]] == 64 { 0u32 } else { [[T_4]] + 1 }
// CHECK-NEXT:         }) as u32 }, { let [[T_5]] = x; ({ let [[T_6]] = [[T_5]].leading_zeros(); if [[T_6]] == 64 {
// CHECK-NEXT:         0u32 } else { [[T_6]] + 1 } }) as u32 }, { let [[T_7]] = x; ({ let [[T_8]] = (! [[T_7]] as
// CHECK-NEXT:         $T).trailing_zeros(); if [[T_8]] == 64 { 0u32 } else { [[T_8]] + 1 } }) as u32 }, {
// CHECK-NEXT:         let [[T_9]] = x; ({ let [[T_10]] = [[T_9]].trailing_zeros(); if [[T_10]] == 64 { 0u32 }
// CHECK-NEXT:         else { [[T_10]] + 1 } }) as u32 }, { let [[T_11]] = x; (64 - [[T_11]].count_ones()) as u32
// CHECK-NEXT:         }, { let [[T_12]] = x; [[T_12]].count_ones() as u32 }, ({ let [[T_13]] = x; [[T_13]]
// CHECK-NEXT:         .count_ones() == 1 }) as u32, { let [[T_14]] = x; (64 - [[T_14]].leading_zeros()) as
// CHECK-NEXT:         u32 }, ({ let [[T_15]] = x; if [[T_15]].leading_zeros() == 64 { 0u64 } else { 1u64
// CHECK-NEXT:         .checked_shl(63 - [[T_15]].leading_zeros()).unwrap_or(0u64) as $T } }) as $T, ({ let
// CHECK-NEXT:         [[T_16]] = x; if [[T_16]].count_ones() == 0 { 1u64 } else { if [[T_16]].count_ones() == 1
// CHECK-NEXT:         { [[T_16]] } else { 1u64.checked_shl(64 - [[T_16]].leading_zeros()).unwrap_or(0u64) as
// CHECK-NEXT:         $T } } }) as $T) }; unsafe { fflush(std::ptr::null_mut()) }; if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT__7 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = (($X) as $T) & 8191; let _ =
// CHECK-NEXT:         std::io::Write::flush(& mut std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; (((__t as $T) & 8191).leading_zeros() - 3) as u32 }, {
// CHECK-NEXT:         let [[T_0]] = x; (((((! [[T_0]] as $T) & 8191) as $T) & 8191).leading_zeros() - 3) as
// CHECK-NEXT:         u32 }, { let [[T_1]] = x; (([[T_1]] as $T) & 8191).trailing_zeros().min(13u32) as u32
// CHECK-NEXT:         }, { let [[T_2]] = x; ((((! [[T_2]] as $T) & 8191) as $T) & 8191).trailing_zeros()
// CHECK-NEXT:         .min(13u32) as u32 }, { let [[T_3]] = x; ({ let [[T_4]] = ((((! [[T_3]] as $T) & 8191) as
// CHECK-NEXT:         $T) & 8191).leading_zeros() - 3; if [[T_4]] == 13 { 0u32 } else { [[T_4]] + 1 } }) as
// CHECK-NEXT:         u32 }, { let [[T_5]] = x; ({ let [[T_6]] = (([[T_5]] as $T) & 8191).leading_zeros() - 3;
// CHECK-NEXT:         if [[T_6]] == 13 { 0u32 } else { [[T_6]] + 1 } }) as u32 }, { let [[T_7]] = x; ({ let
// CHECK-NEXT:         [[T_8]] = ((((! [[T_7]] as $T) & 8191) as $T) & 8191).trailing_zeros().min(13u32); if
// CHECK-NEXT:         [[T_8]] == 13 { 0u32 } else { [[T_8]] + 1 } }) as u32 }, { let [[T_9]] = x; ({ let
// CHECK-NEXT:         [[T_10]] = (([[T_9]] as $T) & 8191).trailing_zeros().min(13u32); if [[T_10]] == 13 { 0u32
// CHECK-NEXT:         } else { [[T_10]] + 1 } }) as u32 }, { let [[T_11]] = x; (13 - (([[T_11]] as $T) & 8191)
// CHECK-NEXT:         .count_ones()) as u32 }, { let [[T_12]] = x; (([[T_12]] as $T) & 8191).count_ones() as
// CHECK-NEXT:         u32 }, ({ let [[T_13]] = x; (([[T_13]] as $T) & 8191).count_ones() == 1 }) as u32, {
// CHECK-NEXT:         let [[T_14]] = x; (13 - ((([[T_14]] as $T) & 8191).leading_zeros() - 3)) as u32 }, ({
// CHECK-NEXT:         let [[T_15]] = x; if (([[T_15]] as $T) & 8191).leading_zeros() - 3 == 13 { 0u16 } else
// CHECK-NEXT:         { (1u16.checked_shl(12 - ((([[T_15]] as $T) & 8191).leading_zeros() - 3))
// CHECK-NEXT:         .unwrap_or(0u16) as $T) & 8191 } }) as u64, ({ let [[T_16]] = x; if (([[T_16]] as $T) &
// CHECK-NEXT:         8191).count_ones() == 0 { 1u16 } else { if (([[T_16]] as $T) & 8191).count_ones() ==
// CHECK-NEXT:         1 { [[T_16]] } else { (1u16.checked_shl(13 - ((([[T_16]] as $T) & 8191).leading_zeros()
// CHECK-NEXT:         - 3)).unwrap_or(0u16) as $T) & 8191 } } }) as u64) }; unsafe {
// CHECK-NEXT:         fflush(std::ptr::null_mut()) }; if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = $X; let _ = std::io::Write::flush(& mut
// CHECK-NEXT:         std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; __t.leading_zeros() as u32 }, { let [[T_0]] = x; (! [[T_0]])
// CHECK-NEXT:         .leading_zeros() as u32 }, { let [[T_1]] = x; [[T_1]].trailing_zeros() as u32 }, { let
// CHECK-NEXT:         [[T_2]] = x; (! [[T_2]]).trailing_zeros() as u32 }, { let [[T_3]] = x; ({ let [[T_4]] = (!
// CHECK-NEXT:         [[T_3]]).leading_zeros(); if [[T_4]] == 100 { 0u32 } else { [[T_4]] + 1 } }) as u32 }, {
// CHECK-NEXT:         let [[T_5]] = x; ({ let [[T_6]] = [[T_5]].leading_zeros(); if [[T_6]] == 100 { 0u32 } else {
// CHECK-NEXT:         [[T_6]] + 1 } }) as u32 }, { let [[T_7]] = x; ({ let [[T_8]] = (! [[T_7]]).trailing_zeros();
// CHECK-NEXT:         if [[T_8]] == 100 { 0u32 } else { [[T_8]] + 1 } }) as u32 }, { let [[T_9]] = x; ({ let
// CHECK-NEXT:         [[T_10]] = [[T_9]].trailing_zeros(); if [[T_10]] == 100 { 0u32 } else { [[T_10]] + 1 } }) as
// CHECK-NEXT:         u32 }, { let [[T_11]] = x; (100 - [[T_11]].count_ones()) as u32 }, { let [[T_12]] = x;
// CHECK-NEXT:         [[T_12]].count_ones() as u32 }, ({ let [[T_13]] = x; [[T_13]].count_ones() == 1 }) as u32,
// CHECK-NEXT:         { let [[T_14]] = x; (100 - [[T_14]].leading_zeros()) as u32 }, { let [[T_15]] = x; if
// CHECK-NEXT:         [[T_15]].leading_zeros() == 100 { < bitint::BUint < 100, 2, 16, bitint::A8
// CHECK-NEXT:         >>::from_u128(0) } else { < bitint::BUint < 100, 2, 16, bitint::A8
// CHECK-NEXT:         >>::from_u128(1).shl_bits((99 - [[T_15]].leading_zeros()) as usize) } } .to_u128()
// CHECK-NEXT:         as u64, { let [[T_16]] = x; if [[T_16]].count_ones() == 0 { < bitint::BUint < 100, 2,
// CHECK-NEXT:         16, bitint::A8 >>::from_u128(1) } else { if [[T_16]].count_ones() == 1 { [[T_16]] }
// CHECK-NEXT:         else { < bitint::BUint < 100, 2, 16, bitint::A8 >>::from_u128(1).shl_bits((100 -
// CHECK-NEXT:         [[T_16]].leading_zeros()) as usize) } } } .to_u128() as u64) }; unsafe {
// CHECK-NEXT:         fflush(std::ptr::null_mut()) }; if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT__8 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = (($X) as $T) & 127; let _ =
// CHECK-NEXT:         std::io::Write::flush(& mut std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; (((__t as $T) & 127).leading_zeros() - 1) as u32 }, {
// CHECK-NEXT:         let [[T_0]] = x; (((((! [[T_0]] as $T) & 127) as $T) & 127).leading_zeros() - 1) as u32
// CHECK-NEXT:         }, { let [[T_1]] = x; (([[T_1]] as $T) & 127).trailing_zeros().min(7u32) as u32 }, {
// CHECK-NEXT:         let [[T_2]] = x; ((((! [[T_2]] as $T) & 127) as $T) & 127).trailing_zeros().min(7u32)
// CHECK-NEXT:         as u32 }, { let [[T_3]] = x; ({ let [[T_4]] = ((((! [[T_3]] as $T) & 127) as $T) & 127)
// CHECK-NEXT:         .leading_zeros() - 1; if [[T_4]] == 7 { 0u32 } else { [[T_4]] + 1 } }) as u32 }, { let
// CHECK-NEXT:         [[T_5]] = x; ({ let [[T_6]] = (([[T_5]] as $T) & 127).leading_zeros() - 1; if [[T_6]] == 7 {
// CHECK-NEXT:         0u32 } else { [[T_6]] + 1 } }) as u32 }, { let [[T_7]] = x; ({ let [[T_8]] = ((((! [[T_7]]
// CHECK-NEXT:         as $T) & 127) as $T) & 127).trailing_zeros().min(7u32); if [[T_8]] == 7 { 0u32 }
// CHECK-NEXT:         else { [[T_8]] + 1 } }) as u32 }, { let [[T_9]] = x; ({ let [[T_10]] = (([[T_9]] as $T) &
// CHECK-NEXT:         127).trailing_zeros().min(7u32); if [[T_10]] == 7 { 0u32 } else { [[T_10]] + 1 } }) as
// CHECK-NEXT:         u32 }, { let [[T_11]] = x; (7 - (([[T_11]] as $T) & 127).count_ones()) as u32 }, { let
// CHECK-NEXT:         [[T_12]] = x; (([[T_12]] as $T) & 127).count_ones() as u32 }, ({ let [[T_13]] = x; (([[T_13]]
// CHECK-NEXT:         as $T) & 127).count_ones() == 1 }) as u32, { let [[T_14]] = x; (7 - ((([[T_14]] as $T)
// CHECK-NEXT:         & 127).leading_zeros() - 1)) as u32 }, ({ let [[T_15]] = x; if (([[T_15]] as $T) & 127)
// CHECK-NEXT:         .leading_zeros() - 1 == 7 { 0u8 } else { (1u8.checked_shl(6 - ((([[T_15]] as $T) &
// CHECK-NEXT:         127).leading_zeros() - 1)).unwrap_or(0u8) as $T) & 127 } }) as u64, ({ let [[T_16]]
// CHECK-NEXT:         = x; if (([[T_16]] as $T) & 127).count_ones() == 0 { 1u8 } else { if (([[T_16]] as $T)
// CHECK-NEXT:         & 127).count_ones() == 1 { [[T_16]] } else { (1u8.checked_shl(7 - ((([[T_16]] as $T) &
// CHECK-NEXT:         127).leading_zeros() - 1)).unwrap_or(0u8) as $T) & 127 } } }) as u64) }; unsafe {
// CHECK-NEXT:         fflush(std::ptr::null_mut()) }; if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT__9 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = ($X) as $T; let _ =
// CHECK-NEXT:         std::io::Write::flush(& mut std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; __t.leading_zeros() as $T }, { let [[T_0]] = x; (! [[T_0]] as
// CHECK-NEXT:         $T).leading_zeros() as $T }, { let [[T_1]] = x; [[T_1]].trailing_zeros() as $T }, { let
// CHECK-NEXT:         [[T_2]] = x; (! [[T_2]] as $T).trailing_zeros() as $T }, { let [[T_3]] = x; ({ let [[T_4]] =
// CHECK-NEXT:         (! [[T_3]] as $T).leading_zeros(); if [[T_4]] == 32 { 0u32 } else { [[T_4]] + 1 } }) as $T
// CHECK-NEXT:         }, { let [[T_5]] = x; ({ let [[T_6]] = [[T_5]].leading_zeros(); if [[T_6]] == 32 { 0u32 }
// CHECK-NEXT:         else { [[T_6]] + 1 } }) as $T }, { let [[T_7]] = x; ({ let [[T_8]] = (! [[T_7]] as $T)
// CHECK-NEXT:         .trailing_zeros(); if [[T_8]] == 32 { 0u32 } else { [[T_8]] + 1 } }) as $T }, { let
// CHECK-NEXT:         [[T_9]] = x; ({ let [[T_10]] = [[T_9]].trailing_zeros(); if [[T_10]] == 32 { 0u32 } else {
// CHECK-NEXT:         [[T_10]] + 1 } }) as $T }, { let [[T_11]] = x; (32 - [[T_11]].count_ones()) as $T }, { let
// CHECK-NEXT:         [[T_12]] = x; [[T_12]].count_ones() as $T }, ({ let [[T_13]] = x; [[T_13]].count_ones() == 1
// CHECK-NEXT:         }) as $T, { let [[T_14]] = x; (32 - [[T_14]].leading_zeros()) as $T }, ({ let [[T_15]] =
// CHECK-NEXT:         x; if [[T_15]].leading_zeros() == 32 { 0u32 } else { 1u32.checked_shl(31 - [[T_15]]
// CHECK-NEXT:         .leading_zeros()).unwrap_or(0u32) as $T } }) as u64, ({ let [[T_16]] = x; if [[T_16]]
// CHECK-NEXT:         .count_ones() == 0 { 1u32 } else { if [[T_16]].count_ones() == 1 { [[T_16]] } else {
// CHECK-NEXT:         1u32.checked_shl(32 - [[T_16]].leading_zeros()).unwrap_or(0u32) as $T } } }) as u64)
// CHECK-NEXT:         }; unsafe { fflush(std::ptr::null_mut()) }; if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT__10 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = (($X) as $T) & 137438953471; let _ =
// CHECK-NEXT:         std::io::Write::flush(& mut std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; (((__t as $T) & 137438953471).leading_zeros() - 27) as
// CHECK-NEXT:         u32 }, { let [[T_0]] = x; (((((! [[T_0]] as $T) & 137438953471) as $T) & 137438953471)
// CHECK-NEXT:         .leading_zeros() - 27) as u32 }, { let [[T_1]] = x; (([[T_1]] as $T) & 137438953471)
// CHECK-NEXT:         .trailing_zeros().min(37u32) as u32 }, { let [[T_2]] = x; ((((! [[T_2]] as $T) &
// CHECK-NEXT:         137438953471) as $T) & 137438953471).trailing_zeros().min(37u32) as u32 }, { let
// CHECK-NEXT:         [[T_3]] = x; ({ let [[T_4]] = ((((! [[T_3]] as $T) & 137438953471) as $T) & 137438953471)
// CHECK-NEXT:         .leading_zeros() - 27; if [[T_4]] == 37 { 0u32 } else { [[T_4]] + 1 } }) as u32 }, {
// CHECK-NEXT:         let [[T_5]] = x; ({ let [[T_6]] = (([[T_5]] as $T) & 137438953471).leading_zeros() - 27;
// CHECK-NEXT:         if [[T_6]] == 37 { 0u32 } else { [[T_6]] + 1 } }) as u32 }, { let [[T_7]] = x; ({ let
// CHECK-NEXT:         [[T_8]] = ((((! [[T_7]] as $T) & 137438953471) as $T) & 137438953471).trailing_zeros()
// CHECK-NEXT:         .min(37u32); if [[T_8]] == 37 { 0u32 } else { [[T_8]] + 1 } }) as u32 }, { let [[T_9]]
// CHECK-NEXT:         = x; ({ let [[T_10]] = (([[T_9]] as $T) & 137438953471).trailing_zeros().min(37u32);
// CHECK-NEXT:         if [[T_10]] == 37 { 0u32 } else { [[T_10]] + 1 } }) as u32 }, { let [[T_11]] = x; (37 -
// CHECK-NEXT:         (([[T_11]] as $T) & 137438953471).count_ones()) as u32 }, { let [[T_12]] = x; (([[T_12]]
// CHECK-NEXT:         as $T) & 137438953471).count_ones() as u32 }, ({ let [[T_13]] = x; (([[T_13]] as $T) &
// CHECK-NEXT:         137438953471).count_ones() == 1 }) as u32, { let [[T_14]] = x; (37 - ((([[T_14]] as $T)
// CHECK-NEXT:         & 137438953471).leading_zeros() - 27)) as u32 }, ({ let [[T_15]] = x; if (([[T_15]] as
// CHECK-NEXT:         $T) & 137438953471).leading_zeros() - 27 == 37 { 0u64 } else { (1u64
// CHECK-NEXT:         .checked_shl(36 - ((([[T_15]] as $T) & 137438953471).leading_zeros() - 27))
// CHECK-NEXT:         .unwrap_or(0u64) as $T) & 137438953471 } }) as $T, ({ let [[T_16]] = x; if (([[T_16]]
// CHECK-NEXT:         as $T) & 137438953471).count_ones() == 0 { 1u64 } else { if (([[T_16]] as $T) &
// CHECK-NEXT:         137438953471).count_ones() == 1 { [[T_16]] } else { (1u64.checked_shl(37 - ((([[T_16]]
// CHECK-NEXT:         as $T) & 137438953471).leading_zeros() - 27)).unwrap_or(0u64) as $T) &
// CHECK-NEXT:         137438953471 } } }) as $T) }; unsafe { fflush(std::ptr::null_mut()) }; if true {
// CHECK-NEXT:         break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT__11 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = $X; let _ = std::io::Write::flush(& mut
// CHECK-NEXT:         std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; (((__t as $T) & 127).leading_zeros() - 1) as u32 }, {
// CHECK-NEXT:         let [[T_0]] = x; (((((! [[T_0]] as $T) & 127) as $T) & 127).leading_zeros() - 1) as u32
// CHECK-NEXT:         }, { let [[T_1]] = x; (([[T_1]] as $T) & 127).trailing_zeros().min(7u32) as u32 }, {
// CHECK-NEXT:         let [[T_2]] = x; ((((! [[T_2]] as $T) & 127) as $T) & 127).trailing_zeros().min(7u32)
// CHECK-NEXT:         as u32 }, { let [[T_3]] = x; ({ let [[T_4]] = ((((! [[T_3]] as $T) & 127) as $T) & 127)
// CHECK-NEXT:         .leading_zeros() - 1; if [[T_4]] == 7 { 0u32 } else { [[T_4]] + 1 } }) as u32 }, { let
// CHECK-NEXT:         [[T_5]] = x; ({ let [[T_6]] = (([[T_5]] as $T) & 127).leading_zeros() - 1; if [[T_6]] == 7 {
// CHECK-NEXT:         0u32 } else { [[T_6]] + 1 } }) as u32 }, { let [[T_7]] = x; ({ let [[T_8]] = ((((! [[T_7]]
// CHECK-NEXT:         as $T) & 127) as $T) & 127).trailing_zeros().min(7u32); if [[T_8]] == 7 { 0u32 }
// CHECK-NEXT:         else { [[T_8]] + 1 } }) as u32 }, { let [[T_9]] = x; ({ let [[T_10]] = (([[T_9]] as $T) &
// CHECK-NEXT:         127).trailing_zeros().min(7u32); if [[T_10]] == 7 { 0u32 } else { [[T_10]] + 1 } }) as
// CHECK-NEXT:         u32 }, { let [[T_11]] = x; (7 - (([[T_11]] as $T) & 127).count_ones()) as u32 }, { let
// CHECK-NEXT:         [[T_12]] = x; (([[T_12]] as $T) & 127).count_ones() as u32 }, ({ let [[T_13]] = x; (([[T_13]]
// CHECK-NEXT:         as $T) & 127).count_ones() == 1 }) as u32, { let [[T_14]] = x; (7 - ((([[T_14]] as $T)
// CHECK-NEXT:         & 127).leading_zeros() - 1)) as u32 }, ({ let [[T_15]] = x; if (([[T_15]] as $T) & 127)
// CHECK-NEXT:         .leading_zeros() - 1 == 7 { 0u8 } else { (1u8.checked_shl(6 - ((([[T_15]] as $T) &
// CHECK-NEXT:         127).leading_zeros() - 1)).unwrap_or(0u8) as $T) & 127 } }) as u64, ({ let [[T_16]]
// CHECK-NEXT:         = x; if (([[T_16]] as $T) & 127).count_ones() == 0 { 1u8 } else { if (([[T_16]] as $T)
// CHECK-NEXT:         & 127).count_ones() == 1 { [[T_16]] } else { (1u8.checked_shl(7 - ((([[T_16]] as $T) &
// CHECK-NEXT:         127).leading_zeros() - 1)).unwrap_or(0u8) as $T) & 127 } } }) as u64) }; unsafe {
// CHECK-NEXT:         fflush(std::ptr::null_mut()) }; if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT__12 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = ($X) as $T; let _ =
// CHECK-NEXT:         std::io::Write::flush(& mut std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; __t.leading_zeros() as u32 }, { let [[T_0]] = x; (! [[T_0]]
// CHECK-NEXT:         as $T).leading_zeros() as u32 }, { let [[T_1]] = x; [[T_1]].trailing_zeros() as u32 },
// CHECK-NEXT:         { let [[T_2]] = x; (! [[T_2]] as $T).trailing_zeros() as u32 }, { let [[T_3]] = x; ({ let
// CHECK-NEXT:         [[T_4]] = (! [[T_3]] as $T).leading_zeros(); if [[T_4]] == 16 { 0u32 } else { [[T_4]] + 1 }
// CHECK-NEXT:         }) as u32 }, { let [[T_5]] = x; ({ let [[T_6]] = [[T_5]].leading_zeros(); if [[T_6]] == 16 {
// CHECK-NEXT:         0u32 } else { [[T_6]] + 1 } }) as u32 }, { let [[T_7]] = x; ({ let [[T_8]] = (! [[T_7]] as
// CHECK-NEXT:         $T).trailing_zeros(); if [[T_8]] == 16 { 0u32 } else { [[T_8]] + 1 } }) as u32 }, {
// CHECK-NEXT:         let [[T_9]] = x; ({ let [[T_10]] = [[T_9]].trailing_zeros(); if [[T_10]] == 16 { 0u32 }
// CHECK-NEXT:         else { [[T_10]] + 1 } }) as u32 }, { let [[T_11]] = x; (16 - [[T_11]].count_ones()) as u32
// CHECK-NEXT:         }, { let [[T_12]] = x; [[T_12]].count_ones() as u32 }, ({ let [[T_13]] = x; [[T_13]]
// CHECK-NEXT:         .count_ones() == 1 }) as u32, { let [[T_14]] = x; (16 - [[T_14]].leading_zeros()) as
// CHECK-NEXT:         u32 }, ({ let [[T_15]] = x; if [[T_15]].leading_zeros() == 16 { 0u16 } else { 1u16
// CHECK-NEXT:         .checked_shl(15 - [[T_15]].leading_zeros()).unwrap_or(0u16) as $T } }) as u64, ({
// CHECK-NEXT:         let [[T_16]] = x; if [[T_16]].count_ones() == 0 { 1u16 } else { if [[T_16]].count_ones()
// CHECK-NEXT:         == 1 { [[T_16]] } else { 1u16.checked_shl(16 - [[T_16]].leading_zeros())
// CHECK-NEXT:         .unwrap_or(0u16) as $T } } }) as u64) }; unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:         if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT__13 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = ($X) as $T; let _ =
// CHECK-NEXT:         std::io::Write::flush(& mut std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; __t.leading_zeros() as u32 }, { let [[T_0]] = x; (! [[T_0]]
// CHECK-NEXT:         as $T).leading_zeros() as u32 }, { let [[T_1]] = x; [[T_1]].trailing_zeros() as u32 },
// CHECK-NEXT:         { let [[T_2]] = x; (! [[T_2]] as $T).trailing_zeros() as u32 }, { let [[T_3]] = x; ({ let
// CHECK-NEXT:         [[T_4]] = (! [[T_3]] as $T).leading_zeros(); if [[T_4]] == 8 { 0u32 } else { [[T_4]] + 1 } })
// CHECK-NEXT:         as u32 }, { let [[T_5]] = x; ({ let [[T_6]] = [[T_5]].leading_zeros(); if [[T_6]] == 8 { 0u32
// CHECK-NEXT:         } else { [[T_6]] + 1 } }) as u32 }, { let [[T_7]] = x; ({ let [[T_8]] = (! [[T_7]] as $T)
// CHECK-NEXT:         .trailing_zeros(); if [[T_8]] == 8 { 0u32 } else { [[T_8]] + 1 } }) as u32 }, { let
// CHECK-NEXT:         [[T_9]] = x; ({ let [[T_10]] = [[T_9]].trailing_zeros(); if [[T_10]] == 8 { 0u32 } else {
// CHECK-NEXT:         [[T_10]] + 1 } }) as u32 }, { let [[T_11]] = x; (8 - [[T_11]].count_ones()) as u32 }, {
// CHECK-NEXT:         let [[T_12]] = x; [[T_12]].count_ones() as u32 }, ({ let [[T_13]] = x; [[T_13]].count_ones()
// CHECK-NEXT:         == 1 }) as u32, { let [[T_14]] = x; (8 - [[T_14]].leading_zeros()) as u32 }, ({ let
// CHECK-NEXT:         [[T_15]] = x; if [[T_15]].leading_zeros() == 8 { 0u8 } else { 1u8.checked_shl(7 - [[T_15]]
// CHECK-NEXT:         .leading_zeros()).unwrap_or(0u8) as $T } }) as u64, ({ let [[T_16]] = x; if [[T_16]]
// CHECK-NEXT:         .count_ones() == 0 { 1u8 } else { if [[T_16]].count_ones() == 1 { [[T_16]] } else { 1u8
// CHECK-NEXT:         .checked_shl(8 - [[T_16]].leading_zeros()).unwrap_or(0u8) as $T } } }) as u64) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) }; if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT__14 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = $X; let _ = std::io::Write::flush(& mut
// CHECK-NEXT:         std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; (((__t as $T) & 8191).leading_zeros() - 3) as u32 }, {
// CHECK-NEXT:         let [[T_0]] = x; (((((! [[T_0]] as $T) & 8191) as $T) & 8191).leading_zeros() - 3) as
// CHECK-NEXT:         u32 }, { let [[T_1]] = x; (([[T_1]] as $T) & 8191).trailing_zeros().min(13u32) as u32
// CHECK-NEXT:         }, { let [[T_2]] = x; ((((! [[T_2]] as $T) & 8191) as $T) & 8191).trailing_zeros()
// CHECK-NEXT:         .min(13u32) as u32 }, { let [[T_3]] = x; ({ let [[T_4]] = ((((! [[T_3]] as $T) & 8191) as
// CHECK-NEXT:         $T) & 8191).leading_zeros() - 3; if [[T_4]] == 13 { 0u32 } else { [[T_4]] + 1 } }) as
// CHECK-NEXT:         u32 }, { let [[T_5]] = x; ({ let [[T_6]] = (([[T_5]] as $T) & 8191).leading_zeros() - 3;
// CHECK-NEXT:         if [[T_6]] == 13 { 0u32 } else { [[T_6]] + 1 } }) as u32 }, { let [[T_7]] = x; ({ let
// CHECK-NEXT:         [[T_8]] = ((((! [[T_7]] as $T) & 8191) as $T) & 8191).trailing_zeros().min(13u32); if
// CHECK-NEXT:         [[T_8]] == 13 { 0u32 } else { [[T_8]] + 1 } }) as u32 }, { let [[T_9]] = x; ({ let
// CHECK-NEXT:         [[T_10]] = (([[T_9]] as $T) & 8191).trailing_zeros().min(13u32); if [[T_10]] == 13 { 0u32
// CHECK-NEXT:         } else { [[T_10]] + 1 } }) as u32 }, { let [[T_11]] = x; (13 - (([[T_11]] as $T) & 8191)
// CHECK-NEXT:         .count_ones()) as u32 }, { let [[T_12]] = x; (([[T_12]] as $T) & 8191).count_ones() as
// CHECK-NEXT:         u32 }, ({ let [[T_13]] = x; (([[T_13]] as $T) & 8191).count_ones() == 1 }) as u32, {
// CHECK-NEXT:         let [[T_14]] = x; (13 - ((([[T_14]] as $T) & 8191).leading_zeros() - 3)) as u32 }, ({
// CHECK-NEXT:         let [[T_15]] = x; if (([[T_15]] as $T) & 8191).leading_zeros() - 3 == 13 { 0u16 } else
// CHECK-NEXT:         { (1u16.checked_shl(12 - ((([[T_15]] as $T) & 8191).leading_zeros() - 3))
// CHECK-NEXT:         .unwrap_or(0u16) as $T) & 8191 } }) as u64, ({ let [[T_16]] = x; if (([[T_16]] as $T) &
// CHECK-NEXT:         8191).count_ones() == 0 { 1u16 } else { if (([[T_16]] as $T) & 8191).count_ones() ==
// CHECK-NEXT:         1 { [[T_16]] } else { (1u16.checked_shl(13 - ((([[T_16]] as $T) & 8191).leading_zeros()
// CHECK-NEXT:         - 3)).unwrap_or(0u16) as $T) & 8191 } } }) as u64) }; unsafe {
// CHECK-NEXT:         fflush(std::ptr::null_mut()) }; if true { break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT__15 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = $X; let _ = std::io::Write::flush(& mut
// CHECK-NEXT:         std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; (((__t as $T) & 137438953471).leading_zeros() - 27) as
// CHECK-NEXT:         u32 }, { let [[T_0]] = x; (((((! [[T_0]] as $T) & 137438953471) as $T) & 137438953471)
// CHECK-NEXT:         .leading_zeros() - 27) as u32 }, { let [[T_1]] = x; (([[T_1]] as $T) & 137438953471)
// CHECK-NEXT:         .trailing_zeros().min(37u32) as u32 }, { let [[T_2]] = x; ((((! [[T_2]] as $T) &
// CHECK-NEXT:         137438953471) as $T) & 137438953471).trailing_zeros().min(37u32) as u32 }, { let
// CHECK-NEXT:         [[T_3]] = x; ({ let [[T_4]] = ((((! [[T_3]] as $T) & 137438953471) as $T) & 137438953471)
// CHECK-NEXT:         .leading_zeros() - 27; if [[T_4]] == 37 { 0u32 } else { [[T_4]] + 1 } }) as u32 }, {
// CHECK-NEXT:         let [[T_5]] = x; ({ let [[T_6]] = (([[T_5]] as $T) & 137438953471).leading_zeros() - 27;
// CHECK-NEXT:         if [[T_6]] == 37 { 0u32 } else { [[T_6]] + 1 } }) as u32 }, { let [[T_7]] = x; ({ let
// CHECK-NEXT:         [[T_8]] = ((((! [[T_7]] as $T) & 137438953471) as $T) & 137438953471).trailing_zeros()
// CHECK-NEXT:         .min(37u32); if [[T_8]] == 37 { 0u32 } else { [[T_8]] + 1 } }) as u32 }, { let [[T_9]]
// CHECK-NEXT:         = x; ({ let [[T_10]] = (([[T_9]] as $T) & 137438953471).trailing_zeros().min(37u32);
// CHECK-NEXT:         if [[T_10]] == 37 { 0u32 } else { [[T_10]] + 1 } }) as u32 }, { let [[T_11]] = x; (37 -
// CHECK-NEXT:         (([[T_11]] as $T) & 137438953471).count_ones()) as u32 }, { let [[T_12]] = x; (([[T_12]]
// CHECK-NEXT:         as $T) & 137438953471).count_ones() as u32 }, ({ let [[T_13]] = x; (([[T_13]] as $T) &
// CHECK-NEXT:         137438953471).count_ones() == 1 }) as u32, { let [[T_14]] = x; (37 - ((([[T_14]] as $T)
// CHECK-NEXT:         & 137438953471).leading_zeros() - 27)) as u32 }, ({ let [[T_15]] = x; if (([[T_15]] as
// CHECK-NEXT:         $T) & 137438953471).leading_zeros() - 27 == 37 { 0u64 } else { (1u64
// CHECK-NEXT:         .checked_shl(36 - ((([[T_15]] as $T) & 137438953471).leading_zeros() - 27))
// CHECK-NEXT:         .unwrap_or(0u64) as $T) & 137438953471 } }) as $T, ({ let [[T_16]] = x; if (([[T_16]]
// CHECK-NEXT:         as $T) & 137438953471).count_ones() == 0 { 1u64 } else { if (([[T_16]] as $T) &
// CHECK-NEXT:         137438953471).count_ones() == 1 { [[T_16]] } else { (1u64.checked_shl(37 - ((([[T_16]]
// CHECK-NEXT:         as $T) & 137438953471).leading_zeros() - 27)).unwrap_or(0u64) as $T) &
// CHECK-NEXT:         137438953471 } } }) as $T) }; unsafe { fflush(std::ptr::null_mut()) }; if true {
// CHECK-NEXT:         break; } }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: macro_rules! REPORT__3 {
// CHECK-NEXT:     ($T:ty, $X:expr) => {
// CHECK-NEXT:         '__slate_label_0 : loop { let x : $T = $X; let _ = std::io::Write::flush(& mut
// CHECK-NEXT:         std::io::stdout()); unsafe {
// CHECK-NEXT:         slate_support::printf(c"%u %u %u %u %u %u %u %u %u %u %u %u %llu %llu\n"
// CHECK-NEXT:         .as_ptr(), { let __t = x; __t.leading_zeros() as u32 }, { let [[T_0]] = x; (! [[T_0]])
// CHECK-NEXT:         .leading_zeros() as u32 }, { let [[T_1]] = x; [[T_1]].trailing_zeros() as u32 }, { let
// CHECK-NEXT:         [[T_2]] = x; (! [[T_2]]).trailing_zeros() as u32 }, { let [[T_3]] = x; ({ let [[T_4]] = (!
// CHECK-NEXT:         [[T_3]]).leading_zeros(); if [[T_4]] == 65 { 0u32 } else { [[T_4]] + 1 } }) as u32 }, {
// CHECK-NEXT:         let [[T_5]] = x; ({ let [[T_6]] = [[T_5]].leading_zeros(); if [[T_6]] == 65 { 0u32 } else {
// CHECK-NEXT:         [[T_6]] + 1 } }) as u32 }, { let [[T_7]] = x; ({ let [[T_8]] = (! [[T_7]]).trailing_zeros();
// CHECK-NEXT:         if [[T_8]] == 65 { 0u32 } else { [[T_8]] + 1 } }) as u32 }, { let [[T_9]] = x; ({ let
// CHECK-NEXT:         [[T_10]] = [[T_9]].trailing_zeros(); if [[T_10]] == 65 { 0u32 } else { [[T_10]] + 1 } }) as
// CHECK-NEXT:         u32 }, { let [[T_11]] = x; (65 - [[T_11]].count_ones()) as u32 }, { let [[T_12]] = x;
// CHECK-NEXT:         [[T_12]].count_ones() as u32 }, ({ let [[T_13]] = x; [[T_13]].count_ones() == 1 }) as u32,
// CHECK-NEXT:         { let [[T_14]] = x; (65 - [[T_14]].leading_zeros()) as u32 }, { let [[T_15]] = x; if [[T_15]]
// CHECK-NEXT:         .leading_zeros() == 65 { < bitint::BUint < 65, 2, 16, bitint::A8 >>::from_u128(0)
// CHECK-NEXT:         } else { < bitint::BUint < 65, 2, 16, bitint::A8 >>::from_u128(1).shl_bits((64 -
// CHECK-NEXT:         [[T_15]].leading_zeros()) as usize) } } .to_u128() as u64, { let [[T_16]] = x; if [[T_16]]
// CHECK-NEXT:         .count_ones() == 0 { < bitint::BUint < 65, 2, 16, bitint::A8 >>::from_u128(1) }
// CHECK-NEXT:         else { if [[T_16]].count_ones() == 1 { [[T_16]] } else { < bitint::BUint < 65, 2, 16,
// CHECK-NEXT:         bitint::A8 >>::from_u128(1).shl_bits((65 - [[T_16]].leading_zeros()) as usize) } } }
// CHECK-NEXT:         .to_u128() as u64) }; unsafe { fflush(std::ptr::null_mut()) }; if true { break; }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: static mut seeds: __SlateAlign16<[u64; 24]> = __SlateAlign16([
// CHECK-NEXT:     0u64, 1u64, 2u64, 3u64, 5u64, 8u64, 64u64, 100u64, 127u64, 128u64, 129u64, 255u64,
// CHECK-NEXT:     256u64, 4097u64, 65535u64, 65536u64, 2147483647u64, 2147483648u64, 4294967295u64,
// CHECK-NEXT:     4294967296u64, 81985529216486895u64, 9223372036854775807u64, 9223372036854775808u64,
// CHECK-NEXT:     18446744073709551615u64,
// CHECK-NEXT: ]);
// CHECK-NEXT: fn wide() {
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64)
// CHECK-NEXT:         < (std::mem::size_of::<[u64; 24]>() as u64) / (std::mem::size_of::<u64>() as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut w: bitint::BUint<100, 2, 16, bitint::A8> = <bitint::BUint<
// CHECK-NEXT:             100,
// CHECK-NEXT:             2,
// CHECK-NEXT:             16,
// CHECK-NEXT:             bitint::A8,
// CHECK-NEXT:         >>::from_u128((unsafe { seeds.0[i as usize] }) as u128);
// CHECK-NEXT:         REPORT!(bitint::BUint < 100, 2, 16, bitint::A8 >, w);
// CHECK-NEXT:         REPORT!(bitint::BUint < 100, 2, 16, bitint::A8 >, w.shl_bits(30 as usize));
// CHECK-NEXT:         REPORT!(bitint::BUint < 100, 2, 16, bitint::A8 >, w.bitnot());
// CHECK-NEXT:         REPORT__3!(
// CHECK-NEXT:             bitint::BUint < 65, 2, 16, bitint::A8 >, < bitint::BUint < 65, 2, 16,
// CHECK-NEXT:             bitint::A8 >>::from_u128((unsafe { seeds.0[i as usize] }) as u128)
// CHECK-NEXT:         );
// CHECK-NEXT:         REPORT__3!(
// CHECK-NEXT:             bitint::BUint < 65, 2, 16, bitint::A8 >, < bitint::BUint < 65, 2, 16,
// CHECK-NEXT:             bitint::A8 >>::from_u128((unsafe { seeds.0[i as usize] }) as u128).bitnot()
// CHECK-NEXT:         );
// CHECK-NEXT:         REPORT__2!(
// CHECK-NEXT:             bitint::BUint < 128, 2, 16, bitint::A8 >, < bitint::BUint < 128, 2, 16,
// CHECK-NEXT:             bitint::A8 >>::from_u128((unsafe { seeds.0[i as usize] }) as u128)
// CHECK-NEXT:             .shl_bits(64 as usize)
// CHECK-NEXT:         );
// CHECK-NEXT:         REPORT__2!(
// CHECK-NEXT:             bitint::BUint < 128, 2, 16, bitint::A8 >, < bitint::BUint < 128, 2, 16,
// CHECK-NEXT:             bitint::A8 >>::from_u128((unsafe { seeds.0[i as usize] }) as u128).bitnot()
// CHECK-NEXT:         );
// CHECK-NEXT:         i = i.wrapping_add(1);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: u32 = 0;
// CHECK-NEXT:     while (i as u64)
// CHECK-NEXT:         < (std::mem::size_of::<[u64; 24]>() as u64) / (std::mem::size_of::<u64>() as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         let s: u64 = unsafe { seeds.0[i as usize] };
// CHECK-NEXT:         REPORT__13!(u8, s);
// CHECK-NEXT:         REPORT__12!(u16, s);
// CHECK-NEXT:         REPORT__9!(u32, s);
// CHECK-NEXT:         REPORT__4!(u64, s);
// CHECK-NEXT:         REPORT__5!(u8, s);
// CHECK-NEXT:         REPORT__8!(u8, s);
// CHECK-NEXT:         REPORT__7!(u16, s);
// CHECK-NEXT:         REPORT__10!(u64, s);
// CHECK-NEXT:         REPORT__6!(u64, s);
// CHECK-NEXT:         REPORT__11!(u8, (! ((s as u8) & 127) as u8) & 127);
// CHECK-NEXT:         REPORT__14!(u16, (! ((s as u16) & 8191) as u16) & 8191);
// CHECK-NEXT:         REPORT__15!(u64, (! ((s as u64) & 137438953471) as u64) & 137438953471);
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
