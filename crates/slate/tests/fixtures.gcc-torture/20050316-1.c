/* PR rtl-optimization/16104 */
/* { dg-require-effective-target int32plus } */
/* { dg-options "-Wno-psabi" } */

extern void abort(void);

typedef int          V2SI __attribute__((vector_size(8)));
typedef unsigned int V2USI __attribute__((vector_size(8)));
typedef short        V2HI __attribute__((vector_size(4)));
typedef unsigned int V2UHI __attribute__((vector_size(4)));

int test1(void) { return (long long)(V2SI)0LL; }

int test2(V2SI x) { return (long long)x; }

V2SI test3(void) { return (V2SI)(long long)(int)(V2HI)0; }

V2SI test4(V2HI x) { return (V2SI)(long long)(int)x; }

V2SI test5(V2USI x) { return (V2SI)x; }

int main(void) {
  if (sizeof(short) != 2 || sizeof(int) != 4 || sizeof(long long) != 8)
    return 0;

  if (test1() != 0)
    abort();

  V2SI x = {2, 2};
  if (test2(x) != 2)
    abort();

  union {
    V2SI      x;
    int       y[2];
    V2USI     z;
    long long l;
  } u;
  u.x = test3();
  if (u.y[0] != 0 || u.y[1] != 0)
    abort();

  V2HI y = {4, 4};
  union {
    V2SI      x;
    long long y;
  } v;
  v.x = test4(y);
  if (v.y != 0x40004)
    abort();

  V2USI z = {6, 6};
  u.x     = test5(z);
  if (u.y[0] != 6 || u.y[1] != 6)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR rtl-optimization/16104
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-require-effective-target int32plus }
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-options "-Wno-psabi" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     x: std::simd::Simd<i32, 2>,
// CHECK-NEXT:     y: [i32; 2],
// CHECK-NEXT:     z: std::simd::Simd<u32, 2>,
// CHECK-NEXT:     l: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     x: std::simd::Simd<i32, 2>,
// CHECK-NEXT:     y: i64,
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test1() -> i32 {
// CHECK-NEXT:     (unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i32, 2>, i64>(unsafe {
// CHECK-NEXT:             std::mem::transmute::<i64, std::simd::Simd<i32, 2>>(0 as i64)
// CHECK-NEXT:         })
// CHECK-NEXT:     }) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2(mut x: std::simd::Simd<i32, 2>) -> i32 {
// CHECK-NEXT:     (unsafe { std::mem::transmute::<std::simd::Simd<i32, 2>, i64>(x) }) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3() -> std::simd::Simd<i32, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<i64, std::simd::Simd<i32, 2>>(
// CHECK-NEXT:             (unsafe {
// CHECK-NEXT:                 std::mem::transmute::<std::simd::Simd<i16, 2>, i32>(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<i32, std::simd::Simd<i16, 2>>(0 as i32)
// CHECK-NEXT:                 })
// CHECK-NEXT:             }) as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test4(mut x: std::simd::Simd<i16, 2>) -> std::simd::Simd<i32, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<i64, std::simd::Simd<i32, 2>>(
// CHECK-NEXT:             (unsafe { std::mem::transmute::<std::simd::Simd<i16, 2>, i32>(x) }) as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test5(mut x: std::simd::Simd<u32, 2>) -> std::simd::Simd<i32, 2> {
// CHECK-NEXT:     return unsafe { std::mem::transmute::<std::simd::Simd<u32, 2>, std::simd::Simd<i32, 2>>(x) };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (2 as u64) != (((2 as i32) as i64) as u64)
// CHECK-NEXT:         || (4 as u64) != (((4 as i32) as i64) as u64)
// CHECK-NEXT:         || (8 as u64) != (((8 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     if test1() != (0 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut x: std::simd::Simd<i32, 2> = std::simd::Simd::from_array([2 as i32, 2 as i32]);
// CHECK-NEXT:     if test2(x) != (2 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut u: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.x = test3();
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { unsafe { u.y.as_mut_ptr() as *mut i32 }.offset((0 as i32) as isize) } })
// CHECK-NEXT:         != (0 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { u.y.as_mut_ptr() as *mut i32 }.offset((1 as i32) as isize) }
// CHECK-NEXT:         }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut y: std::simd::Simd<i16, 2> =
// CHECK-NEXT:         std::simd::Simd::from_array([(4 as i32) as i16, (4 as i32) as i16]);
// CHECK-NEXT:     let mut v: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         v.x = test4(y);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { v.y }) != ((262148 as i32) as i64) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut z: std::simd::Simd<u32, 2> =
// CHECK-NEXT:         std::simd::Simd::from_array([(6 as i32) as u32, (6 as i32) as u32]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.x = test5(z);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { unsafe { u.y.as_mut_ptr() as *mut i32 }.offset((0 as i32) as isize) } })
// CHECK-NEXT:         != (6 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { u.y.as_mut_ptr() as *mut i32 }.offset((1 as i32) as isize) }
// CHECK-NEXT:         }) != (6 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
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
