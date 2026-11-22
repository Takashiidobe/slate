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
// CHECK-EMPTY:
// CHECK-NEXT: #[repr(C, align(16))]
// CHECK-NEXT: struct __SlateAlign16<T>(T);
// CHECK-EMPTY:
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_atoi(s: *const i8) -> i32 {
// CHECK-NEXT:     let bytes = unsafe { core::ffi::CStr::from_ptr(s).to_bytes() };
// CHECK-NEXT:     let n = bytes.len();
// CHECK-NEXT:     let mut i = 0usize;
// CHECK-NEXT:     while i < n && (bytes[i].is_ascii_whitespace() || bytes[i] == 11u8) {
// CHECK-NEXT:         i += 1usize;
// CHECK-NEXT:     }
// CHECK-NEXT:     let start = i;
// CHECK-NEXT:     if i < n && (bytes[i] == 43u8 || bytes[i] == 45u8) {
// CHECK-NEXT:         i += 1usize;
// CHECK-NEXT:     }
// CHECK-NEXT:     let digits = i;
// CHECK-NEXT:     while i < n && bytes[i].is_ascii_digit() {
// CHECK-NEXT:         i += 1usize;
// CHECK-NEXT:     }
// CHECK-NEXT:     if i == digits {
// CHECK-NEXT:         return 0i32;
// CHECK-NEXT:     }
// CHECK-NEXT:     return core::str::from_utf8(&bytes[start..i])
// CHECK-NEXT:         .unwrap()
// CHECK-NEXT:         .parse::<i128>()
// CHECK-NEXT:         .unwrap_or(0i128)
// CHECK-NEXT:         .clamp(i32::MIN as i128, i32::MAX as i128) as i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_atol(s: *const i8) -> i64 {
// CHECK-NEXT:     let bytes = unsafe { core::ffi::CStr::from_ptr(s).to_bytes() };
// CHECK-NEXT:     let n = bytes.len();
// CHECK-NEXT:     let mut i = 0usize;
// CHECK-NEXT:     while i < n && (bytes[i].is_ascii_whitespace() || bytes[i] == 11u8) {
// CHECK-NEXT:         i += 1usize;
// CHECK-NEXT:     }
// CHECK-NEXT:     let start = i;
// CHECK-NEXT:     if i < n && (bytes[i] == 43u8 || bytes[i] == 45u8) {
// CHECK-NEXT:         i += 1usize;
// CHECK-NEXT:     }
// CHECK-NEXT:     let digits = i;
// CHECK-NEXT:     while i < n && bytes[i].is_ascii_digit() {
// CHECK-NEXT:         i += 1usize;
// CHECK-NEXT:     }
// CHECK-NEXT:     if i == digits {
// CHECK-NEXT:         return 0i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     return core::str::from_utf8(&bytes[start..i])
// CHECK-NEXT:         .unwrap()
// CHECK-NEXT:         .parse::<i128>()
// CHECK-NEXT:         .unwrap_or(0i128)
// CHECK-NEXT:         .clamp(i64::MIN as i128, i64::MAX as i128) as i64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_atoll(s: *const i8) -> i64 {
// CHECK-NEXT:     let bytes = unsafe { core::ffi::CStr::from_ptr(s).to_bytes() };
// CHECK-NEXT:     let n = bytes.len();
// CHECK-NEXT:     let mut i = 0usize;
// CHECK-NEXT:     while i < n && (bytes[i].is_ascii_whitespace() || bytes[i] == 11u8) {
// CHECK-NEXT:         i += 1usize;
// CHECK-NEXT:     }
// CHECK-NEXT:     let start = i;
// CHECK-NEXT:     if i < n && (bytes[i] == 43u8 || bytes[i] == 45u8) {
// CHECK-NEXT:         i += 1usize;
// CHECK-NEXT:     }
// CHECK-NEXT:     let digits = i;
// CHECK-NEXT:     while i < n && bytes[i].is_ascii_digit() {
// CHECK-NEXT:         i += 1usize;
// CHECK-NEXT:     }
// CHECK-NEXT:     if i == digits {
// CHECK-NEXT:         return 0i64;
// CHECK-NEXT:     }
// CHECK-NEXT:     return core::str::from_utf8(&bytes[start..i])
// CHECK-NEXT:         .unwrap()
// CHECK-NEXT:         .parse::<i128>()
// CHECK-NEXT:         .unwrap_or(0i128)
// CHECK-NEXT:         .clamp(i64::MIN as i128, i64::MAX as i128) as i64;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: unsafe impl<T> Sync for __SlateAlign16<T> {}
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::strcpy;
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut a: __SlateAlign16<[i8; 32]> = __SlateAlign16([0i8; 32]);
// CHECK-NEXT:     let mut b: __SlateAlign16<[i8; 32]> = __SlateAlign16([0i8; 32]);
// CHECK-NEXT:     let mut c: __SlateAlign16<[i8; 32]> = __SlateAlign16([0i8; 32]);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strcpy(
// CHECK-NEXT:             a.0.as_mut_ptr() as *mut core::ffi::c_char,
// CHECK-NEXT:             c"  -42abc".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as *mut i8
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strcpy(
// CHECK-NEXT:             b.0.as_mut_ptr() as *mut core::ffi::c_char,
// CHECK-NEXT:             c"+1000000000000zzz".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as *mut i8
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         strcpy(
// CHECK-NEXT:             c.0.as_mut_ptr() as *mut core::ffi::c_char,
// CHECK-NEXT:             c"9000000000000".as_ptr() as *const core::ffi::c_char,
// CHECK-NEXT:         ) as *mut i8
// CHECK-NEXT:     };
// CHECK-NEXT:     let __slate_printf_arg0 = __slate_atoi(
// CHECK-NEXT:         ((a.0.as_mut_ptr() as *const i8) as *const core::ffi::c_char) as *const i8,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%ld\n".as_ptr(),
// CHECK-NEXT:             __slate_atol(
// CHECK-NEXT:                 ((b.0.as_mut_ptr() as *const i8) as *const core::ffi::c_char)
// CHECK-NEXT:                     as *const i8,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%lld\n".as_ptr(),
// CHECK-NEXT:             __slate_atoll(
// CHECK-NEXT:                 ((c.0.as_mut_ptr() as *const i8) as *const core::ffi::c_char)
// CHECK-NEXT:                     as *const i8,
// CHECK-NEXT:             ),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(core::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: #[unsafe(no_mangle)]
// CHECK-NEXT: unsafe extern "C-unwind" fn main(
// CHECK-NEXT:     argc: i32,
// CHECK-NEXT:     argv: *mut *mut core::ffi::c_char,
// CHECK-NEXT:     envp: *mut *mut core::ffi::c_char,
// CHECK-NEXT: ) -> i32 {
// CHECK-NEXT:     unsafe { __slate_main() }
// CHECK-NEXT: }
// CHECK-NOT: {{.}}
// SLATE-FILECHECK-END
