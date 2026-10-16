/* PR target/85582 */

#ifdef __SIZEOF_INT128__
typedef __int128          S;
typedef unsigned __int128 U;
#else
typedef long long          S;
typedef unsigned long long U;
#endif

__attribute__((noipa)) S f1(S x, int y) {
  x  = x << (y & 5);
  x += y;
  return x;
}

__attribute__((noipa)) S f2(S x, int y) {
  x  = x >> (y & 5);
  x += y;
  return x;
}

__attribute__((noipa)) U f3(U x, int y) {
  x  = x >> (y & 5);
  x += y;
  return x;
}

int main() {
  S a = (S)1 << (sizeof(S) * __CHAR_BIT__ - 7);
  S b = f1(a, 12);
  if (b != ((S)1 << (sizeof(S) * __CHAR_BIT__ - 3)) + 12)
    __builtin_abort();
  S c = (U)1 << (sizeof(S) * __CHAR_BIT__ - 1);
  S d = f2(c, 12);
  if ((U)d != ((U)0x1f << (sizeof(S) * __CHAR_BIT__ - 5)) + 12)
    __builtin_abort();
  U e = (U)1 << (sizeof(U) * __CHAR_BIT__ - 1);
  U f = f3(c, 12);
  if (f != ((U)1 << (sizeof(U) * __CHAR_BIT__ - 5)) + 12)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/85582
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f1(mut x: i128, mut y: i32) -> i128 {
// CHECK-NEXT:     x <<= y & (5 as i32);
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: i128 = x;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: i128 = [[V_0]] + (y as i128);
// CHECK-NEXT:     x = [[V_1]];
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2(mut x: i128, mut y: i32) -> i128 {
// CHECK-NEXT:     x >>= y & (5 as i32);
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: i128 = x;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: i128 = [[V_2]] + (y as i128);
// CHECK-NEXT:     x = [[V_3]];
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f3(mut x: u128, mut y: i32) -> u128 {
// CHECK-NEXT:     x >>= y & (5 as i32);
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: u128 = x;
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: u128 = [[V_4]].wrapping_add((y as i128) as u128);
// CHECK-NEXT:     x = [[V_5]];
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: i128 = ((1 as i32) as i128)
// CHECK-NEXT:         << (16 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((7 as i32) as i64) as u64);
// CHECK-NEXT:     let mut b: i128 = f1(a, 12 as i32);
// CHECK-NEXT:     if b != (((1 as i32) as i128)
// CHECK-NEXT:         << (16 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((3 as i32) as i64) as u64))
// CHECK-NEXT:         + ((12 as i32) as i128)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut c: i128 = ((((1 as i32) as i128) as u128)
// CHECK-NEXT:         << (16 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((1 as i32) as i64) as u64)) as i128;
// CHECK-NEXT:     let mut d: i128 = f2(c, 12 as i32);
// CHECK-NEXT:     if (d as u128)
// CHECK-NEXT:         != ((((31 as i32) as i128) as u128)
// CHECK-NEXT:             << (16 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((5 as i32) as i64) as u64))
// CHECK-NEXT:         .wrapping_add(((12 as i32) as i128) as u128)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     (((1 as i32) as i128) as u128)
// CHECK-NEXT:         << (16 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((1 as i32) as i64) as u64);
// CHECK-NEXT:     let mut f: u128 = f3(c as u128, 12 as i32);
// CHECK-NEXT:     if f != ((((1 as i32) as i128) as u128)
// CHECK-NEXT:         << (16 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((5 as i32) as i64) as u64))
// CHECK-NEXT:     .wrapping_add(((12 as i32) as i128) as u128)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
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
// SLATE-FILECHECK-END
