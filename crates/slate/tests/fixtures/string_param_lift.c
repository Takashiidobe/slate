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
// CHECK: {{^}}#![allow(non_camel_case_types)]
// CHECK-NEXT: #![no_main]
// CHECK-NEXT: unsafe extern "C" {
// CHECK-NEXT:     fn printf([[ARG_0:arg[0-9]+]]: *const i8, ...) -> i32;
// CHECK-NEXT:     fn atoi([[ARG_0]]: *const i8) -> i32;
// CHECK-NEXT:     fn strlen([[ARG_0]]: *const i8) -> u64;
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_atoi(s: *const i8) -> i32 {
// CHECK-NEXT:     let bytes = unsafe { std::ffi::CStr::from_ptr(s) }.to_bytes();
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
// CHECK-NEXT:     return std::str::from_utf8(&bytes[start..i])
// CHECK-NEXT:         .unwrap()
// CHECK-NEXT:         .parse::<i128>()
// CHECK-NEXT:         .unwrap_or(0i128)
// CHECK-NEXT:         .clamp(i32::MIN as i128, i32::MAX as i128) as i32;
// CHECK-NEXT: }
// CHECK-NEXT: fn parse_num(s: *mut i8) -> i32 {
// CHECK-NEXT:     unsafe { __slate_atoi(s as *const i8) }
// CHECK-NEXT: }
// CHECK-NEXT: fn forward_num(s: *mut i8) -> i32 {
// CHECK-NEXT:     parse_num(s)
// CHECK-NEXT: }
// CHECK-NEXT: fn text_len(s: *mut i8) -> i32 {
// CHECK-NEXT:     ((unsafe { strlen(s as *const i8) }) as u32) as i32
// CHECK-NEXT: }
// CHECK-NEXT: fn __slate_main() -> i32 {
// CHECK-NEXT:     let mut digits: [i8; 3] = [52, 50, 0];
// CHECK-NEXT:     let mut word: [i8; 6] = [104, 101, 108, 108, 111, 0];
// CHECK-NEXT:     let __slate_printf_arg0 = forward_num(digits.as_mut_ptr() as *mut i8);
// CHECK-NEXT:     let __slate_printf_arg1 = text_len(word.as_mut_ptr() as *mut i8);
// CHECK-NEXT:     println!("{} {}", __slate_printf_arg0, __slate_printf_arg1);
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
