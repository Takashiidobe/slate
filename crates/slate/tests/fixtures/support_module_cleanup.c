#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char digits[] = "123";
  printf("%d\n", atoi(digits));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
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
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut digits: [i8; 4] = [49, 50, 51, 0];
// CHECK-NEXT:     let __slate_printf_arg0 = __slate_atoi(
// CHECK-NEXT:         ((digits.as_mut_ptr() as *const i8) as *const core::ffi::c_char) as *const i8,
// CHECK-NEXT:     );
// CHECK-NEXT:     println!("{}", __slate_printf_arg0);
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
