/* PR optimization/8988 */
/* Contributed by Kevin Easton */

void foo(char *p1, char **p2) {}

int main(void) {
  char  str[] = "foo { xx }";
  char *ptr   = str + 5;

  foo(ptr, &ptr);

  while (*ptr && (*ptr == 13 || *ptr == 32))
    ptr++;

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR optimization/8988
// CHECK-NEXT: //!
// CHECK-NEXT: //! Contributed by Kevin Easton
// CHECK-NEXT: fn foo(mut p1: *mut i8, mut p2: &*mut i8) {}
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut str: [i8; 11] = [
// CHECK-NEXT:         102 as i8, 111 as i8, 111 as i8, 32 as i8, 123 as i8, 32 as i8, 120 as i8, 120 as i8,
// CHECK-NEXT:         32 as i8, 125 as i8, 0 as i8,
// CHECK-NEXT:     ];
// CHECK-NEXT:     let mut ptr: *mut i8 = unsafe { (str.as_mut_ptr() as *mut i8).offset((5 as i32) as isize) };
// CHECK-NEXT:     foo(ptr, unsafe { &(*std::ptr::addr_of_mut!(ptr)) });
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while (unsafe { *ptr }) != (0 as i8)
// CHECK-NEXT:         && (((unsafe { *ptr }) as i32) == (13 as i32) || ((unsafe { *ptr }) as i32) == (32 as i32))
// CHECK-NEXT:     {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: *mut i8 = ptr;
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut i8 = unsafe { [[V_0]].offset((1 as i32) as isize) };
// CHECK-NEXT:         ptr = [[V_1]];
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
