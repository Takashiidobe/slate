/* PR tree-optimization/60960 */

typedef unsigned char v4qi __attribute__((vector_size(4)));

__attribute__((noinline, noclone)) v4qi f1(v4qi v) { return v / 2; }

__attribute__((noinline, noclone)) v4qi f2(v4qi v) {
  return v / (v4qi){2, 2, 2, 2};
}

__attribute__((noinline, noclone)) v4qi f3(v4qi x, v4qi y) { return x / y; }

int main() {
  v4qi x = {5, 5, 5, 5};
  v4qi y = {2, 2, 2, 2};
  v4qi z = f1(x);
  if (__builtin_memcmp(&y, &z, sizeof(y)) != 0)
    __builtin_abort();
  z = f2(x);
  if (__builtin_memcmp(&y, &z, sizeof(y)) != 0)
    __builtin_abort();
  z = f3(x, y);
  if (__builtin_memcmp(&y, &z, sizeof(y)) != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/60960
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f1(mut v: std::simd::Simd<u8, 4>) -> std::simd::Simd<u8, 4> {
// CHECK-NEXT:     return v / std::simd::Simd::splat(((2 as i32) as i8) as u8);
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f2(mut v: std::simd::Simd<u8, 4>) -> std::simd::Simd<u8, 4> {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<u8, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     return v / unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                 ((2 as i32) as i8) as u8,
// CHECK-NEXT:                 ((2 as i32) as i8) as u8,
// CHECK-NEXT:                 ((2 as i32) as i8) as u8,
// CHECK-NEXT:                 ((2 as i32) as i8) as u8,
// CHECK-NEXT:             ]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn f3(mut x: std::simd::Simd<u8, 4>, mut y: std::simd::Simd<u8, 4>) -> std::simd::Simd<u8, 4> {
// CHECK-NEXT:     return x / y;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut x: std::simd::Simd<u8, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut y: std::simd::Simd<u8, 4> = std::simd::Simd::from_array([
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut z: std::simd::Simd<u8, 4> = f1(x);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             std::ptr::addr_of_mut!(y) as *const (),
// CHECK-NEXT:             std::ptr::addr_of_mut!(z) as *const (),
// CHECK-NEXT:             4 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     z = f2(x);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             std::ptr::addr_of_mut!(y) as *const (),
// CHECK-NEXT:             std::ptr::addr_of_mut!(z) as *const (),
// CHECK-NEXT:             4 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     z = f3(x, y);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             std::ptr::addr_of_mut!(y) as *const (),
// CHECK-NEXT:             std::ptr::addr_of_mut!(z) as *const (),
// CHECK-NEXT:             4 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
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
