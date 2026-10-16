/* PR tree-optimization/57321 */

int a = 1, *b, **c;

static int foo(int *p) {
  if (*p == a) {
    int  *i[7][5] = {{0}};
    int **j[1][1];
    j[0][0] = &i[0][0];
    *b      = &p != c;
  }
  return 0;
}

int main() {
  int i = 0;
  foo(&i);
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR tree-optimization/57321
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: static mut a: i32 = 1 as i32;
// CHECK-EMPTY:
// CHECK-NEXT: static mut b: *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: static mut c: *mut *mut i32 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut p: *mut i32) -> i32 {
// CHECK-NEXT:     if (unsafe { *p }) == unsafe { a } {
// CHECK-NEXT:         let mut i: __SlateAlign16<{{\[\[}}*mut i32; 5]; 7]> = __SlateAlign16({
// CHECK-NEXT:             let mut [[T_0:__t[0-9]+]]: {{\[\[}}*mut i32; 5]; 7] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:             [[T_0]][0] = {
// CHECK-NEXT:                 let mut [[T_1:__t[0-9]+]]: [*mut i32; 5] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:                 [[T_1]][0] = std::ptr::null_mut::<i32>();
// CHECK-NEXT:                 [[T_1]]
// CHECK-NEXT:             };
// CHECK-NEXT:             [[T_0]]
// CHECK-NEXT:         });
// CHECK-NEXT:         let mut j: {{\[\[}}*mut *mut i32; 1]; 1] = unsafe { std::mem::zeroed() };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         (j.as_mut_ptr() as *mut [*mut *mut i32; 1]).offset((0 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_mut_ptr() as *mut *mut *mut i32
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:             } = unsafe {
// CHECK-NEXT:                 unsafe {
// CHECK-NEXT:                     (*unsafe {
// CHECK-NEXT:                         (i.0.as_mut_ptr() as *mut [*mut i32; 5]).offset((0 as i32) as isize)
// CHECK-NEXT:                     })
// CHECK-NEXT:                     .as_mut_ptr() as *mut *mut i32
// CHECK-NEXT:                 }
// CHECK-NEXT:                 .offset((0 as i32) as isize)
// CHECK-NEXT:             };
// CHECK-NEXT:         }
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             *unsafe { b } = (std::ptr::addr_of_mut!(p) != unsafe { c }) as i32;
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT:     0 as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut i: i32 = 0 as i32;
// CHECK-NEXT:     foo(std::ptr::addr_of_mut!(i));
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
