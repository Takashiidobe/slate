#include <stdio.h>

void abort(void);

struct W {
  int tag;
  _BitInt(128) s;
  unsigned _BitInt(128) u;
};

static _BitInt(128) add128(_BitInt(128) a, _BitInt(128) b) { return a + b; }

static unsigned _BitInt(128)
    mul128(unsigned _BitInt(128) a, unsigned _BitInt(128) b) {
  return a * b;
}

static int switch_wide_case_u128(unsigned _BitInt(128) x) {
  switch (x) {
  case 200000000000000000000000000000000000000uwb:
    return 1;
  default:
    return 0;
  }
}

static int switch_wide_selector(_BitInt(256) x) {
  switch (x) {
  case 1:
  case 2:
    return 10;
  case -987654321098765432109876543210987654321098765432109876543210wb:
    return 20;
  case 300 ... 400:
    return 30;
  default:
    return 0;
  }
}

static int switch_wide_selector_unsigned(unsigned _BitInt(256) x) {
  switch (x) {
  case 123456789012345678901234567890123456789012345678901234567890uwb:
    return 1;
  case 0:
    return 2;
  default:
    return 3;
  }
}

static int switch_wide_fallthrough(_BitInt(256) x) {
  int acc = 0;
  switch (x) {
  case 1:
    acc += 100;
  case 2:
    acc += 1;
    break;
  case 3:
    acc = 999;
    break;
  default:
    acc = -1;
  }
  return acc;
}

static _BitInt(256) add256(_BitInt(256) a, _BitInt(256) b) { return a + b; }

static unsigned _BitInt(256)
    mul256(unsigned _BitInt(256) a, unsigned _BitInt(256) b) {
  return a * b;
}

// Odd, non-64-aligned widths: exercise BITS that don't line up with a
// native Rust integer or a whole number of 64-bit limbs. Values stay well
// inside each width's representable range, so wraparound never fires; the
// invariants below (undoing +/- and double negation) hold for any width
// under two's-complement arithmetic, so they need no per-width hand
// computation.
static void check_bitint3(void) {
  _BitInt(3) x   = 1;
  _BitInt(3) y   = -2;
  _BitInt(3) sum = x + y;
  if (sum - y != x)
    abort();
  if (-(-x) != x)
    abort();
  if (!(y < x))
    abort();

  unsigned _BitInt(3) ux   = 5;
  unsigned _BitInt(3) uy   = 2;
  unsigned _BitInt(3) usum = ux + uy;
  if (usum - uy != ux)
    abort();
}

static void check_bitint129(void) {
  _BitInt(129) x  = 123456789;
  _BitInt(129) y  = -987654321;
  _BitInt(129) s2 = x + y;
  if (s2 - y != x)
    abort();
  if (-(-x) != x)
    abort();
  if (!(y < x))
    abort();

  unsigned _BitInt(129) ux   = 123456789;
  unsigned _BitInt(129) uy   = 987654321;
  unsigned _BitInt(129) usum = ux + uy;
  if (usum - uy != ux)
    abort();
}

static void check_bitint512(void) {
  _BitInt(512) x  = 123456789012345LL;
  _BitInt(512) y  = -987654321098765LL;
  _BitInt(512) s3 = x + y;
  if (s3 - y != x)
    abort();
  if (-(-x) != x)
    abort();
  if (!(y < x))
    abort();

  unsigned _BitInt(512) ux   = 123456789012345ULL;
  unsigned _BitInt(512) uy   = 987654321098765ULL;
  unsigned _BitInt(512) usum = ux + uy;
  if (usum - uy != ux)
    abort();
}

// Decimal literals whose magnitude exceeds u128::MAX (2^128) but still fits
// the target _BitInt(N) width, exercising the arbitrary-width decimal-string
// parse path instead of the i128/u128 fast paths.
static void check_bitint_wide_literal(void) {
  _BitInt(256) x256 =
      123456789012345678901234567890123456789012345678901234567890wb;
  _BitInt(256) y256 =
      -123456789012345678901234567890123456789012345678901234567890wb;
  if (x256 + y256 != 0)
    abort();
  if (-x256 != y256)
    abort();
  if (!(y256 < x256))
    abort();

  unsigned _BitInt(256) ux256 =
      123456789012345678901234567890123456789012345678901234567890uwb;
  unsigned _BitInt(256) uy256   = 300uwb;
  unsigned _BitInt(256) usum256 = ux256 + uy256;
  if (usum256 - uy256 != ux256)
    abort();

  _BitInt(512) x512 =
      123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890wb;
  _BitInt(512) y512 =
      -123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890wb;
  if (x512 + y512 != 0)
    abort();
  if (-x512 != y512)
    abort();
  if (!(y512 < x512))
    abort();

  unsigned _BitInt(512) ux512 =
      123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890uwb;
  unsigned _BitInt(512) uy512   = 512uwb;
  unsigned _BitInt(512) usum512 = ux512 + uy512;
  if (usum512 - uy512 != ux512)
    abort();
}

int main(void) {
  _BitInt(128) x128   = -12345;
  _BitInt(128) y128   = 100;
  _BitInt(128) sum128 = add128(x128, y128);
  printf("%lld\n", (long long)sum128);

  unsigned _BitInt(128) p128    = 1000000000000ULL;
  unsigned _BitInt(128) q128    = 3;
  unsigned _BitInt(128) prod128 = mul128(p128, q128);
  printf("%llu\n", (unsigned long long)prod128);

  struct W w;
  w.tag  = 1;
  w.s    = -5;
  w.u    = 12345678901234567890ULL;
  w.s   += 10;
  w.u   *= 2;
  printf("%zu\n", sizeof(struct W));
  printf("%lld\n", (long long)w.s);
  printf("%llu\n", (unsigned long long)w.u);

  _BitInt(256) x256   = 12345;
  _BitInt(256) y256   = -300;
  _BitInt(256) sum256 = add256(x256, y256);
  if (sum256 != 12045)
    abort();

  unsigned _BitInt(256) p256    = 1000000000000ULL;
  unsigned _BitInt(256) q256    = 3;
  unsigned _BitInt(256) prod256 = mul256(p256, q256);
  if (prod256 != 3000000000000ULL)
    abort();

  _BitInt(256) neg256 = -x256;
  if (neg256 != -12345)
    abort();

  _BitInt(256) diff256 = x256 - y256;
  if (diff256 != 12645)
    abort();

  if (!(y256 < x256))
    abort();
  if (!(x256 > y256))
    abort();

  check_bitint3();
  check_bitint129();
  check_bitint512();
  check_bitint_wide_literal();

  if (switch_wide_case_u128(200000000000000000000000000000000000000uwb) != 1)
    abort();
  if (switch_wide_case_u128(1) != 0)
    abort();

  if (switch_wide_selector(1) != 10)
    abort();
  if (switch_wide_selector(2) != 10)
    abort();
  if (switch_wide_selector(
          -987654321098765432109876543210987654321098765432109876543210wb) !=
      20)
    abort();
  if (switch_wide_selector(350) != 30)
    abort();
  if (switch_wide_selector(300) != 30)
    abort();
  if (switch_wide_selector(400) != 30)
    abort();
  if (switch_wide_selector(999) != 0)
    abort();

  if (switch_wide_selector_unsigned(
          123456789012345678901234567890123456789012345678901234567890uwb) != 1)
    abort();
  if (switch_wide_selector_unsigned(0) != 2)
    abort();
  if (switch_wide_selector_unsigned(42) != 3)
    abort();

  if (switch_wide_fallthrough(1) != 101)
    abort();
  if (switch_wide_fallthrough(2) != 1)
    abort();
  if (switch_wide_fallthrough(3) != 999)
    abort();
  if (switch_wide_fallthrough(4) != -1)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct W {
// CHECK-NEXT:     tag: i32,
// CHECK-NEXT:     s: bitint::BInt<128, 2, 16, bitint::A8>,
// CHECK-NEXT:     u: bitint::BUint<128, 2, 16, bitint::A8>,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn add128(
// CHECK-NEXT:     mut a: bitint::BInt<128, 2, 16, bitint::A8>,
// CHECK-NEXT:     b: bitint::BInt<128, 2, 16, bitint::A8>,
// CHECK-NEXT: ) -> bitint::BInt<128, 2, 16, bitint::A8> {
// CHECK-NEXT:     return a.wrapping_add(b);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn mul128(
// CHECK-NEXT:     mut a: bitint::BUint<128, 2, 16, bitint::A8>,
// CHECK-NEXT:     b: bitint::BUint<128, 2, 16, bitint::A8>,
// CHECK-NEXT: ) -> bitint::BUint<128, 2, 16, bitint::A8> {
// CHECK-NEXT:     return a.wrapping_mul(b);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn switch_wide_case_u128(mut x: bitint::BUint<128, 2, 16, bitint::A8>) -> i32 {
// CHECK-NEXT:     match x.to_u128() {
// CHECK-NEXT:         200000000000000000000000000000000000000 => {
// CHECK-NEXT:             return 1;
// CHECK-NEXT:         }
// CHECK-NEXT:         _ => {
// CHECK-NEXT:             return 0;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn switch_wide_selector(mut [[T_0:__t[0-9]+]]: bitint::BInt<256, 4, 32, bitint::A8>) -> i32 {
// CHECK-NEXT:     let [[T_1:__t[0-9]+]]: i64 = if [[T_0]].compare(<bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(1)) == 0
// CHECK-NEXT:         || [[T_0]].compare(<bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(2)) == 0
// CHECK-NEXT:     {
// CHECK-NEXT:         0
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if [[T_0]].compare(<bitint::BInt<256, 4, 32, bitint::A8>>::from_le_bytes::<
// CHECK-NEXT:             208,
// CHECK-NEXT:             26,
// CHECK-NEXT:         >([
// CHECK-NEXT:             22, 129, 57, 174, 40, 152, 225, 202, 105, 52, 211, 44, 12, 27, 248, 174, 51, 43, 71,
// CHECK-NEXT:             18, 204, 192, 87, 168, 98, 255,
// CHECK-NEXT:         ])) == 0
// CHECK-NEXT:         {
// CHECK-NEXT:             1
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if [[T_0]].compare(<bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(300)) >= 0
// CHECK-NEXT:                 && [[T_0]].compare(<bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(400)) <= 0
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
// CHECK-NEXT:                 return 10;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 return 20;
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 return 30;
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
// CHECK-EMPTY:
// CHECK-NEXT: fn switch_wide_selector_unsigned(mut [[T_0]]: bitint::BUint<256, 4, 32, bitint::A8>) -> i32 {
// CHECK-NEXT:     let [[T_1]]: i64 = if [[T_0]].compare(<bitint::BUint<256, 4, 32, bitint::A8>>::from_le_bytes::<
// CHECK-NEXT:         208,
// CHECK-NEXT:         26,
// CHECK-NEXT:     >([
// CHECK-NEXT:         210, 10, 63, 206, 150, 241, 207, 140, 156, 180, 55, 140, 55, 164, 135, 63, 23, 98, 30, 188,
// CHECK-NEXT:         228, 4, 245, 170, 19, 0,
// CHECK-NEXT:     ])) == 0
// CHECK-NEXT:     {
// CHECK-NEXT:         0
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if [[T_0]].compare(<bitint::BUint<256, 4, 32, bitint::A8>>::from_u128(0)) == 0 {
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
// CHECK-NEXT:                 return 3;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe { std::mem::zeroed() }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn switch_wide_fallthrough(mut [[T_0]]: bitint::BInt<256, 4, 32, bitint::A8>) -> i32 {
// CHECK-NEXT:     let mut acc: i32 = 0;
// CHECK-NEXT:     let mut [[T_1]]: i64 = if [[T_0]].compare(<bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(1)) == 0 {
// CHECK-NEXT:         0
// CHECK-NEXT:     } else {
// CHECK-NEXT:         if [[T_0]].compare(<bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(2)) == 0 {
// CHECK-NEXT:             1
// CHECK-NEXT:         } else {
// CHECK-NEXT:             if [[T_0]].compare(<bitint::BInt<256, 4, 32, bitint::A8>>::from_u128(3)) == 0 {
// CHECK-NEXT:                 2
// CHECK-NEXT:             } else {
// CHECK-NEXT:                 3
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     loop {
// CHECK-NEXT:         match [[T_1]] {
// CHECK-NEXT:             0 => {
// CHECK-NEXT:                 acc += 100;
// CHECK-NEXT:                 [[T_1]] = 1;
// CHECK-NEXT:                 continue;
// CHECK-NEXT:             }
// CHECK-NEXT:             1 => {
// CHECK-NEXT:                 acc += 1;
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:             2 => {
// CHECK-NEXT:                 acc = 999;
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:             3 => {
// CHECK-NEXT:                 acc = -1;
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:             _ => {
// CHECK-NEXT:                 break;
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     acc
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn add256(
// CHECK-NEXT:     mut a: bitint::BInt<256, 4, 32, bitint::A8>,
// CHECK-NEXT:     b: bitint::BInt<256, 4, 32, bitint::A8>,
// CHECK-NEXT: ) -> bitint::BInt<256, 4, 32, bitint::A8> {
// CHECK-NEXT:     return a.wrapping_add(b);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn mul256(
// CHECK-NEXT:     mut a: bitint::BUint<256, 4, 32, bitint::A8>,
// CHECK-NEXT:     b: bitint::BUint<256, 4, 32, bitint::A8>,
// CHECK-NEXT: ) -> bitint::BUint<256, 4, 32, bitint::A8> {
// CHECK-NEXT:     return a.wrapping_mul(b);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// Odd, non-64-aligned widths: exercise BITS that don't line up with a
// CHECK-NEXT: ///
// CHECK-NEXT: /// native Rust integer or a whole number of 64-bit limbs. Values stay well
// CHECK-NEXT: ///
// CHECK-NEXT: /// inside each width's representable range, so wraparound never fires; the
// CHECK-NEXT: ///
// CHECK-NEXT: /// invariants below (undoing +/- and double negation) hold for any width
// CHECK-NEXT: ///
// CHECK-NEXT: /// under two's-complement arithmetic, so they need no per-width hand
// CHECK-NEXT: ///
// CHECK-NEXT: /// computation.
// CHECK-NEXT: fn check_bitint3() {
// CHECK-NEXT:     let x: i8 = 1i8;
// CHECK-NEXT:     let y: i8 = -2i8;
// CHECK-NEXT:     let sum: i8 = (x.wrapping_add(y) as i8) << 5 >> 5;
// CHECK-NEXT:     if (sum.wrapping_sub(y) as i8) << 5 >> 5 != x {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (((x.wrapping_neg() as i8) << 5 >> 5).wrapping_neg() as i8) << 5 >> 5 != x {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if y >= x {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let ux: u8 = 5u8;
// CHECK-NEXT:     let uy: u8 = 2u8;
// CHECK-NEXT:     let usum: u8 = (ux.wrapping_add(uy) as u8) & 7;
// CHECK-NEXT:     if (usum.wrapping_sub(uy) as u8) & 7 != ux {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check_bitint129() {
// CHECK-NEXT:     let mut x: bitint::BInt<129, 3, 24, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<129, 3, 24, bitint::A8>>::from_i128(123456789 as i128);
// CHECK-NEXT:     let mut y: bitint::BInt<129, 3, 24, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<129, 3, 24, bitint::A8>>::from_i128(-987654321 as i128);
// CHECK-NEXT:     let mut s2: bitint::BInt<129, 3, 24, bitint::A8> = x.wrapping_add(y);
// CHECK-NEXT:     if s2.wrapping_sub(y).compare(x) != 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if x.wrapping_neg().wrapping_neg().compare(x) != 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if y.compare(x) >= 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut ux: bitint::BUint<129, 3, 24, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<129, 3, 24, bitint::A8>>::from_bint(
// CHECK-NEXT:             <bitint::BInt<129, 3, 24, bitint::A8>>::from_i128(123456789 as i128),
// CHECK-NEXT:         );
// CHECK-NEXT:     let uy: bitint::BUint<129, 3, 24, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<129, 3, 24, bitint::A8>>::from_bint(
// CHECK-NEXT:             <bitint::BInt<129, 3, 24, bitint::A8>>::from_i128(987654321 as i128),
// CHECK-NEXT:         );
// CHECK-NEXT:     let mut usum: bitint::BUint<129, 3, 24, bitint::A8> = ux.wrapping_add(uy);
// CHECK-NEXT:     if usum.wrapping_sub(uy).compare(ux) != 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn check_bitint512() {
// CHECK-NEXT:     let mut x: bitint::BInt<512, 8, 64, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<512, 8, 64, bitint::A8>>::from_i128(123456789012345 as i128);
// CHECK-NEXT:     let mut y: bitint::BInt<512, 8, 64, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<512, 8, 64, bitint::A8>>::from_i128(-987654321098765 as i128);
// CHECK-NEXT:     let mut s3: bitint::BInt<512, 8, 64, bitint::A8> = x.wrapping_add(y);
// CHECK-NEXT:     if s3.wrapping_sub(y).compare(x) != 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if x.wrapping_neg().wrapping_neg().compare(x) != 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if y.compare(x) >= 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut ux: bitint::BUint<512, 8, 64, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<512, 8, 64, bitint::A8>>::from_u128(123456789012345 as u128);
// CHECK-NEXT:     let uy: bitint::BUint<512, 8, 64, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<512, 8, 64, bitint::A8>>::from_u128(987654321098765 as u128);
// CHECK-NEXT:     let mut usum: bitint::BUint<512, 8, 64, bitint::A8> = ux.wrapping_add(uy);
// CHECK-NEXT:     if usum.wrapping_sub(uy).compare(ux) != 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: /// Decimal literals whose magnitude exceeds u128::MAX (2^128) but still fits
// CHECK-NEXT: ///
// CHECK-NEXT: /// the target _BitInt(N) width, exercising the arbitrary-width decimal-string
// CHECK-NEXT: ///
// CHECK-NEXT: /// parse path instead of the i128/u128 fast paths.
// CHECK-NEXT: fn check_bitint_wide_literal() {
// CHECK-NEXT:     let mut x256: bitint::BInt<256, 4, 32, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<256, 4, 32, bitint::A8>>::from_bint(
// CHECK-NEXT:             <bitint::BInt<198, 4, 32, bitint::A8>>::from_le_bytes::<208, 26>([
// CHECK-NEXT:                 210, 10, 63, 206, 150, 241, 207, 140, 156, 180, 55, 140, 55, 164, 135, 63, 23, 98,
// CHECK-NEXT:                 30, 188, 228, 4, 245, 170, 19, 0,
// CHECK-NEXT:             ]),
// CHECK-NEXT:         );
// CHECK-NEXT:     let mut y256: bitint::BInt<256, 4, 32, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<256, 4, 32, bitint::A8>>::from_bint(
// CHECK-NEXT:             <bitint::BInt<198, 4, 32, bitint::A8>>::from_le_bytes::<208, 26>([
// CHECK-NEXT:                 210, 10, 63, 206, 150, 241, 207, 140, 156, 180, 55, 140, 55, 164, 135, 63, 23, 98,
// CHECK-NEXT:                 30, 188, 228, 4, 245, 170, 19, 0,
// CHECK-NEXT:             ])
// CHECK-NEXT:             .wrapping_neg(),
// CHECK-NEXT:         );
// CHECK-NEXT:     if x256
// CHECK-NEXT:         .wrapping_add(y256)
// CHECK-NEXT:         .compare(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(0 as i128))
// CHECK-NEXT:         != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if x256.wrapping_neg().compare(y256) != 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if y256.compare(x256) >= 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut ux256: bitint::BUint<256, 4, 32, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<256, 4, 32, bitint::A8>>::from_buint(
// CHECK-NEXT:             <bitint::BUint<197, 4, 32, bitint::A8>>::from_le_bytes::<208, 26>([
// CHECK-NEXT:                 210, 10, 63, 206, 150, 241, 207, 140, 156, 180, 55, 140, 55, 164, 135, 63, 23, 98,
// CHECK-NEXT:                 30, 188, 228, 4, 245, 170, 19, 0,
// CHECK-NEXT:             ]),
// CHECK-NEXT:         );
// CHECK-NEXT:     let uy256: bitint::BUint<256, 4, 32, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<256, 4, 32, bitint::A8>>::from_u128(300 as u128);
// CHECK-NEXT:     let mut usum256: bitint::BUint<256, 4, 32, bitint::A8> = ux256.wrapping_add(uy256);
// CHECK-NEXT:     if usum256.wrapping_sub(uy256).compare(ux256) != 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut x512: bitint::BInt<512, 8, 64, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<512, 8, 64, bitint::A8>>::from_bint(
// CHECK-NEXT:             <bitint::BInt<297, 5, 40, bitint::A8>>::from_le_bytes::<304, 38>([
// CHECK-NEXT:                 210, 10, 63, 206, 150, 241, 207, 172, 75, 241, 123, 231, 130, 233, 195, 64, 33, 8,
// CHECK-NEXT:                 215, 205, 158, 24, 11, 164, 143, 195, 150, 106, 88, 231, 93, 89, 13, 215, 23, 62,
// CHECK-NEXT:                 248, 0,
// CHECK-NEXT:             ]),
// CHECK-NEXT:         );
// CHECK-NEXT:     let mut y512: bitint::BInt<512, 8, 64, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<512, 8, 64, bitint::A8>>::from_bint(
// CHECK-NEXT:             <bitint::BInt<297, 5, 40, bitint::A8>>::from_le_bytes::<304, 38>([
// CHECK-NEXT:                 210, 10, 63, 206, 150, 241, 207, 172, 75, 241, 123, 231, 130, 233, 195, 64, 33, 8,
// CHECK-NEXT:                 215, 205, 158, 24, 11, 164, 143, 195, 150, 106, 88, 231, 93, 89, 13, 215, 23, 62,
// CHECK-NEXT:                 248, 0,
// CHECK-NEXT:             ])
// CHECK-NEXT:             .wrapping_neg(),
// CHECK-NEXT:         );
// CHECK-NEXT:     if x512
// CHECK-NEXT:         .wrapping_add(y512)
// CHECK-NEXT:         .compare(<bitint::BInt<512, 8, 64, bitint::A8>>::from_i128(0 as i128))
// CHECK-NEXT:         != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if x512.wrapping_neg().compare(y512) != 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if y512.compare(x512) >= 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut ux512: bitint::BUint<512, 8, 64, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<512, 8, 64, bitint::A8>>::from_buint(
// CHECK-NEXT:             <bitint::BUint<296, 5, 40, bitint::A8>>::from_le_bytes::<304, 38>([
// CHECK-NEXT:                 210, 10, 63, 206, 150, 241, 207, 172, 75, 241, 123, 231, 130, 233, 195, 64, 33, 8,
// CHECK-NEXT:                 215, 205, 158, 24, 11, 164, 143, 195, 150, 106, 88, 231, 93, 89, 13, 215, 23, 62,
// CHECK-NEXT:                 248, 0,
// CHECK-NEXT:             ]),
// CHECK-NEXT:         );
// CHECK-NEXT:     let uy512: bitint::BUint<512, 8, 64, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<512, 8, 64, bitint::A8>>::from_u128(512 as u128);
// CHECK-NEXT:     let mut usum512: bitint::BUint<512, 8, 64, bitint::A8> = ux512.wrapping_add(uy512);
// CHECK-NEXT:     if usum512.wrapping_sub(uy512).compare(ux512) != 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let x128: bitint::BInt<128, 2, 16, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(-12345 as i128);
// CHECK-NEXT:     let y128: bitint::BInt<128, 2, 16, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(100 as i128);
// CHECK-NEXT:     let mut sum128: bitint::BInt<128, 2, 16, bitint::A8> = add128(x128, y128);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), sum128.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let p128: bitint::BUint<128, 2, 16, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<128, 2, 16, bitint::A8>>::from_u128(1000000000000 as u128);
// CHECK-NEXT:     let q128: bitint::BUint<128, 2, 16, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<128, 2, 16, bitint::A8>>::from_bint(
// CHECK-NEXT:             <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(3 as i128),
// CHECK-NEXT:         );
// CHECK-NEXT:     let mut prod128: bitint::BUint<128, 2, 16, bitint::A8> = mul128(p128, q128);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), prod128.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut w: W = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     w.tag = 1;
// CHECK-NEXT:     w.s = <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(-5 as i128);
// CHECK-NEXT:     w.u = <bitint::BUint<128, 2, 16, bitint::A8>>::from_u128(12345678901234567890 as u128);
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: bitint::BInt<128, 2, 16, bitint::A8> = w.s;
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: bitint::BInt<128, 2, 16, bitint::A8> =
// CHECK-NEXT:         [[V_0]].wrapping_add(<bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(
// CHECK-NEXT:             10 as i128,
// CHECK-NEXT:         ));
// CHECK-NEXT:     w.s = [[V_1]];
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: bitint::BUint<128, 2, 16, bitint::A8> = w.u;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: bitint::BUint<128, 2, 16, bitint::A8> =
// CHECK-NEXT:         [[V_2]].wrapping_mul(<bitint::BUint<128, 2, 16, bitint::A8>>::from_bint(
// CHECK-NEXT:             <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(2 as i128),
// CHECK-NEXT:         ));
// CHECK-NEXT:     w.u = [[V_3]];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%zu\n".as_ptr(), 40u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%lld\n".as_ptr(), w.s.to_i128() as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"%llu\n".as_ptr(), w.u.to_u128() as u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut x256: bitint::BInt<256, 4, 32, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(12345 as i128);
// CHECK-NEXT:     let mut y256: bitint::BInt<256, 4, 32, bitint::A8> =
// CHECK-NEXT:         <bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(-300 as i128);
// CHECK-NEXT:     let mut sum256: bitint::BInt<256, 4, 32, bitint::A8> = add256(x256, y256);
// CHECK-NEXT:     if sum256.compare(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(
// CHECK-NEXT:         12045 as i128,
// CHECK-NEXT:     )) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let p256: bitint::BUint<256, 4, 32, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<256, 4, 32, bitint::A8>>::from_u128(1000000000000 as u128);
// CHECK-NEXT:     let q256: bitint::BUint<256, 4, 32, bitint::A8> =
// CHECK-NEXT:         <bitint::BUint<256, 4, 32, bitint::A8>>::from_bint(
// CHECK-NEXT:             <bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(3 as i128),
// CHECK-NEXT:         );
// CHECK-NEXT:     let mut prod256: bitint::BUint<256, 4, 32, bitint::A8> = mul256(p256, q256);
// CHECK-NEXT:     if prod256.compare(<bitint::BUint<256, 4, 32, bitint::A8>>::from_u128(
// CHECK-NEXT:         3000000000000 as u128,
// CHECK-NEXT:     )) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut neg256: bitint::BInt<256, 4, 32, bitint::A8> = x256.wrapping_neg();
// CHECK-NEXT:     if neg256.compare(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(
// CHECK-NEXT:         -12345 as i128,
// CHECK-NEXT:     )) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut diff256: bitint::BInt<256, 4, 32, bitint::A8> = x256.wrapping_sub(y256);
// CHECK-NEXT:     if diff256.compare(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(
// CHECK-NEXT:         12645 as i128,
// CHECK-NEXT:     )) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if y256.compare(x256) >= 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if x256.compare(y256) <= 0 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     check_bitint3();
// CHECK-NEXT:     check_bitint129();
// CHECK-NEXT:     check_bitint512();
// CHECK-NEXT:     check_bitint_wide_literal();
// CHECK-NEXT:     if switch_wide_case_u128(<bitint::BUint<128, 2, 16, bitint::A8>>::from_u128(
// CHECK-NEXT:         200000000000000000000000000000000000000,
// CHECK-NEXT:     )) != 1
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_case_u128(<bitint::BUint<128, 2, 16, bitint::A8>>::from_bint(
// CHECK-NEXT:         <bitint::BInt<128, 2, 16, bitint::A8>>::from_i128(1 as i128),
// CHECK-NEXT:     )) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_selector(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(1 as i128)) != 10 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_selector(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(2 as i128)) != 10 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_selector(<bitint::BInt<256, 4, 32, bitint::A8>>::from_bint(
// CHECK-NEXT:         <bitint::BInt<201, 4, 32, bitint::A8>>::from_le_bytes::<208, 26>([
// CHECK-NEXT:             234, 126, 198, 81, 215, 103, 30, 53, 150, 203, 44, 211, 243, 228, 7, 81, 204, 212, 184,
// CHECK-NEXT:             237, 51, 63, 168, 87, 157, 0,
// CHECK-NEXT:         ])
// CHECK-NEXT:         .wrapping_neg(),
// CHECK-NEXT:     )) != 20
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_selector(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(
// CHECK-NEXT:         350 as i128,
// CHECK-NEXT:     )) != 30
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_selector(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(
// CHECK-NEXT:         300 as i128,
// CHECK-NEXT:     )) != 30
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_selector(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(
// CHECK-NEXT:         400 as i128,
// CHECK-NEXT:     )) != 30
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_selector(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(
// CHECK-NEXT:         999 as i128,
// CHECK-NEXT:     )) != 0
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_selector_unsigned(<bitint::BUint<256, 4, 32, bitint::A8>>::from_buint(
// CHECK-NEXT:         <bitint::BUint<197, 4, 32, bitint::A8>>::from_le_bytes::<208, 26>([
// CHECK-NEXT:             210, 10, 63, 206, 150, 241, 207, 140, 156, 180, 55, 140, 55, 164, 135, 63, 23, 98, 30,
// CHECK-NEXT:             188, 228, 4, 245, 170, 19, 0,
// CHECK-NEXT:         ]),
// CHECK-NEXT:     )) != 1
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_selector_unsigned(<bitint::BUint<256, 4, 32, bitint::A8>>::from_bint(
// CHECK-NEXT:         <bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(0 as i128),
// CHECK-NEXT:     )) != 2
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_selector_unsigned(<bitint::BUint<256, 4, 32, bitint::A8>>::from_bint(
// CHECK-NEXT:         <bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(42 as i128),
// CHECK-NEXT:     )) != 3
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_fallthrough(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(1 as i128)) != 101
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_fallthrough(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(2 as i128)) != 1 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_fallthrough(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(3 as i128)) != 999
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if switch_wide_fallthrough(<bitint::BInt<256, 4, 32, bitint::A8>>::from_i128(4 as i128)) != -1 {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
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
