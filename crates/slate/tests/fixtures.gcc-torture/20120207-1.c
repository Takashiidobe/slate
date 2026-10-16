/* PR middle-end/51994 */
/* Testcase by Uros Bizjak <ubizjak@gmail.com> */

extern char *strcpy(char *, const char *);
extern void  abort(void);

char __attribute__((noinline)) test(int a) {
  char  buf[16];
  char *output = buf;

  strcpy(&buf[0], "0123456789");

  output += a;
  output -= 1;

  return output[0];
}

int main() {
  if (test(2) != '1')
    abort();

  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: #![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! PR middle-end/51994
// CHECK-NEXT: //!
// CHECK-NEXT: //! Testcase by Uros Bizjak <ubizjak@gmail.com>
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn strcpy([[V_0:__v[0-9]+]]: *mut i8, [[V_1:__v[0-9]+]]: *const i8) -> *mut i8;
// CHECK-NEXT:     fn abort();
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: fn test(mut a: i32) -> i8 {
// CHECK-NEXT:     let mut buf: __SlateAlign16<[i8; 16]> = __SlateAlign16([0 as i8; 16]);
// CHECK-NEXT:     let mut output: *mut i8 = buf.0.as_mut_ptr() as *mut i8;
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strcpy(
// CHECK-NEXT:             unsafe { (buf.0.as_mut_ptr() as *mut i8).offset((0 as i32) as isize) },
// CHECK-NEXT:             c"0123456789".as_ptr() as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     let mut [[V_2:__v[0-9]+]]: *mut i8 = output;
// CHECK-NEXT:     let mut [[V_3:__v[0-9]+]]: *mut i8 = unsafe { [[V_2]].offset(a as isize) };
// CHECK-NEXT:     output = [[V_3]];
// CHECK-NEXT:     let mut [[V_4:__v[0-9]+]]: *mut i8 = output;
// CHECK-NEXT:     let mut [[V_5:__v[0-9]+]]: *mut i8 = unsafe { [[V_4]].offset(-((1 as i32) as isize)) };
// CHECK-NEXT:     output = [[V_5]];
// CHECK-NEXT:     unsafe { *unsafe { output.offset((0 as i32) as isize) } }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     if (test(2 as i32) as i32) != (49 as i32) {
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
