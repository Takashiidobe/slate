#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
  char a[32];
  char b[32];
  char c[32];
  strcpy(a, "  -42abc");
  strcpy(b, "+1000000000000zzz");
  strcpy(c, "9000000000000");
  printf("%d\n", atoi(a));
  printf("%ld\n", atol(b));
  printf("%lld\n", atoll(c));
  return 0;
}

// SLATE-FILECHECK-BEGIN
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf(__format: *const i8, ...) -> i32;
// CHECK-NEXT:     fn atoi(__nptr: *const i8) -> i32;
// CHECK-NEXT:     fn atol(__nptr: *const i8) -> i64;
// CHECK-NEXT:     fn atoll(__nptr: *const i8) -> i64;
// CHECK-NEXT:     fn strcpy(__dest: *mut i8, __src: *const i8) -> *mut i8;
// CHECK-NEXT: }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut libc::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i8; 32]> = __SlateAlign16([0 as i8; 32]);
// CHECK-NEXT:     let mut b: __SlateAlign16<[i8; 32]> = __SlateAlign16([0 as i8; 32]);
// CHECK-NEXT:     let mut c: __SlateAlign16<[i8; 32]> = __SlateAlign16([0 as i8; 32]);
// CHECK-NEXT:     unsafe { strcpy(a.0.as_mut_ptr() as *mut i8, c"  -42abc".as_ptr()) };
// CHECK-NEXT:     unsafe { strcpy(b.0.as_mut_ptr() as *mut i8, c"+1000000000000zzz".as_ptr()) };
// CHECK-NEXT:     unsafe { strcpy(c.0.as_mut_ptr() as *mut i8, c"9000000000000".as_ptr()) };
// CHECK-NEXT:     let __slate_printf_arg0 = unsafe { atoi(a.0.as_mut_ptr() as *const i8) };
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
// CHECK-NEXT:     unsafe { printf(c"%ld\n".as_ptr(), unsafe { atol(b.0.as_mut_ptr() as *const i8) }) };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         printf(c"%lld\n".as_ptr(), unsafe { atoll(c.0.as_mut_ptr() as *const i8) })
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut std::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut std::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
