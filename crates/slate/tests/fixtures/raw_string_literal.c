// { dg-options "-std=gnu23" }
#include <stdio.h>
#include <string.h>
#include <wchar.h>

#define RAW_IN_MACRO R"(macro\t"quoted")"

int main(void) {
  const char *plain     = R"(gnu\n)";
  const char *delimited = R"tag(a)"b)tag";
  const char *multiline = R"(first
second)";
  const char *utf8      = u8R"x(A)x";
  const wchar_t *wide   = LR"(w\0)";
  const char *joined    = R"(left\)" "\x41";
  printf("%s|%s|%s|%s|%s|%s\n", plain, delimited, multiline, utf8, RAW_IN_MACRO,
         joined);
  printf("%zu %zu %zu %d\n", strlen(plain), sizeof(R"(\\)"), wcslen(wide),
         (int)wide[1]);
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: //! { dg-options "-std=gnu23" }
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn fflush(_0: *mut slate_support::FILE) -> i32;
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let plain: *const i8 = c"gnu\\n".as_ptr();
// CHECK-NEXT:     let delimited: *const i8 = c"a)\"b".as_ptr();
// CHECK-NEXT:     let multiline: *const i8 = c"first\nsecond".as_ptr();
// CHECK-NEXT:     let utf8: *const i8 = c"A".as_ptr();
// CHECK-NEXT:     let wide: *const i32 =
// CHECK-NEXT:         (unsafe { std::ptr::addr_of_mut!(__slate_str22) as *mut i32 }) as *const i32;
// CHECK-NEXT:     let joined: *const i8 = c"left\\A".as_ptr();
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%s|%s|%s|%s|%s|%s\n".as_ptr(),
// CHECK-NEXT:             plain,
// CHECK-NEXT:             delimited,
// CHECK-NEXT:             multiline,
// CHECK-NEXT:             utf8,
// CHECK-NEXT:             c"macro\\t\"quoted\"".as_ptr() as *mut i8,
// CHECK-NEXT:             joined,
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     let _ = std::io::Write::flush(&mut std::io::stdout());
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::printf(
// CHECK-NEXT:             c"%zu %zu %zu %d\n".as_ptr(),
// CHECK-NEXT:             "gnu\\n".len() as u64,
// CHECK-NEXT:             std::mem::size_of::<[i8; 3]>() as u64,
// CHECK-NEXT:             wcslen(wide as *const i32) as u64,
// CHECK-NEXT:             *wide.add(1),
// CHECK-NEXT:         )
// CHECK-NEXT:     };
// CHECK-NEXT:     unsafe { fflush(std::ptr::null_mut()) };
// CHECK-NEXT:     0
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::strlen;
// CHECK-NEXT: use slate_support::wcslen;
// CHECK-EMPTY:
// CHECK-NEXT: static mut __slate_str22: [i32; 4] = [119, 92, 48, 0];
// CHECK-EMPTY:
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
