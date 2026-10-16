/* PR middle-end/111151 */

int main() {
  unsigned a = (1U + __INT_MAX__) / 2U;
  unsigned b = 1U;
  unsigned c = (a * 2U > b * 2U ? a * 2U : b * 2U) * 2U;
  if (c != 0U)
    __builtin_abort();
  int d = (-__INT_MAX__ - 1) / 2;
  int e = 10;
  int f = (d * 2 > e * 5 ? d * 2 : e * 5) * 6;
  if (f != 300)
    __builtin_abort();
  int g = (-__INT_MAX__ - 1) / 2;
  int h = 0;
  int i = (g * 2 > h * 5 ? g * 2 : h * 5) / -1;
  if (i != 0)
    __builtin_abort();
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/111151
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: u32 = (1 as u32).wrapping_add((2147483647 as i32) as u32) / (2 as u32);
// CHECK-NEXT:     let mut b: u32 = 1 as u32;
// CHECK-NEXT:     let mut c: u32 = if a.wrapping_mul(2 as u32) > b.wrapping_mul(2 as u32) {
// CHECK-NEXT:         a.wrapping_mul(2 as u32)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         b.wrapping_mul(2 as u32)
// CHECK-NEXT:     }
// CHECK-NEXT:     .wrapping_mul(2 as u32);
// CHECK-NEXT:     if c != (0 as u32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut d: i32 = (-(2147483647 as i32) - (1 as i32)) / (2 as i32);
// CHECK-NEXT:     let mut e: i32 = 10 as i32;
// CHECK-NEXT:     let mut f: i32 = (if d * (2 as i32) > e * (5 as i32) {
// CHECK-NEXT:         d * (2 as i32)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         e * (5 as i32)
// CHECK-NEXT:     }) * (6 as i32);
// CHECK-NEXT:     if f != (300 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut g: i32 = (-(2147483647 as i32) - (1 as i32)) / (2 as i32);
// CHECK-NEXT:     let mut h: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = (if g * (2 as i32) > h * (5 as i32) {
// CHECK-NEXT:         g * (2 as i32)
// CHECK-NEXT:     } else {
// CHECK-NEXT:         h * (5 as i32)
// CHECK-NEXT:     }) / -(1 as i32);
// CHECK-NEXT:     if i != (0 as i32) {
// CHECK-NEXT:         unsafe { __builtin_abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0
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
