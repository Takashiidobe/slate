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

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-NEXT: use slate_support::atoi;
// CHECK-NEXT: use slate_support::atol;
// CHECK-NEXT: use slate_support::atoll;
// CHECK-NEXT: use slate_support::strcpy;
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i8; 32]> = __SlateAlign16([0i8; 32]);
// CHECK-NEXT:     let mut b: __SlateAlign16<[i8; 32]> = __SlateAlign16([0i8; 32]);
// CHECK-NEXT:     let mut c: __SlateAlign16<[i8; 32]> = __SlateAlign16([0i8; 32]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strcpy(
// CHECK-NEXT:             (a.0.as_mut_ptr() as *mut i8) as *mut core::ffi::c_char,
// CHECK-NEXT:             c"  -42abc".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as *mut i8
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strcpy(
// CHECK-NEXT:             (b.0.as_mut_ptr() as *mut i8) as *mut core::ffi::c_char,
// CHECK-NEXT:             c"+1000000000000zzz".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as *mut i8
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strcpy(
// CHECK-NEXT:             (c.0.as_mut_ptr() as *mut i8) as *mut core::ffi::c_char,
// CHECK-NEXT:             c"9000000000000".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as *mut i8
// CHECK-NEXT:     };
// CHECK-NEXT:     let __slate_printf_arg0 = unsafe {
// CHECK-NEXT:         atoi((a.0.as_mut_ptr() as *const i8) as *const core::ffi::c_char) as i32
// CHECK-NEXT:     };
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld\n".as_ptr(),
// CHECK-NEXT:             atol((b.0.as_mut_ptr() as *const i8) as *const core::ffi::c_char) as i64,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lld\n".as_ptr(),
// CHECK-NEXT:             atoll((c.0.as_mut_ptr() as *const i8) as *const core::ffi::c_char) as i64,
// CHECK-NEXT:         )
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
