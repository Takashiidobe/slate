/* PR c/8032 */
/* Verify that an empty initializer inside a partial
   parent initializer doesn't confuse GCC.  */

void abort(void);

struct X {
  int a;
  int b;
  int z[];
};

struct X x = {.b = 40, .z = {}};

int main() {
  if (x.b != 40)
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR c/8032
// CHECK-NEXT: //!
// CHECK-NEXT: //! Verify that an empty initializer inside a partial
// CHECK-NEXT: //! parent initializer doesn't confuse GCC.
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C)]
// CHECK-NEXT: #[derive(Clone, Copy)]
// CHECK-NEXT: struct X {
// CHECK-NEXT:     a: i32,
// CHECK-NEXT:     b: i32,
// CHECK-NEXT:     z: [i32; 0],
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: static mut x: X = {
// CHECK-NEXT:     let mut [[T_0:__t[0-9]+]]: X = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     [[T_0]].b = 40 as i32;
// CHECK-NEXT:     [[T_0]].z = [];
// CHECK-NEXT:     [[T_0]]
// CHECK-NEXT: };
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (unsafe { x.b }) != (40 as i32) {
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
