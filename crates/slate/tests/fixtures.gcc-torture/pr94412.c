/* PR middle-end/94412 */

typedef unsigned V __attribute__((__vector_size__(sizeof(unsigned) * 2)));

void foo(V *v, V *w) { *w = -*v / 11; }

void bar(V *v, V *w) { *w = -18 / -*v; }

int main() {
  V a = (V){1, 0};
  V b = (V){3, __INT_MAX__};
  V c, d;
  foo(&a, &c);
  bar(&b, &d);
  if (c[0] != -1U / 11 || c[1] != 0 || d[0] != 0 || d[1] != -18U / -__INT_MAX__)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/94412
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut v: &std::simd::Simd<u32, 2>, mut w: &mut std::simd::Simd<u32, 2>) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(w as *mut std::simd::Simd<u32, 2>) = (std::simd::Simd::splat(0)
// CHECK-NEXT:             - unsafe { *(v as *const std::simd::Simd<u32, 2>) })
// CHECK-NEXT:             / std::simd::Simd::splat((11 as i32) as u32);
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn bar(mut v: &std::simd::Simd<u32, 2>, mut w: &mut std::simd::Simd<u32, 2>) {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(w as *mut std::simd::Simd<u32, 2>) = std::simd::Simd::splat(-(18 as i32) as u32)
// CHECK-NEXT:             / (std::simd::Simd::splat(0) - unsafe { *(v as *const std::simd::Simd<u32, 2>) });
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<u32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: std::simd::Simd<u32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut a: std::simd::Simd<u32, 2> = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] =
// CHECK-NEXT:                 std::simd::Simd::from_array([(1 as i32) as u32, (0 as i32) as u32]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut b: std::simd::Simd<u32, 2> = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_1]] =
// CHECK-NEXT:                 std::simd::Simd::from_array([(3 as i32) as u32, (2147483647 as i32) as u32]);
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut c: std::simd::Simd<u32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut d: std::simd::Simd<u32, 2> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     foo(unsafe { &(*std::ptr::addr_of_mut!(a)) }, unsafe {
// CHECK-NEXT:         &mut (*std::ptr::addr_of_mut!(c))
// CHECK-NEXT:     });
// CHECK-NEXT:     bar(unsafe { &(*std::ptr::addr_of_mut!(b)) }, unsafe {
// CHECK-NEXT:         &mut (*std::ptr::addr_of_mut!(d))
// CHECK-NEXT:     });
// CHECK-NEXT:     if c[((0 as i32) as usize)] != (1 as u32).wrapping_neg() / ((11 as i32) as u32)
// CHECK-NEXT:         || c[((1 as i32) as usize)] != ((0 as i32) as u32)
// CHECK-NEXT:         || d[((0 as i32) as usize)] != ((0 as i32) as u32)
// CHECK-NEXT:         || d[((1 as i32) as usize)] != (18 as u32).wrapping_neg() / (-(2147483647 as i32) as u32)
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
