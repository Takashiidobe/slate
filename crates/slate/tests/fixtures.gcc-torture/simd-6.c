extern void abort(void);
extern int  memcmp(const void *, const void *, __SIZE_TYPE__);

typedef unsigned char v8qi __attribute__((vector_size(8)));

v8qi foo(v8qi x, v8qi y) { return x * y; }

int main() {
  v8qi a = {1, 2, 3, 4, 5, 6, 7, 8};
  v8qi b = {3, 3, 3, 3, 3, 3, 3, 3};
  v8qi c = {3, 6, 9, 12, 15, 18, 21, 24};
  v8qi r;

  r = foo(a, b);
  if (memcmp(&r, &c, 8) != 0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT:     fn memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn foo(
// CHECK-NEXT:     mut x: std::simd::Simd<u8, 8>,
// CHECK-NEXT:     mut y: std::simd::Simd<u8, 8>,
// CHECK-NEXT: ) -> std::simd::Simd<u8, 8> {
// CHECK-NEXT:     return x * y;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: std::simd::Simd<u8, 8> = std::simd::Simd::from_array([
// CHECK-NEXT:         ((1 as i32) as i8) as u8,
// CHECK-NEXT:         ((2 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((4 as i32) as i8) as u8,
// CHECK-NEXT:         ((5 as i32) as i8) as u8,
// CHECK-NEXT:         ((6 as i32) as i8) as u8,
// CHECK-NEXT:         ((7 as i32) as i8) as u8,
// CHECK-NEXT:         ((8 as i32) as i8) as u8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut b: std::simd::Simd<u8, 8> = std::simd::Simd::from_array([
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut c: std::simd::Simd<u8, 8> = std::simd::Simd::from_array([
// CHECK-NEXT:         ((3 as i32) as i8) as u8,
// CHECK-NEXT:         ((6 as i32) as i8) as u8,
// CHECK-NEXT:         ((9 as i32) as i8) as u8,
// CHECK-NEXT:         ((12 as i32) as i8) as u8,
// CHECK-NEXT:         ((15 as i32) as i8) as u8,
// CHECK-NEXT:         ((18 as i32) as i8) as u8,
// CHECK-NEXT:         ((21 as i32) as i8) as u8,
// CHECK-NEXT:         ((24 as i32) as i8) as u8,
// CHECK-NEXT:     ]);
// CHECK-NEXT:     let mut r: std::simd::Simd<u8, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     r = foo(a, b);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         memcmp(
// CHECK-NEXT:             std::ptr::addr_of_mut!(r) as *const (),
// CHECK-NEXT:             std::ptr::addr_of_mut!(c) as *const (),
// CHECK-NEXT:             ((8 as i32) as i64) as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
