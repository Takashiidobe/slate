/* PR target/85169 */

typedef char V __attribute__((vector_size(64)));

static void __attribute__((noipa)) foo(V *p) {
  V v   = *p;
  v[63] = 1;
  *p    = v;
}

int main() {
  V v = (V){};
  foo(&v);
  for (unsigned i = 0; i < 64; i++)
    if (v[i] != (i == 63))
      __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![feature(portable_simd)]
// CHECK-NEXT: #![feature(simd_ffi)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR target/85169
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: &mut std::simd::Simd<i8, 64>) {
// CHECK-NEXT:     let mut v: std::simd::Simd<i8, 64> = unsafe { *(p as *mut std::simd::Simd<i8, 64>) };
// CHECK-NEXT:     v[((63 as i32) as usize)] = (1 as i32) as i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *(p as *mut std::simd::Simd<i8, 64>) = v;
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut [[SLATE_COMPOUND_0:__slate_compound_[0-9]+]]: std::simd::Simd<i8, 64> = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut v: std::simd::Simd<i8, 64> = unsafe {
// CHECK-NEXT:         *{
// CHECK-NEXT:             [[SLATE_COMPOUND_0]] = std::simd::Simd::from_array({
// CHECK-NEXT:                 let mut [[T_0:__t[0-9]+]]: [i8; 64] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_0]]
// CHECK-NEXT:             });
// CHECK-NEXT:             std::ptr::addr_of_mut!([[SLATE_COMPOUND_0]])
// CHECK-NEXT:         }
// CHECK-NEXT:     };
// CHECK-NEXT:     foo(unsafe { &mut (*std::ptr::addr_of_mut!(v)) });
// CHECK-NEXT:     let mut i: u32 = (0 as i32) as u32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < ((64 as i32) as u32) {
// CHECK-NEXT:         if (v[(i as usize)] as i32) != ((i == ((63 as i32) as u32)) as i32) {
// CHECK-NEXT:             unsafe { __builtin_abort() };
// CHECK-NEXT:         }
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: u32 = i;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: u32 = [[V_0]].wrapping_add((1 as i32) as u32);
// CHECK-NEXT:         i = [[V_1]];
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
