#include <stdio.h>

typedef unsigned __int128 u128;
typedef __int128 i128;
typedef u128 v2u128 __attribute__((vector_size(32)));
typedef i128 v4i128 __attribute__((vector_size(64)));
typedef unsigned long long v2u64 __attribute__((vector_size(16)));
typedef unsigned char v32u8 __attribute__((vector_size(32)));

struct holder {
  char tag;
  v2u128 lanes;
};

static void show(const char *name, u128 x) {
  printf("%s %016llx%016llx\n", name, (unsigned long long)(x >> 64), (unsigned long long)x);
}

static v2u128 mix(v2u128 a, v2u128 b) {
  v2u128 r = a * b + (a ^ b);
  r -= (v2u128){1, 2};
  r |= a << (v2u128){3, 65};
  return r;
}

int main(void) {
  v2u128 a = {((u128)1 << 100) | 7, ~(u128)0};
  v2u128 b = {3, (u128)5 << 70};
  v2u128 m = mix(a, b);
  show("m0", m[0]);
  show("m1", m[1]);
  v2u128 n = -a;
  show("n0", n[0]);
  show("n1", (~b)[1]);
  show("sh", (a >> (v2u128){1, 127})[1]);
  v4i128 s = {-5, 7, -9, 11};
  s = s / (v4i128){2, -2, 4, 3} + s % (v4i128){3, 3, 5, 5};
  for (int i = 0; i < 4; i++)
    printf("s[%d]=%lld\n", i, (long long)s[i]);
  s[2] = s[0] * s[1];
  printf("s[2]=%lld\n", (long long)s[2]);
  v2u64 narrow = {0xffffffffffffffffull, 3};
  v2u128 wide = __builtin_convertvector(narrow, v2u128);
  show("w0", wide[0] + 1);
  v2u64 back = __builtin_convertvector(b, v2u64);
  printf("back %llx %llx\n", back[0], back[1]);
  v32u8 bytes = (v32u8)a;
  printf("bytes %d %d %d\n", bytes[0], bytes[12], bytes[31]);
  struct holder h = {'x', {9, 10}};
  h.lanes += (v2u128){1, 1};
  show("h1", h.lanes[1]);
  printf("%zu %zu %zu %zu %zu\n", sizeof(v2u128), _Alignof(v2u128), sizeof(v4i128), _Alignof(v4i128),
         sizeof(struct holder));
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
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct holder {
// CHECK-NEXT:     tag: i8,
// CHECK-NEXT:     lanes: __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32>,
// CHECK-NEXT: }
// CHECK-NEXT: fn show(name: *const i8, x: u128) {
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%s %016llx%016llx\n".as_ptr(), name, (x >> 64i32) as u64, x as u64)
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT: }
// CHECK-NEXT: fn mix(
// CHECK-NEXT:     a: __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32>,
// CHECK-NEXT:     b: __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32>,
// CHECK-NEXT: ) -> __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: __slate_vector::__SlateVector<
// CHECK-NEXT:         u128,
// CHECK-NEXT:         2,
// CHECK-NEXT:         __slate_vector::Align32,
// CHECK-NEXT:     > = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: __slate_vector::__SlateVector<
// CHECK-NEXT:         u128,
// CHECK-NEXT:         2,
// CHECK-NEXT:         __slate_vector::Align32,
// CHECK-NEXT:     > = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut r: __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32> = a * b
// CHECK-NEXT:         + (a ^ b);
// CHECK-NEXT:     let [[V_0:__v[0-9]+]]: __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32> = r;
// CHECK-NEXT:     r = [[V_0]]
// CHECK-NEXT:         - unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = __slate_vector::__SlateVector::from_array([
// CHECK-NEXT:                     1 as u128,
// CHECK-NEXT:                     2 as u128,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     let [[V_1:__v[0-9]+]]: __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32> = r;
// CHECK-NEXT:     r = [[V_1]]
// CHECK-NEXT:         | a
// CHECK-NEXT:             << unsafe {
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_1]] = __slate_vector::__SlateVector::from_array([
// CHECK-NEXT:                         3 as u128,
// CHECK-NEXT:                         65 as u128,
// CHECK-NEXT:                     ]);
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:     return r;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: __slate_vector::__SlateVector<
// CHECK-NEXT:         u128,
// CHECK-NEXT:         2,
// CHECK-NEXT:         __slate_vector::Align32,
// CHECK-NEXT:     > = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: __slate_vector::__SlateVector<
// CHECK-NEXT:         i128,
// CHECK-NEXT:         4,
// CHECK-NEXT:         __slate_vector::Align64,
// CHECK-NEXT:     > = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0]]: __slate_vector::__SlateVector<
// CHECK-NEXT:         i128,
// CHECK-NEXT:         4,
// CHECK-NEXT:         __slate_vector::Align64,
// CHECK-NEXT:     > = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_4:__slate_compound_[0-9]+]]: __slate_vector::__SlateVector<
// CHECK-NEXT:         u128,
// CHECK-NEXT:         2,
// CHECK-NEXT:         __slate_vector::Align32,
// CHECK-NEXT:     > = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let a: __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32> = __slate_vector::__SlateVector::from_array([
// CHECK-NEXT:         (1 as u128) << 100i32 | 7,
// CHECK-NEXT:         !(0 as u128),
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut b: __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32> = __slate_vector::__SlateVector::from_array([
// CHECK-NEXT:         3 as u128,
// CHECK-NEXT:         (5 as u128) << 70i32,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let m: __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32> = mix(a, b);
// CHECK-NEXT:     show(c"m0".as_ptr(), m[0]);
// CHECK-NEXT:     show(c"m1".as_ptr(), m[1]);
// CHECK-NEXT:     let n: __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32> = -a;
// CHECK-NEXT:     show(c"n0".as_ptr(), n[0]);
// CHECK-NEXT:     show(c"n1".as_ptr(), (!b)[1]);
// CHECK-NEXT:     show(
// CHECK-NEXT:         c"sh".as_ptr(),
// CHECK-NEXT:         (a
// CHECK-NEXT:             >> unsafe {
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_2]] = __slate_vector::__SlateVector::from_array([
// CHECK-NEXT:                         1 as u128,
// CHECK-NEXT:                         127 as u128,
// CHECK-NEXT:                     ]);
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:                 }
// CHECK-NEXT:             })[1],
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut s: __slate_vector::__SlateVector<i128, 4, __slate_vector::Align64> = __slate_vector::__SlateVector::from_array([
// CHECK-NEXT:         -5 as i128,
// CHECK-NEXT:         7 as i128,
// CHECK-NEXT:         -9 as i128,
// CHECK-NEXT:         11 as i128,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     s = s
// CHECK-NEXT:         / unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_3]] = __slate_vector::__SlateVector::from_array([
// CHECK-NEXT:                     2 as i128,
// CHECK-NEXT:                     -2 as i128,
// CHECK-NEXT:                     4 as i128,
// CHECK-NEXT:                     3 as i128,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         + s
// CHECK-NEXT:             % unsafe {
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_0]] = __slate_vector::__SlateVector::from_array([
// CHECK-NEXT:                         3 as i128,
// CHECK-NEXT:                         3 as i128,
// CHECK-NEXT:                         5 as i128,
// CHECK-NEXT:                         5 as i128,
// CHECK-NEXT:                     ]);
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:     for i in 0i32..4i32 {
// CHECK-NEXT:         let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:         unsafe { printf(c"s[%d]=%lld\n".as_ptr(), i, s[i as usize] as i64) };
// CHECK-NEXT:         unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     }
// CHECK-NEXT:     s[2] = s[0] * s[1];
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"s[2]=%lld\n".as_ptr(), s[2] as i64) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let mut narrow: std::simd::Simd<u64, 2> = std::simd::Simd::from_array([
// CHECK-NEXT:         18446744073709551615u64, 3u64,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut wide: __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32> = __slate_vector::__SlateVector::from_array(
// CHECK-NEXT:         narrow.to_array().map(|__lane| __lane as u128),
// CHECK-NEXT:     );
// CHECK-NEXT:     show(c"w0".as_ptr(), wide[0].wrapping_add(1));
// CHECK-NEXT:     let back: std::simd::Simd<u64, 2> = std::simd::Simd::from_array(
// CHECK-NEXT:         b.to_array().map(|__lane| __lane as u64),
// CHECK-NEXT:     );
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe { printf(c"back %llx %llx\n".as_ptr(), back[0], back[1]) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let bytes: std::simd::Simd<u8, 32> = unsafe {
// CHECK-NEXT:         std::mem::transmute::<
// CHECK-NEXT:             __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32>,
// CHECK-NEXT:             std::simd::Simd<u8, 32>,
// CHECK-NEXT:         >(a)
// CHECK-NEXT:     };
// CHECK-NEXT:     println!(
// CHECK-NEXT:         "bytes {} {} {}", (bytes[0] as u32) as i32, (bytes[12] as u32) as i32, (bytes[31]
// CHECK-NEXT:         as u32) as i32
// CHECK-NEXT:     );
// CHECK-NEXT:     let mut h: holder = {
// CHECK-NEXT:         let mut [[T_0:__t[0-9]+]]: holder = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         [[T_0]].tag = 120i8;
// CHECK-NEXT:         [[T_0]].lanes = __slate_vector::__SlateVector::from_array([9 as u128, 10 as u128]);
// CHECK-NEXT:         [[T_0]]
// CHECK-NEXT:     };
// CHECK-NEXT:     let [[V_2:__v[0-9]+]]: __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32> = h.lanes;
// CHECK-NEXT:     let [[V_3:__v[0-9]+]]: __slate_vector::__SlateVector<u128, 2, __slate_vector::Align32> = [[V_2]]
// CHECK-NEXT:         + unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_4]] = __slate_vector::__SlateVector::from_array([
// CHECK-NEXT:                     1 as u128,
// CHECK-NEXT:                     1 as u128,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_4]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     h.lanes = [[V_3]];
// CHECK-NEXT:     show(c"h1".as_ptr(), h.lanes[1]);
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%zu %zu %zu %zu %zu\n".as_ptr(), 32u64, 32u64, 64u64, 64u64, 64u64)
// CHECK-NEXT:     };
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
