#include <stdio.h>

typedef unsigned char v2u8 __attribute__((vector_size(2)));
typedef unsigned char v128u8 __attribute__((vector_size(128)));
typedef float v128f __attribute__((vector_size(512)));
typedef int v96i __attribute__((vector_size(384)));

int main(void) {
  v2u8 seed = {3, 250};
  v128u8 spread = __builtin_shufflevector(seed, seed, 0, 1, 2, 3, 1, 0, 3, 2, 0, 0, 1, 1, 2, 2, 3, 3,
      0, 1, 2, 3, 1, 0, 3, 2, 0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 1, 0, 3, 2, 0, 0, 1, 1, 2, 2, 3, 3,
      0, 1, 2, 3, 1, 0, 3, 2, 0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 1, 0, 3, 2, 0, 0, 1, 1, 2, 2, 3, 3,
      0, 1, 2, 3, 1, 0, 3, 2, 0, 0, 1, 1, 2, 2, 3, 3, 0, 1, 2, 3, 1, 0, 3, 2, 0, 0, 1, 1, 2, 2, 3, 3,
      0, 1, 2, 3, 1, 0, 3, 2, 0, 0, 1, 1, 2, 2, 3, 3);
  spread = spread + spread;
  spread = -spread;
  unsigned total = 0;
  for (int i = 0; i < 128; i++)
    total += spread[i] * (i + 1);
  printf("spread %u %d %d\n", total, spread[1], spread[127]);
  v128f f = {};
  for (int i = 0; i < 128; i++)
    f[i] = i * 0.5f;
  f = f * f - f / 2;
  printf("f %g %g\n", f[3], f[127]);
  v96i w = {};
  w[95] = 7;
  w = (w << 2) | 1;
  printf("w %d %d\n", w[0], w[95]);
  v2u8 pair = __builtin_shufflevector(spread, spread, 1, 127);
  printf("pair %d %d\n", pair[0], pair[1]);
  printf("%zu %zu %zu %zu\n", sizeof(v128u8), _Alignof(v128u8), sizeof(v96i), _Alignof(v96i));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: mod __slate_vector {
// CHECK-NEXT:     use core::ops::*;
// CHECK-EMPTY:
// CHECK-NEXT:     #[repr(C)]
// CHECK-NEXT:     pub struct __SlateVector<T, const N: usize, A>(pub [T; N], pub [A; 0]);
// CHECK-EMPTY:
// CHECK-NEXT:     impl<T: Copy, const N: usize, A: Copy> Clone for __SlateVector<T, N, A> {
// CHECK-NEXT:         fn clone(&self) -> Self {
// CHECK-NEXT:             *self
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     impl<T: Copy, const N: usize, A: Copy> Copy for __SlateVector<T, N, A> {}
// CHECK-EMPTY:
// CHECK-NEXT:     macro_rules! alignments {
// CHECK-NEXT:         ($($name:ident = $align:literal),*) => {$(
// CHECK-NEXT:             #[repr(align($align))]
// CHECK-NEXT:             #[derive(Clone, Copy)]
// CHECK-NEXT:             pub struct $name;
// CHECK-NEXT:         )*};
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     alignments!(
// CHECK-NEXT:         Align1 = 1,
// CHECK-NEXT:         Align2 = 2,
// CHECK-NEXT:         Align4 = 4,
// CHECK-NEXT:         Align8 = 8,
// CHECK-NEXT:         Align16 = 16,
// CHECK-NEXT:         Align32 = 32,
// CHECK-NEXT:         Align64 = 64,
// CHECK-NEXT:         Align128 = 128,
// CHECK-NEXT:         Align256 = 256,
// CHECK-NEXT:         Align512 = 512,
// CHECK-NEXT:         Align1024 = 1024,
// CHECK-NEXT:         Align2048 = 2048,
// CHECK-NEXT:         Align4096 = 4096
// CHECK-NEXT:     );
// CHECK-EMPTY:
// CHECK-NEXT:     impl<T: Copy, const N: usize, A: Copy> __SlateVector<T, N, A> {
// CHECK-NEXT:         pub const fn from_array(lanes: [T; N]) -> Self {
// CHECK-NEXT:             Self(lanes, [])
// CHECK-NEXT:         }
// CHECK-EMPTY:
// CHECK-NEXT:         pub const fn splat(lane: T) -> Self {
// CHECK-NEXT:             Self([lane; N], [])
// CHECK-NEXT:         }
// CHECK-EMPTY:
// CHECK-NEXT:         pub const fn to_array(self) -> [T; N] {
// CHECK-NEXT:             self.0
// CHECK-NEXT:         }
// CHECK-EMPTY:
// CHECK-NEXT:         fn map(self, f: impl Fn(T) -> T) -> Self {
// CHECK-NEXT:             Self(self.0.map(f), [])
// CHECK-NEXT:         }
// CHECK-EMPTY:
// CHECK-NEXT:         fn zip(self, other: Self, f: impl Fn(T, T) -> T) -> Self {
// CHECK-NEXT:             let mut lanes = self.0;
// CHECK-NEXT:             for (lane, other) in lanes.iter_mut().zip(other.0) {
// CHECK-NEXT:                 *lane = f(*lane, other);
// CHECK-NEXT:             }
// CHECK-NEXT:             Self(lanes, [])
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     impl<T, const N: usize, A> Index<usize> for __SlateVector<T, N, A> {
// CHECK-NEXT:         type Output = T;
// CHECK-EMPTY:
// CHECK-NEXT:         fn index(&self, index: usize) -> &T {
// CHECK-NEXT:             &self.0[index]
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     impl<T, const N: usize, A> IndexMut<usize> for __SlateVector<T, N, A> {
// CHECK-NEXT:         fn index_mut(&mut self, index: usize) -> &mut T {
// CHECK-NEXT:             &mut self.0[index]
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     macro_rules! binary {
// CHECK-NEXT:         ($t:ty, $($trait:ident $method:ident $assign:ident $assign_method:ident $f:expr),*) => {$(
// CHECK-NEXT:             impl<const N: usize, A: Copy> $trait for __SlateVector<$t, N, A> {
// CHECK-NEXT:                 type Output = Self;
// CHECK-EMPTY:
// CHECK-NEXT:                 fn $method(self, other: Self) -> Self {
// CHECK-NEXT:                     self.zip(other, $f)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-EMPTY:
// CHECK-NEXT:             impl<const N: usize, A: Copy> $assign for __SlateVector<$t, N, A> {
// CHECK-NEXT:                 fn $assign_method(&mut self, other: Self) {
// CHECK-NEXT:                     *self = self.zip(other, $f);
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         )*};
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     macro_rules! integer {
// CHECK-NEXT:         ($($t:ty),*) => {$(
// CHECK-NEXT:             binary!(
// CHECK-NEXT:                 $t,
// CHECK-NEXT:                 Add add AddAssign add_assign <$t>::wrapping_add,
// CHECK-NEXT:                 Sub sub SubAssign sub_assign <$t>::wrapping_sub,
// CHECK-NEXT:                 Mul mul MulAssign mul_assign <$t>::wrapping_mul,
// CHECK-NEXT:                 Div div DivAssign div_assign <$t>::wrapping_div,
// CHECK-NEXT:                 Rem rem RemAssign rem_assign <$t>::wrapping_rem,
// CHECK-NEXT:                 BitAnd bitand BitAndAssign bitand_assign |a: $t, b: $t| a & b,
// CHECK-NEXT:                 BitOr bitor BitOrAssign bitor_assign |a: $t, b: $t| a | b,
// CHECK-NEXT:                 BitXor bitxor BitXorAssign bitxor_assign |a: $t, b: $t| a ^ b,
// CHECK-NEXT:                 Shl shl ShlAssign shl_assign |a: $t, b: $t| a.wrapping_shl(b as u32),
// CHECK-NEXT:                 Shr shr ShrAssign shr_assign |a: $t, b: $t| a.wrapping_shr(b as u32)
// CHECK-NEXT:             );
// CHECK-EMPTY:
// CHECK-NEXT:             impl<const N: usize, A: Copy> Neg for __SlateVector<$t, N, A> {
// CHECK-NEXT:                 type Output = Self;
// CHECK-EMPTY:
// CHECK-NEXT:                 fn neg(self) -> Self {
// CHECK-NEXT:                     self.map(<$t>::wrapping_neg)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-EMPTY:
// CHECK-NEXT:             impl<const N: usize, A: Copy> Not for __SlateVector<$t, N, A> {
// CHECK-NEXT:                 type Output = Self;
// CHECK-EMPTY:
// CHECK-NEXT:                 fn not(self) -> Self {
// CHECK-NEXT:                     self.map(|a| !a)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         )*};
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     macro_rules! float {
// CHECK-NEXT:         ($($t:ty),*) => {$(
// CHECK-NEXT:             binary!(
// CHECK-NEXT:                 $t,
// CHECK-NEXT:                 Add add AddAssign add_assign |a: $t, b: $t| a + b,
// CHECK-NEXT:                 Sub sub SubAssign sub_assign |a: $t, b: $t| a - b,
// CHECK-NEXT:                 Mul mul MulAssign mul_assign |a: $t, b: $t| a * b,
// CHECK-NEXT:                 Div div DivAssign div_assign |a: $t, b: $t| a / b,
// CHECK-NEXT:                 Rem rem RemAssign rem_assign |a: $t, b: $t| a % b
// CHECK-NEXT:             );
// CHECK-EMPTY:
// CHECK-NEXT:             impl<const N: usize, A: Copy> Neg for __SlateVector<$t, N, A> {
// CHECK-NEXT:                 type Output = Self;
// CHECK-EMPTY:
// CHECK-NEXT:                 fn neg(self) -> Self {
// CHECK-NEXT:                     self.map(|a| -a)
// CHECK-NEXT:                 }
// CHECK-NEXT:             }
// CHECK-NEXT:         )*};
// CHECK-NEXT:     }
// CHECK-EMPTY:
// CHECK-NEXT:     integer!(i8, u8, i16, u16, i32, u32, i64, u64, i128, u128);
// CHECK-NEXT:     float!(f32, f64);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut seed: core::simd::Simd<u8, 2> = core::simd::Simd::from_array([3u8, 250u8]);
// CHECK-NEXT:     let mut spread: __slate_vector::__SlateVector<u8, 128, __slate_vector::Align128> = {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = seed.to_array();
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = seed.to_array();
// CHECK-NEXT:         __slate_vector::__SlateVector::from_array(
// CHECK-NEXT:             [
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][0],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_0]][1],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][0],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:                 [[T_1]][1],
// CHECK-NEXT:             ],
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     spread += spread;
// CHECK-NEXT:     spread = -spread;
// CHECK-NEXT:     let mut total: u32 = 0;
// CHECK-NEXT:     for i in 0i32..128i32 {
// CHECK-NEXT:         total = total.wrapping_add(((spread[i as usize] as i32) * (i + 1)) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"spread %u %d %d\n".as_ptr(),
// CHECK-NEXT:             total,
// CHECK-NEXT:             spread[1] as i32,
// CHECK-NEXT:             spread[127] as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut f: __slate_vector::__SlateVector<f32, 128, __slate_vector::Align512> = __slate_vector::__SlateVector::from_array(
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_2:__t[0-9]+]]: [f32; 128] = unsafe { core::mem::zeroed() };
// CHECK-NEXT:             [[T_2]]
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     for i in 0i32..128i32 {
// CHECK-NEXT:         f[i as usize] = (i as f32) * 0.5f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     f = f * f - f / __slate_vector::__SlateVector::splat(2i32 as f32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { slate_support::printf(c"f %g %g\n".as_ptr(), f[3] as f64, f[127] as f64) };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let mut w: __slate_vector::__SlateVector<i32, 96, __slate_vector::Align512> = __slate_vector::__SlateVector::from_array(
// CHECK-NEXT:         {
// CHECK-NEXT:             let mut [[T_3:__t[0-9]+]]: [i32; 96] = unsafe { core::mem::zeroed() };
// CHECK-NEXT:             [[T_3]]
// CHECK-NEXT:         },
// CHECK-NEXT:     );
// CHECK-NEXT:     w[95] = 7;
// CHECK-NEXT:     w = w << __slate_vector::__SlateVector::splat(2i32)
// CHECK-NEXT:         | __slate_vector::__SlateVector::splat(1i32);
// CHECK-NEXT:     println!("w {} {}", w[0], w[95]);
// CHECK-NEXT:     let pair: core::simd::Simd<u8, 2> = {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = spread.to_array();
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = spread.to_array();
// CHECK-NEXT:         core::simd::Simd::from_array([[[T_4]][1], [[T_4]][127]])
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("pair {} {}", pair[0] as i32, pair[1] as i32);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%zu %zu %zu %zu\n".as_ptr(),
// CHECK-NEXT:             128u64,
// CHECK-NEXT:             128u64,
// CHECK-NEXT:             512u64,
// CHECK-NEXT:             512u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
