/* PR tree-optimization/66187 */

int   a = 1, e = -1;
short b, f;

int main() {
  f     = e;
  int g = b < 0 ? 0 : f + b;
  if ((g & -4) < 0)
    a = 0;
  if (a)
    __builtin_abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/66187
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     #[link_name = "abort"]
// CHECK-NEXT:     fn __builtin_abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut e: i32 = -(1 as i32);
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut f: i16 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         f = (unsafe { e }) as i16;
// CHECK-NEXT:     }
// CHECK-NEXT:     let mut g: i32 = if ((unsafe { b }) as i32) < (0 as i32) {
// CHECK-NEXT:         0 as i32
// CHECK-NEXT:     } else {
// CHECK-NEXT:         ((unsafe { f }) as i32) + ((unsafe { b }) as i32)
// CHECK-NEXT:     };
// CHECK-NEXT:     if g & -(4 as i32) < (0 as i32) {
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             a = 0 as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     if (unsafe { a }) != (0 as i32) {
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
