/* PR middle-end/30473 */

extern int  sprintf(char *, const char *, ...);
extern void abort(void);

char *foo(char *buf, char *p) {
  sprintf(buf, "abcde", p++);
  return p;
}

int main(void) {
  char buf[6];
  if (foo(buf, &buf[2]) != &buf[3])
    abort();
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/30473
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn sprintf([[V_0:__v[0-9]+]]: *mut i8, [[V_1:__v[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn foo(mut buf: *mut i8, mut p: *mut i8) -> *mut i8 {
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut i8 = p;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: *mut i8 = unsafe { [[V_2]].offset((1 as i32) as isize) };
// CHECK-NEXT:     p = [[V_3]];
// CHECK-NEXT:     unsafe { sprintf(buf, c"abcde".as_ptr() as *const i8, [[V_2]]) };
// CHECK-NEXT:     p
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut buf: [i8; 6] = [0 as i8; 6];
// CHECK-NEXT:     if foo(buf.as_mut_ptr() as *mut i8, unsafe {
// CHECK-NEXT:         (buf.as_mut_ptr() as *mut i8).offset((2 as i32) as isize)
// CHECK-NEXT:     }) != unsafe { (buf.as_mut_ptr() as *mut i8).offset((3 as i32) as isize) }
// CHECK-NEXT:     {
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
