extern void abort(void);

int foo() {
  int mat[2][1];
  int (*a)[1] = mat;
  int det     = 0;
  int i;
  mat[0][0] = 1;
  mat[1][0] = 2;
  for (i = 0; i < 2; ++i)
    det += a[i][0];
  return det;
}

int main() {
  if (foo() != 3)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     let mut mat: {{\[\[}}i32; 1]; 2] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:     let mut a: *mut [i32; 1] = mat.as_mut_ptr() as *mut [i32; 1];
// CHECK-NEXT:     let mut det: i32 = 0 as i32;
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     (mat.as_mut_ptr() as *mut [i32; 1]).offset((0 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .as_mut_ptr() as *mut i32
// CHECK-NEXT:             }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         } = 1 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     (mat.as_mut_ptr() as *mut [i32; 1]).offset((1 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .as_mut_ptr() as *mut i32
// CHECK-NEXT:             }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         } = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while i < (2 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = det;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]]
// CHECK-NEXT:             + unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe { a.offset(i as isize) }).as_mut_ptr() as *mut i32
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             };
// CHECK-NEXT:         det = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = i;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         i = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     det
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo() != (3 as i32) {
// CHECK-NEXT:         unsafe { abort() };
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// SLATE-FILECHECK-END
