/* PR target/85582 */

#ifdef __SIZEOF_INT128__
typedef __int128          S;
typedef unsigned __int128 U;
#else
typedef long long          S;
typedef unsigned long long U;
#endif

__attribute__((noipa)) U f1(U x, int y) { return x << (y & -2); }

__attribute__((noipa)) S f2(S x, int y) { return x >> (y & -2); }

__attribute__((noipa)) U f3(U x, int y) { return x >> (y & -2); }

int main() {
  U a = (U)1 << (sizeof(U) * __CHAR_BIT__ - 7);
  if (f1(a, 5) != ((U)1 << (sizeof(S) * __CHAR_BIT__ - 3)))
    __builtin_abort();
  S b = (U)0x101 << (sizeof(S) * __CHAR_BIT__ / 2 - 7);
  if (f1(b, sizeof(S) * __CHAR_BIT__ / 2) !=
      (U)0x101 << (sizeof(S) * __CHAR_BIT__ - 7))
    __builtin_abort();
  if (f1(b, sizeof(S) * __CHAR_BIT__ / 2 + 2) !=
      (U)0x101 << (sizeof(S) * __CHAR_BIT__ - 5))
    __builtin_abort();
  S c = (U)1 << (sizeof(S) * __CHAR_BIT__ - 1);
  if ((U)f2(c, 5) != ((U)0x1f << (sizeof(S) * __CHAR_BIT__ - 5)))
    __builtin_abort();
  if ((U)f2(c, sizeof(S) * __CHAR_BIT__ / 2) !=
      ((U)-1 << (sizeof(S) * __CHAR_BIT__ / 2 - 1)))
    __builtin_abort();
  if ((U)f2(c, sizeof(S) * __CHAR_BIT__ / 2 + 2) !=
      ((U)-1 << (sizeof(S) * __CHAR_BIT__ / 2 - 3)))
    __builtin_abort();
  U d = (U)1 << (sizeof(S) * __CHAR_BIT__ - 1);
  if (f3(c, 5) != ((U)0x1 << (sizeof(S) * __CHAR_BIT__ - 5)))
    __builtin_abort();
  if (f3(c, sizeof(S) * __CHAR_BIT__ / 2) !=
      ((U)1 << (sizeof(S) * __CHAR_BIT__ / 2 - 1)))
    __builtin_abort();
  if (f3(c, sizeof(S) * __CHAR_BIT__ / 2 + 2) !=
      ((U)1 << (sizeof(S) * __CHAR_BIT__ / 2 - 3)))
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
// CHECK-NEXT: fn f1(mut x: u128, mut y: i32) -> u128 {
// CHECK-NEXT:     x << (y & -(2 as i32))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2(mut x: i128, mut y: i32) -> i128 {
// CHECK-NEXT:     x >> (y & -(2 as i32))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f3(mut x: u128, mut y: i32) -> u128 {
// CHECK-NEXT:     x >> (y & -(2 as i32))
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: u128 = (((1 as i32) as i128) as u128)
// CHECK-NEXT:         << (16 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((7 as i32) as i64) as u64);
// CHECK-NEXT:     if f1(a, 5 as i32)
// CHECK-NEXT:         != (((1 as i32) as i128) as u128)
// CHECK-NEXT:             << (16 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut b: i128 = ((((257 as i32) as i128) as u128)
// CHECK-NEXT:         << ((16 as u64).wrapping_mul(((8 as i32) as i64) as u64) / (((2 as i32) as i64) as u64))
// CHECK-NEXT:             .wrapping_sub(((7 as i32) as i64) as u64)) as i128;
// CHECK-NEXT:     if f1(
// CHECK-NEXT:         b as u128,
// CHECK-NEXT:         (((16 as u64).wrapping_mul(((8 as i32) as i64) as u64) / (((2 as i32) as i64) as u64))
// CHECK-NEXT:             as u32) as i32,
// CHECK-NEXT:     ) != (((257 as i32) as i128) as u128)
// CHECK-NEXT:         << (16 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((7 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f1(
// CHECK-NEXT:         b as u128,
// CHECK-NEXT:         (((16 as u64).wrapping_mul(((8 as i32) as i64) as u64) / (((2 as i32) as i64) as u64))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64) as u32) as i32,
// CHECK-NEXT:     ) != (((257 as i32) as i128) as u128)
// CHECK-NEXT:         << (16 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut c: i128 = ((((1 as i32) as i128) as u128)
// CHECK-NEXT:         << (16 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((1 as i32) as i64) as u64)) as i128;
// CHECK-NEXT:     if (f2(c, 5 as i32) as u128)
// CHECK-NEXT:         != (((31 as i32) as i128) as u128)
// CHECK-NEXT:             << (16 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (f2(
// CHECK-NEXT:         c,
// CHECK-NEXT:         (((16 as u64).wrapping_mul(((8 as i32) as i64) as u64) / (((2 as i32) as i64) as u64))
// CHECK-NEXT:             as u32) as i32,
// CHECK-NEXT:     ) as u128)
// CHECK-NEXT:         != ((-(1 as i32) as i128) as u128)
// CHECK-NEXT:             << ((16 as u64).wrapping_mul(((8 as i32) as i64) as u64) / (((2 as i32) as i64) as u64))
// CHECK-NEXT:                 .wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if (f2(
// CHECK-NEXT:         c,
// CHECK-NEXT:         (((16 as u64).wrapping_mul(((8 as i32) as i64) as u64) / (((2 as i32) as i64) as u64))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64) as u32) as i32,
// CHECK-NEXT:     ) as u128)
// CHECK-NEXT:         != ((-(1 as i32) as i128) as u128)
// CHECK-NEXT:             << ((16 as u64).wrapping_mul(((8 as i32) as i64) as u64) / (((2 as i32) as i64) as u64))
// CHECK-NEXT:                 .wrapping_sub(((3 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     (((1 as i32) as i128) as u128)
// CHECK-NEXT:         << (16 as u64)
// CHECK-NEXT:             .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:             .wrapping_sub(((1 as i32) as i64) as u64);
// CHECK-NEXT:     if f3(c as u128, 5 as i32)
// CHECK-NEXT:         != (((1 as i32) as i128) as u128)
// CHECK-NEXT:             << (16 as u64)
// CHECK-NEXT:                 .wrapping_mul(((8 as i32) as i64) as u64)
// CHECK-NEXT:                 .wrapping_sub(((5 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f3(
// CHECK-NEXT:         c as u128,
// CHECK-NEXT:         (((16 as u64).wrapping_mul(((8 as i32) as i64) as u64) / (((2 as i32) as i64) as u64))
// CHECK-NEXT:             as u32) as i32,
// CHECK-NEXT:     ) != (((1 as i32) as i128) as u128)
// CHECK-NEXT:         << ((16 as u64).wrapping_mul(((8 as i32) as i64) as u64) / (((2 as i32) as i64) as u64))
// CHECK-NEXT:             .wrapping_sub(((1 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if f3(
// CHECK-NEXT:         c as u128,
// CHECK-NEXT:         (((16 as u64).wrapping_mul(((8 as i32) as i64) as u64) / (((2 as i32) as i64) as u64))
// CHECK-NEXT:             .wrapping_add(((2 as i32) as i64) as u64) as u32) as i32,
// CHECK-NEXT:     ) != (((1 as i32) as i128) as u128)
// CHECK-NEXT:         << ((16 as u64).wrapping_mul(((8 as i32) as i64) as u64) / (((2 as i32) as i64) as u64))
// CHECK-NEXT:             .wrapping_sub(((3 as i32) as i64) as u64)
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
