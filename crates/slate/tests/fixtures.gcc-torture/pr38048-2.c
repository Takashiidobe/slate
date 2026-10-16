extern void abort(void);

static int inv_J(int a[][2]) {
  int i, j;
  int det = 0.0;
  for (j = 0; j < 2; ++j)
    det += a[j][0] + a[j][1];
  return det;
}

int foo() {
  int mat[2][2];
  mat[0][0] = 1;
  mat[0][1] = 2;
  mat[1][0] = 4;
  mat[1][1] = 8;
  return inv_J(mat);
}

int main() {
  if (foo() != 15)
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn inv_J(mut a: *mut [i32; 2]) -> i32 {
// CHECK-NEXT:     let mut j: i32 = 0 as i32;
// CHECK-NEXT:     let mut det: i32 = 0.0f64 as i32;
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while j < (2 as i32) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: i32 = det;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: i32 = [[V_0]]
// CHECK-NEXT:             + ((unsafe {
// CHECK-NEXT:                 *unsafe {
// CHECK-NEXT:                     unsafe {
// CHECK-NEXT:                         (*unsafe { a.offset(j as isize) }).as_mut_ptr() as *mut i32
// CHECK-NEXT:                     }
// CHECK-NEXT:                         .offset((0 as i32) as isize)
// CHECK-NEXT:                 }
// CHECK-NEXT:             })
// CHECK-NEXT:                 + unsafe {
// CHECK-NEXT:                     *unsafe {
// CHECK-NEXT:                         unsafe {
// CHECK-NEXT:                             (*unsafe { a.offset(j as isize) }).as_mut_ptr() as *mut i32
// CHECK-NEXT:                         }
// CHECK-NEXT:                             .offset((1 as i32) as isize)
// CHECK-NEXT:                     }
// CHECK-NEXT:                 });
// CHECK-NEXT:         det = [[V_1]];
// CHECK-NEXT:         let mut [[V_2:__v[0-9]+]]: i32 = j;
// CHECK-NEXT:         let mut [[V_3:__v[0-9]+]]: i32 = [[V_2]] + (1 as i32);
// CHECK-NEXT:         j = [[V_3]];
// CHECK-NEXT:     }
// CHECK-NEXT:     det
// CHECK-NEXT: }
// CHECK-NEXT: fn foo() -> i32 {
// CHECK-NEXT:     let mut mat: __SlateAlign16<{{\[\[}}i32; 2]; 2]> = __SlateAlign16(unsafe {
// CHECK-NEXT:         std::mem::zeroed()
// CHECK-NEXT:     });
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     (mat.0.as_mut_ptr() as *mut [i32; 2]).offset((0 as i32) as isize)
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
// CHECK-NEXT:                     (mat.0.as_mut_ptr() as *mut [i32; 2]).offset((0 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .as_mut_ptr() as *mut i32
// CHECK-NEXT:             }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:         } = 2 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     (mat.0.as_mut_ptr() as *mut [i32; 2]).offset((1 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .as_mut_ptr() as *mut i32
// CHECK-NEXT:             }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:         } = 4 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         *unsafe {
// CHECK-NEXT:             unsafe {
// CHECK-NEXT:                 (*unsafe {
// CHECK-NEXT:                     (mat.0.as_mut_ptr() as *mut [i32; 2]).offset((1 as i32) as isize)
// CHECK-NEXT:                 })
// CHECK-NEXT:                     .as_mut_ptr() as *mut i32
// CHECK-NEXT:             }
// CHECK-NEXT:                 .offset((1 as i32) as isize)
// CHECK-NEXT:         } = 8 as i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     inv_J(mat.0.as_mut_ptr() as *mut [i32; 2])
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if foo() != (15 as i32) {
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
