/* Checks that pure functions are not treated as const.  */

char *p;

static int __attribute__((pure)) is_end_of_statement(void) {
  return *p == '\n' || *p == ';' || *p == '!';
}

void foo(void) {
  /* The is_end_of_statement call was moved out of the loop at one stage,
     resulting in an endless loop.  */
  while (!is_end_of_statement())
    p++;
}

int main(void) {
  p = "abc\n";
  foo();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! Checks that pure functions are not treated as const.
// CHECK-NEXT: static mut p: *mut i8 = unsafe { std::mem::zeroed() };
// CHECK-EMPTY:
// CHECK-NEXT: fn is_end_of_statement() -> i32 {
// CHECK-NEXT:     (((unsafe { *unsafe { p } }) as i32) == (10 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { p } }) as i32) == (59 as i32)
// CHECK-NEXT:         || ((unsafe { *unsafe { p } }) as i32) == (33 as i32)) as i32
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo() {
// CHECK-NEXT:     // The is_end_of_statement call was moved out of the loop at one stage,
// CHECK-NEXT:     // resulting in an endless loop.
// CHECK-NEXT:     '[[SLATE_BREAK_0:__slate_break_[0-9]+]]: while !(is_end_of_statement() != (0 as i32)) {
// CHECK-NEXT:         let mut [[V_0:__v[0-9]+]]: *mut i8 = unsafe { p };
// CHECK-NEXT:         let mut [[V_1:__v[0-9]+]]: *mut i8 = unsafe { [[V_0]].offset((1 as i32) as isize) };
// CHECK-NEXT:         unsafe {
// CHECK-NEXT:             p = [[V_1]];
// CHECK-NEXT:         }
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         p = c"abc\n".as_ptr() as *mut i8;
// CHECK-NEXT:     }
// CHECK-NEXT:     foo();
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
