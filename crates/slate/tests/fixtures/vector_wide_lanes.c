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
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: mod __slate_vector {
// CHECK-NEXT:     use std::ops::*;
// CHECK-NEXT:     #[repr(C)]
// CHECK-NEXT:     pub struct __SlateVector<T, const N: usize, A>(pub [T; N], pub [A; 0]);
// CHECK-NEXT:     impl<T: Copy, const N: usize, A: Copy> Clone for __SlateVector<T, N, A> {
// CHECK-NEXT:         fn clone(&self) -> Self {
// CHECK-NEXT:             *self
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     impl<T: Copy, const N: usize, A: Copy> Copy for __SlateVector<T, N, A> {}
// CHECK-NEXT:     macro_rules! alignments {
// CHECK-NEXT:         ($($name:ident = $align:literal),*) => {
// CHECK-NEXT:             $(#[repr(align($align))] #[derive(Clone, Copy)] pub struct $name;)*
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     alignments!(
// CHECK-NEXT:         Align1 = 1, Align2 = 2, Align4 = 4, Align8 = 8, Align16 = 16, Align32 = 32,
// CHECK-NEXT:         Align64 = 64, Align128 = 128, Align256 = 256, Align512 = 512, Align1024 = 1024,
// CHECK-NEXT:         Align2048 = 2048, Align4096 = 4096
// CHECK-NEXT:     );
// CHECK-NEXT:     impl<T: Copy, const N: usize, A: Copy> __SlateVector<T, N, A> {
// CHECK-NEXT:         pub const fn from_array(lanes: [T; N]) -> Self {
// CHECK-NEXT:             Self(lanes, [])
// CHECK-NEXT:         }
// CHECK-NEXT:         pub const fn splat(lane: T) -> Self {
// CHECK-NEXT:             Self([lane; N], [])
// CHECK-NEXT:         }
// CHECK-NEXT:         pub const fn to_array(self) -> [T; N] {
// CHECK-NEXT:             self.0
// CHECK-NEXT:         }
// CHECK-NEXT:         fn map(self, f: impl Fn(T) -> T) -> Self {
// CHECK-NEXT:             Self(self.0.map(f), [])
// CHECK-NEXT:         }
// CHECK-NEXT:         fn zip(self, other: Self, f: impl Fn(T, T) -> T) -> Self {
// CHECK-NEXT:             let mut lanes = self.0;
// CHECK-NEXT:             for (lane, other) in lanes.iter_mut().zip(other.0) {
// CHECK-NEXT:                 *lane = f(*lane, other);
// CHECK-NEXT:             }
// CHECK-NEXT:             Self(lanes, [])
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     impl<T, const N: usize, A> Index<usize> for __SlateVector<T, N, A> {
// CHECK-NEXT:         type Output = T;
// CHECK-NEXT:         fn index(&self, index: usize) -> &T {
// CHECK-NEXT:             &self.0[index]
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     impl<T, const N: usize, A> IndexMut<usize> for __SlateVector<T, N, A> {
// CHECK-NEXT:         fn index_mut(&mut self, index: usize) -> &mut T {
// CHECK-NEXT:             &mut self.0[index]
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     macro_rules! binary {
// CHECK-NEXT:         (
// CHECK-NEXT:             $t:ty, $($trait:ident $method:ident $assign:ident $assign_method:ident
// CHECK-NEXT:             $f:expr),*
// CHECK-NEXT:         ) => {
// CHECK-NEXT:             $(impl < const N : usize, A : Copy > $trait for __SlateVector <$t, N, A > {
// CHECK-NEXT:             type Output = Self; fn $method (self, other : Self) -> Self { self.zip(other,
// CHECK-NEXT:             $f) } } impl < const N : usize, A : Copy > $assign for __SlateVector <$t, N,
// CHECK-NEXT:             A > { fn $assign_method (& mut self, other : Self) { * self = self.zip(other,
// CHECK-NEXT:             $f); } })*
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     macro_rules! integer {
// CHECK-NEXT:         ($($t:ty),*) => {
// CHECK-NEXT:             $(binary!($t, Add add AddAssign add_assign <$t >::wrapping_add, Sub sub
// CHECK-NEXT:             SubAssign sub_assign <$t >::wrapping_sub, Mul mul MulAssign mul_assign <$t
// CHECK-NEXT:             >::wrapping_mul, Div div DivAssign div_assign <$t >::wrapping_div, Rem rem
// CHECK-NEXT:             RemAssign rem_assign <$t >::wrapping_rem, BitAnd bitand BitAndAssign
// CHECK-NEXT:             bitand_assign | a : $t, b : $t | a & b, BitOr bitor BitOrAssign bitor_assign
// CHECK-NEXT:             | a : $t, b : $t | a | b, BitXor bitxor BitXorAssign bitxor_assign | a : $t,
// CHECK-NEXT:             b : $t | a ^ b, Shl shl ShlAssign shl_assign | a : $t, b : $t | a
// CHECK-NEXT:             .wrapping_shl(b as u32), Shr shr ShrAssign shr_assign | a : $t, b : $t | a
// CHECK-NEXT:             .wrapping_shr(b as u32)); impl < const N : usize, A : Copy > Neg for
// CHECK-NEXT:             __SlateVector <$t, N, A > { type Output = Self; fn neg(self) -> Self { self
// CHECK-NEXT:             .map(<$t >::wrapping_neg) } } impl < const N : usize, A : Copy > Not for
// CHECK-NEXT:             __SlateVector <$t, N, A > { type Output = Self; fn not(self) -> Self { self
// CHECK-NEXT:             .map(| a | ! a) } })*
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     macro_rules! float {
// CHECK-NEXT:         ($($t:ty),*) => {
// CHECK-NEXT:             $(binary!($t, Add add AddAssign add_assign | a : $t, b : $t | a + b, Sub sub
// CHECK-NEXT:             SubAssign sub_assign | a : $t, b : $t | a - b, Mul mul MulAssign mul_assign |
// CHECK-NEXT:             a : $t, b : $t | a * b, Div div DivAssign div_assign | a : $t, b : $t | a /
// CHECK-NEXT:             b, Rem rem RemAssign rem_assign | a : $t, b : $t | a % b); impl < const N :
// CHECK-NEXT:             usize, A : Copy > Neg for __SlateVector <$t, N, A > { type Output = Self; fn
// CHECK-NEXT:             neg(self) -> Self { self.map(| a | - a) } })*
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     integer!(i8, u8, i16, u16, i32, u32, i64, u64, i128, u128);
// CHECK-NEXT:     float!(f32, f64);
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut seed: std::simd::Simd<u8, 2> = std::simd::Simd::from_array([3u8, 250u8]);
// CHECK-NEXT:     let mut spread: __slate_vector::__SlateVector<u8, 128, __slate_vector::Align128> = {
// CHECK-NEXT:         let [[T_0:__t[0-9]+]] = seed.to_array();
// CHECK-NEXT:         let [[T_1:__t[0-9]+]] = seed.to_array();
// CHECK-NEXT:         __slate_vector::__SlateVector::from_array([
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][0],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_0]][1],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][0],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:             [[T_1]][1],
// CHECK-NEXT:         ])
// CHECK-NEXT:     };
// CHECK-NEXT:     spread += spread;
// CHECK-NEXT:     spread = -spread;
// CHECK-NEXT:     let mut total: u32 = 0;
// CHECK-NEXT:     for i in 0i32..128i32 {
// CHECK-NEXT:         total = total
// CHECK-NEXT:             .wrapping_add((((spread[i as usize] as u32) as i32) * (i + 1)) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(
// CHECK-NEXT:             c"spread %u %d %d\n".as_ptr(),
// CHECK-NEXT:             total,
// CHECK-NEXT:             (spread[1] as u32) as i32,
// CHECK-NEXT:             (spread[127] as u32) as i32,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut f: __slate_vector::__SlateVector<f32, 128, __slate_vector::Align512> = __slate_vector::__SlateVector::from_array({
// CHECK-NEXT:         let mut [[T_2:__t[0-9]+]]: [f32; 128] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_2]]
// CHECK-NEXT:     });
// CHECK-NEXT:     for i in 0i32..128i32 {
// CHECK-NEXT:         f[i as usize] = (i as f32) * 0.5f32;
// CHECK-NEXT:     }
// CHECK-NEXT:     f = f * f - f / __slate_vector::__SlateVector::splat(2i32 as f32);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"f %g %g\n".as_ptr(), f[3] as f64, f[127] as f64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut w: __slate_vector::__SlateVector<i32, 96, __slate_vector::Align512> = __slate_vector::__SlateVector::from_array({
// CHECK-NEXT:         let mut [[T_3:__t[0-9]+]]: [i32; 96] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_3]]
// CHECK-NEXT:     });
// CHECK-NEXT:     w[95] = 7;
// CHECK-NEXT:     w = w << __slate_vector::__SlateVector::splat(2i32)
// CHECK-NEXT:         | __slate_vector::__SlateVector::splat(1i32);
// CHECK-NEXT:     println!("w {} {}", w[0], w[95]);
// CHECK-NEXT:     let pair: std::simd::Simd<u8, 2> = {
// CHECK-NEXT:         let [[T_4:__t[0-9]+]] = spread.to_array();
// CHECK-NEXT:         let [[T_5:__t[0-9]+]] = spread.to_array();
// CHECK-NEXT:         std::simd::Simd::from_array([[[T_4]][1], [[T_4]][127]])
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("pair {} {}", (pair[0] as u32) as i32, (pair[1] as u32) as i32);
// CHECK-NEXT:     unsafe { printf(c"%zu %zu %zu %zu\n".as_ptr(), 128u64, 128u64, 512u64, 512u64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
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
