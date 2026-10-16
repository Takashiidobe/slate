/* PR regression/21897 */
/* This testcase generates MMX instructions together with x87 instructions.
   Currently, there is no "emms" generated to switch between register sets,
   so the testcase fails for targets where MMX insns are enabled.  */
/* { dg-options "-mno-mmx" { target { x86_64-*-* i?86-*-* } } } */

extern void abort(void);

typedef unsigned short v4hi __attribute__((vector_size(8)));
typedef float          v4sf __attribute__((vector_size(16)));

union {
  v4hi  v;
  short s[4];
} u;

union {
  v4sf  v;
  float f[4];
} v;

void foo(void) {
  unsigned int i;
  for (i = 0; i < 2; i++)
    u.v += (v4hi){12, 32768};
  for (i = 0; i < 2; i++)
    v.v += (v4sf){18.0, 20.0, 22};
}

int main(void) {
  foo();
  if (u.s[0] != 24 || u.s[1] != 0 || u.s[2] || u.s[3])
    abort();
  if (v.f[0] != 36.0 || v.f[1] != 40.0 || v.f[2] != 44.0 || v.f[3] != 0.0)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR regression/21897
// CHECK-NEXT: //!
// CHECK-NEXT: //! This testcase generates MMX instructions together with x87 instructions.
// CHECK-NEXT: //! Currently, there is no "emms" generated to switch between register sets,
// CHECK-NEXT: //! so the testcase fails for targets where MMX insns are enabled.
// CHECK-NEXT: //!
// CHECK-NEXT: //! { dg-options "-mno-mmx" { target { x86_64-*-* i?86-*-* } } }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_0:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     v: std::simd::Simd<u16, 4>,
// CHECK-NEXT:     s: [i16; 4],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut u: [[SLATERECORD_0]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: union [[SLATERECORD_1:__SlateRecord[0-9]+]] {
// CHECK-NEXT:     v: std::simd::Simd<f32, 4>,
// CHECK-NEXT:     f: [f32; 4],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut v: [[SLATERECORD_1]] = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<u16, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < ((2 as i32) as u32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: std::simd::Simd<u16, 4> = unsafe { u.v };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: std::simd::Simd<u16, 4> = [[V_0]]
// CHECK-NEXT:             + unsafe {
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array({
// CHECK-NEXT:                         let mut [[T_0:__t[0-9]+]]: [u16; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                         [[T_0]][0] = ((12 as i32) as i16) as u16;
// CHECK-NEXT:                         [[T_0]][1] = ((32768 as i32) as i16) as u16;
// CHECK-NEXT:                         [[T_0]]
// CHECK-NEXT:                     });
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             u.v = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: u32 = [[V_2]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     i = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_1:__slate_break_[0-9]+]]: while i < ((2 as i32) as u32) {
// CHECK-NEXT:         let mut [[V_4:__v[0-9]+]]: std::simd::Simd<f32, 4> = unsafe { v.v };
// CHECK-NEXT:         let mut [[V_5:__v[0-9]+]]: std::simd::Simd<f32, 4> = [[V_4]]
// CHECK-NEXT:             + unsafe {
// CHECK-NEXT:                 *{
// CHECK-NEXT:                     [[SLATE_COMPOUND_1]] = std::simd::Simd::from_array({
// CHECK-NEXT:                         let mut [[T_1:__t[0-9]+]]: [f32; 4] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                         [[T_1]][0] = 18.0f64 as f32;
// CHECK-NEXT:                         [[T_1]][1] = 20.0f64 as f32;
// CHECK-NEXT:                         [[T_1]][2] = (22 as i32) as f32;
// CHECK-NEXT:                         [[T_1]]
// CHECK-NEXT:                     });
// CHECK-NEXT:                     std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             v.v = [[V_5]];
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_6:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_7:__v[0-9]+]]: u32 = [[V_6]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_7]];
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     foo();
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         *unsafe { unsafe { std::ptr::addr_of_mut!(u.s) as *mut i16 }.offset((0 as i32) as isize) }
// CHECK-NEXT:     }) as i32)
// CHECK-NEXT:         != (24 as i32)
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(u.s) as *mut i16 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as i32)
// CHECK-NEXT:             != (0 as i32)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(u.s) as *mut i16 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (0 as i16)
// CHECK-NEXT:         || (unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(u.s) as *mut i16 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) != (0 as i16)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     if ((unsafe {
// CHECK-NEXT:         *unsafe { unsafe { std::ptr::addr_of_mut!(v.f) as *mut f32 }.offset((0 as i32) as isize) }
// CHECK-NEXT:     }) as f64)
// CHECK-NEXT:         != 36.0f64
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(v.f) as *mut f32 }.offset((1 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as f64)
// CHECK-NEXT:             != 40.0f64
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(v.f) as *mut f32 }.offset((2 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as f64)
// CHECK-NEXT:             != 44.0f64
// CHECK-NEXT:         || ((unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe { std::ptr::addr_of_mut!(v.f) as *mut f32 }.offset((3 as i32) as isize)
// CHECK-NEXT:             }
// CHECK-NEXT:         }) as f64)
// CHECK-NEXT:             != 0.0f64
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
