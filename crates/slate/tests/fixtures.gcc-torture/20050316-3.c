/* { dg-options "-Wno-psabi" } */
extern void abort(void);

typedef int          V2SI __attribute__((vector_size(8)));
typedef unsigned int V2USI __attribute__((vector_size(8)));
typedef short        V2HI __attribute__((vector_size(4)));
typedef unsigned int V2UHI __attribute__((vector_size(4)));

V2USI
test1(V2SI x) { return (V2USI)(V2SI)(long long)x; }

long long test2(V2SI x) { return (long long)(V2USI)(V2SI)(long long)x; }

int main(void) {
  if (sizeof(short) != 2 || sizeof(int) != 4 || sizeof(long long) != 8)
    return 0;

  union {
    V2SI      x;
    int       y[2];
    V2USI     z;
    long long l;
  } u;
  V2SI a = {-3, -3};
  u.z    = test1(a);
  if (u.y[0] != -3 || u.y[1] != -3)
    abort();

  u.l = test2(a);
  if (u.y[0] != -3 || u.y[1] != -3)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
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
// CHECK-NEXT: // { dg-options "-Wno-psabi" }
// CHECK-NEXT: fn test1(mut x: std::simd::Simd<i32, 2>) -> std::simd::Simd<u32, 2> {
// CHECK-NEXT:     return unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i32, 2>, std::simd::Simd<u32, 2>>(unsafe {
// CHECK-NEXT:             std::mem::transmute::<i64, std::simd::Simd<i32, 2>>(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<std::simd::Simd<i32, 2>, i64>(x)
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     };
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2(mut x: std::simd::Simd<i32, 2>) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<u32, 2>, i64>(unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i32, 2>, std::simd::Simd<u32, 2>>(unsafe {
// CHECK-NEXT:                 std::mem::transmute::<i64, std::simd::Simd<i32, 2>>(unsafe {
// CHECK-NEXT:                     std::mem::transmute::<std::simd::Simd<i32, 2>, i64>(x)
// CHECK-NEXT:                 })
// CHECK-NEXT:             })
// CHECK-NEXT:         })
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (2 as u64) != (((2 as i32) as i64) as u64)
// CHECK-NEXT:         || (4 as u64) != (((4 as i32) as i64) as u64)
// CHECK-NEXT:         || (8 as u64) != (((8 as i32) as i64) as u64)
// CHECK-NEXT:     {
// CHECK-NEXT:         return 0 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut u: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut a: std::simd::Simd<i32, 2> = std::simd::Simd::from_array([-(3 as i32), -(3 as i32)]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.z = test1(a);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { unsafe { u.y.as_mut_ptr() as *mut i32 }.offset((0 as i32) as isize) } })
// CHECK-NEXT:         != -(3 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { u.y.as_mut_ptr() as *mut i32 }.offset((1 as i32) as isize) }
// CHECK-NEXT:         }) != -(3 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.l = test2(a);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { unsafe { u.y.as_mut_ptr() as *mut i32 }.offset((0 as i32) as isize) } })
// CHECK-NEXT:         != -(3 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { u.y.as_mut_ptr() as *mut i32 }.offset((1 as i32) as isize) }
// CHECK-NEXT:         }) != -(3 as i32)
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
