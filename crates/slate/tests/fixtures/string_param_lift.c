#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse_num(char *s) { return atoi(s); }

int forward_num(char *s) { return parse_num(s); }

int text_len(char *s) { return (int)strlen(s); }

int main(void) {
  char digits[] = "42";
  char word[]   = "hello";
  printf("%d %d\n", forward_num(digits), text_len(word));
  return 0;
}

// SLATE-FILECHECK-BEGIN x86_64-unknown-linux-gnu
// CHECK: {{^}}#![allow(non_camel_case_types, non_snake_case, non_upper_case_globals)]
// CHECK-NEXT: #![no_main]
// CHECK-EMPTY:
// CHECK-NEXT: use slate_support::strlen;
// CHECK-EMPTY:
// CHECK-NEXT: fn parse_num(s: *mut i8) -> i32 {
// CHECK-NEXT:     unsafe {
// CHECK-NEXT:         slate_support::__slate_atoi(
// CHECK-NEXT:             ((s as *const i8) as *const core::ffi::c_char) as *const i8,
// CHECK-NEXT:         )
// CHECK-NEXT:     }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn forward_num(s: *mut i8) -> i32 {
// CHECK-NEXT:     parse_num(s)
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn text_len(s: *mut i8) -> i32 {
// CHECK-NEXT:     unsafe { (strlen((s as *const i8) as *const core::ffi::c_char) as u64) as i32 }
// CHECK-NEXT: }
// CHECK-EMPTY:
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut digits: [i8; 3] = [52, 50, 0];
// CHECK-NEXT:     let mut word: [i8; 6] = [104, 101, 108, 108, 111, 0];
// CHECK-NEXT:     let __slate_printf_arg0 = forward_num(digits.as_mut_ptr());
// CHECK-NEXT:     let __slate_printf_arg1 = text_len(word.as_mut_ptr());
// CHECK-NEXT:     println!("{} {}", __slate_printf_arg0, __slate_printf_arg1);
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
