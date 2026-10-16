/* PR tree-optimization/65427 */

typedef int V __attribute__((vector_size(8 * sizeof(int))));
V           a, b, c, d, e, f;

__attribute__((noinline, noclone)) void foo(int x, int y) {
  do {
    if (x)
      d = a ^ c;
    else
      d = a ^ b;
  } while (y);
}

int main() {
  a = (V){1, 2, 3, 4, 5, 6, 7, 8};
  b = (V){0x40, 0x80, 0x40, 0x80, 0x40, 0x80, 0x40, 0x80};
  e = (V){0x41, 0x82, 0x43, 0x84, 0x45, 0x86, 0x47, 0x88};
  foo(0, 0);
  if (__builtin_memcmp(&d, &e, sizeof(V)) != 0)
    __builtin_abort();
  c = (V){0x80, 0x40, 0x80, 0x40, 0x80, 0x40, 0x80, 0x40};
  f = (V){0x81, 0x42, 0x83, 0x44, 0x85, 0x46, 0x87, 0x48};
  foo(1, 0);
  if (__builtin_memcmp(&d, &f, sizeof(V)) != 0)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/65427
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "memcmp"]
// CHECK-NEXT:     fn __builtin_memcmp([[V_0:__v[0-9]+]]: *const (), [[V_1:__v[0-9]+]]: *const (), [[V_2:__v[0-9]+]]: u64) -> i32;
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut d: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut x: i32, mut y: i32) {
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: loop {
// CHECK-NEXT:         if x != (0 as i32) {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 d = (unsafe { a }) ^ unsafe { c };
// CHECK-NEXT:             }
// CHECK-NEXT:         } else {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 d = (unsafe { a }) ^ unsafe { b };
// CHECK-NEXT:             }
// CHECK-NEXT:         }
// CHECK-NEXT:         if !(y != (0 as i32)) {
// CHECK-NEXT:             break;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_1:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_2:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_3:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_4:__slate_compound_[0-9]+]]: std::simd::Simd<i32, 8> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         a = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32, 8 as i32,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         b = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_1]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     64 as i32, 128 as i32, 64 as i32, 128 as i32, 64 as i32, 128 as i32, 64 as i32,
// CHECK-NEXT:                     128 as i32,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_1]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         e = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_2]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     65 as i32, 130 as i32, 67 as i32, 132 as i32, 69 as i32, 134 as i32, 71 as i32,
// CHECK-NEXT:                     136 as i32,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_2]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(0 as i32, 0 as i32);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(d) }) as *const (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(e) }) as *const (),
// CHECK-NEXT:             32 as u64,
// CHECK-NEXT:         )
// CHECK-NEXT:     }) != (0 as i32)
// CHECK-NEXT:     {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         c = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_3]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     128 as i32, 64 as i32, 128 as i32, 64 as i32, 128 as i32, 64 as i32,
// CHECK-NEXT:                     128 as i32, 64 as i32,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_3]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f = unsafe {
// CHECK-NEXT:             *{
// CHECK-NEXT:                 [[SLATE_COMPOUND_4]] = std::simd::Simd::from_array([
// CHECK-NEXT:                     129 as i32, 66 as i32, 131 as i32, 68 as i32, 133 as i32, 70 as i32,
// CHECK-NEXT:                     135 as i32, 72 as i32,
// CHECK-NEXT:                 ]);
// CHECK-NEXT:                 std::ptr::addr_of_mut!([[SLATE_COMPOUND_4]])
// CHECK-NEXT:             }
// CHECK-NEXT:         };
// CHECK-NEXT:     }
// CHECK-NEXT:     foo(1 as i32, 0 as i32);
// CHECK-NEXT:     if (unsafe {
// CHECK-NEXT:         __builtin_memcmp(
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(d) }) as *const (),
// CHECK-NEXT:             (unsafe { std::ptr::addr_of_mut!(f) }) as *const (),
// CHECK-NEXT:             32 as u64,
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
