/* PR c/7102 */

/* Verify that GCC zero-extends integer constants
   in unsigned binary operations. */

void abort(void);

typedef unsigned char u8;

u8 fun(u8 y) {
  u8 x = ((u8)255) / y;
  return x;
}

int main(void) {
  if (fun((u8)2) != 127)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/7102
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: // Verify that GCC zero-extends integer constants
// CHECK-NEXT: // in unsigned binary operations.
// CHECK-NEXT: fn fun(mut y: u8) -> u8 {
// CHECK-NEXT:     let mut x: u8 =
// CHECK-NEXT:         (((((((255 as i32) as i8) as u8) as u32) as i32) / ((y as u32) as i32)) as i8) as u8;
// CHECK-NEXT:     x
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if ((fun(((2 as i32) as i8) as u8) as u32) as i32) != (127 as i32) {
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
