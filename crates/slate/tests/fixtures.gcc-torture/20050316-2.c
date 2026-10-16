/* This testcase generates MMX instructions together with x87 instructions.
   Currently, there is no "emms" generated to switch between register sets,
   so the testcase fails for targets where MMX insns are enabled.  */
/* { dg-options "-mno-mmx -Wno-psabi" { target { x86_64-*-* i?86-*-* } } } */

extern void abort(void);

typedef int          V2SI __attribute__((vector_size(8)));
typedef unsigned int V2USI __attribute__((vector_size(8)));
typedef float        V2SF __attribute__((vector_size(8)));
typedef short        V2HI __attribute__((vector_size(4)));
typedef unsigned int V2UHI __attribute__((vector_size(4)));

long long test1(V2SF x) { return (long long)(V2SI)x; }

long long test2(V2SF x) { return (long long)x; }

long long test3(V2SI x) { return (long long)(V2SF)x; }

int main(void) {
  if (sizeof(short) != 2 || sizeof(int) != 4 || sizeof(long long) != 8)
    return 0;

  V2SF x = {2.0, 2.0};
  union {
    long long l;
    float     f[2];
    int       i[2];
  } u;
  u.l = test1(x);
  if (u.f[0] != 2.0 || u.f[1] != 2.0)
    abort();

  V2SF y = {6.0, 6.0};
  u.l    = test2(y);
  if (u.f[0] != 6.0 || u.f[1] != 6.0)
    abort();

  V2SI z = {4, 4};
  u.l    = test3(z);
  if (u.i[0] != 4 || u.i[1] != 4)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! This testcase generates MMX instructions together with x87 instructions.
// CHECK-NEXT: //! Currently, there is no "emms" generated to switch between register sets,
// CHECK-NEXT: //! so the testcase fails for targets where MMX insns are enabled.
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-options "-mno-mmx -Wno-psabi" { target { x86_64-*-* i?86-*-* } } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     l: i64,
// CHECK-NEXT:     f: [f32; 2],
// CHECK-NEXT:     i: [i32; 2],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test1(mut x: std::simd::Simd<f32, 2>) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<i32, 2>, i64>(unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<f32, 2>, std::simd::Simd<i32, 2>>(x)
// CHECK-NEXT:         })
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test2(mut x: std::simd::Simd<f32, 2>) -> i64 {
// CHECK-NEXT:     unsafe { std::mem::transmute::<std::simd::Simd<f32, 2>, i64>(x) }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn test3(mut x: std::simd::Simd<i32, 2>) -> i64 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         std::mem::transmute::<std::simd::Simd<f32, 2>, i64>(unsafe {
// CHECK-NEXT:             std::mem::transmute::<std::simd::Simd<i32, 2>, std::simd::Simd<f32, 2>>(x)
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
// CHECK-NEXT:     let mut x: std::simd::Simd<f32, 2> =
// CHECK-NEXT:         std::simd::Simd::from_array([2.0f64 as f32, 2.0f64 as f32]);
// CHECK-NEXT:     let mut u: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.l = test1(x);
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { *unsafe { unsafe { u.f.as_mut_ptr() as *mut f32 }.offset((0 as i32) as isize) } })
// CHECK-NEXT:         as f64)
// CHECK-NEXT:         != 2.0f64
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { u.f.as_mut_ptr() as *mut f32 }.offset((1 as i32) as isize) }
// CHECK-NEXT:         }) as f64)
// CHECK-NEXT:             != 2.0f64
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut y: std::simd::Simd<f32, 2> =
// CHECK-NEXT:         std::simd::Simd::from_array([6.0f64 as f32, 6.0f64 as f32]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.l = test2(y);
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe { *unsafe { unsafe { u.f.as_mut_ptr() as *mut f32 }.offset((0 as i32) as isize) } })
// CHECK-NEXT:         as f64)
// CHECK-NEXT:         != 6.0f64
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe { unsafe { u.f.as_mut_ptr() as *mut f32 }.offset((1 as i32) as isize) }
// CHECK-NEXT:         }) as f64)
// CHECK-NEXT:             != 6.0f64
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut z: std::simd::Simd<i32, 2> = std::simd::Simd::from_array([4 as i32, 4 as i32]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         u.l = test3(z);
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { *unsafe { unsafe { u.i.as_mut_ptr() as *mut i32 }.offset((0 as i32) as isize) } })
// CHECK-NEXT:         != (4 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe { unsafe { u.i.as_mut_ptr() as *mut i32 }.offset((1 as i32) as isize) }
// CHECK-NEXT:         }) != (4 as i32)
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
