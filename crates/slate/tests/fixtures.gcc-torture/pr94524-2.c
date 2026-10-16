/* PR tree-optimization/94524 */

typedef signed char __attribute__((__vector_size__(16))) V;

static __attribute__((__noinline__, __noclone__)) V foo(V c) {
  c %= (signed char)-128;
  return (V)c;
}

int main() {
  V x = foo((V){-128});
  if (x[0] != 0)
    __builtin_abort();
  x = foo((V){-127});
  if (x[0] != -127)
    __builtin_abort();
  x = foo((V){127});
  if (x[0] != 127)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/94524
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut c: std::simd::Simd<i8, 16>) -> std::simd::Simd<i8, 16> {
// CHECK-NEXT:     let mut [[V_0:__v[0-9]+]]: std::simd::Simd<i8, 16> = c;
// CHECK-NEXT:     let mut [[V_1:__v[0-9]+]]: std::simd::Simd<i8, 16> =
// CHECK-NEXT:         [[V_0]] % std::simd::Simd::splat(((-(128 as i32) as i8) as i32) as i8);
// CHECK-NEXT:     c = [[V_1]];
// CHECK-NEXT:     return c;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<i8, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: std::simd::Simd<i8, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: std::simd::Simd<i8, 16> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut x: std::simd::Simd<i8, 16> = foo(unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array({
// CHECK-NEXT:                 let mut [[T_0:__t[0-9]+]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_0]][0] = -(128 as i32) as i8;
// CHECK-NEXT:                 [[T_0]]
// CHECK-NEXT:             });
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     if (x[((0 as i32) as usize)] as i32) != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     x = foo(unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_1]] = std::simd::Simd::from_array({
// CHECK-NEXT:                 let mut [[T_1:__t[0-9]+]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_1]][0] = -(127 as i32) as i8;
// CHECK-NEXT:                 [[T_1]]
// CHECK-NEXT:             });
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     if (x[((0 as i32) as usize)] as i32) != -(127 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     x = foo(unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_2]] = std::simd::Simd::from_array({
// CHECK-NEXT:                 let mut [[T_2:__t[0-9]+]]: [i8; 16] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_2]][0] = (127 as i32) as i8;
// CHECK-NEXT:                 [[T_2]]
// CHECK-NEXT:             });
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:         }
// CHECK-NEXT:     });
// CHECK-NEXT:     if (x[((0 as i32) as usize)] as i32) != (127 as i32) {
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
