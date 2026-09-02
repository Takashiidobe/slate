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

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(
// LOWERING-NEXT:     dead_code,
// LOWERING-NEXT:     unused,
// LOWERING-NEXT:     non_camel_case_types,
// LOWERING-NEXT:     non_snake_case,
// LOWERING-NEXT:     non_upper_case_globals,
// LOWERING-NEXT:     arithmetic_overflow,
// LOWERING-NEXT:     unconditional_panic,
// LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// LOWERING-NEXT:     unused_comparisons
// LOWERING-NEXT: )]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn atoi(_0: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn parse_num({{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { atoi({{arg[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn forward_num({{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = parse_num({{arg[0-9]+}});
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn text_len({{arg[0-9]+}}: *mut i8) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { strlen({{arg[0-9]+}} as *const core::ffi::c_char) }) as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut digits: [i8; 3] = [0; 3];
// LOWERING-NEXT:     let mut word: [i8; 6] = [0; 6];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     digits = [52, 50, 0];
// LOWERING-NEXT:     word = [104, 101, 108, 108, 111, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = digits.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = forward_num({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = word.as_mut_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = text_len({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(
// REWRITES-NEXT:     dead_code,
// REWRITES-NEXT:     unused,
// REWRITES-NEXT:     non_camel_case_types,
// REWRITES-NEXT:     non_snake_case,
// REWRITES-NEXT:     non_upper_case_globals,
// REWRITES-NEXT:     arithmetic_overflow,
// REWRITES-NEXT:     unconditional_panic,
// REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// REWRITES-NEXT:     unused_comparisons
// REWRITES-NEXT: )]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn atoi(_0: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT:     fn strlen(_0: *const core::ffi::c_char) -> usize;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn __slate_atoi(s: *const i8) -> i32 {
// REWRITES-NEXT:     let bytes = unsafe { std::ffi::CStr::from_ptr(s) }.to_bytes();
// REWRITES-NEXT:     let n = bytes.len();
// REWRITES-NEXT:     let mut i = 0usize;
// REWRITES-NEXT:     while i < n && (bytes[i].is_ascii_whitespace() || bytes[i] == 11u8) {
// REWRITES-NEXT:         i += 1usize;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let start = i;
// REWRITES-NEXT:     if i < n && (bytes[i] == 43u8 || bytes[i] == 45u8) {
// REWRITES-NEXT:         i += 1usize;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     let digits = i;
// REWRITES-NEXT:     while i < n && bytes[i].is_ascii_digit() {
// REWRITES-NEXT:         i += 1usize;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     if i == digits {
// REWRITES-NEXT:         return 0i32;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return std::str::from_utf8(&bytes[(start..i)])
// REWRITES-NEXT:         .unwrap()
// REWRITES-NEXT:         .parse()
// REWRITES-NEXT:         .unwrap_or(0i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn parse_num({{arg[0-9]+}}: &str) -> i32 {
// REWRITES-NEXT:     unsafe { __slate_atoi({{arg[0-9]+}}.as_ptr() as *const i8) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn forward_num({{arg[0-9]+}}: &str) -> i32 {
// REWRITES-NEXT:     parse_num({{arg[0-9]+}})
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn text_len({{arg[0-9]+}}: &str) -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = (unsafe { {{arg[0-9]+}}.len() as u64 }) as u64;
// REWRITES-NEXT:     {{_v[0-9]+}} as i32
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut digits: [i8; 3] = [52, 50, 0];
// REWRITES-NEXT:     let mut word: [i8; 6] = [104, 101, 108, 108, 111, 0];
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = c"%d %d\n".as_ptr() as *mut i8;
// REWRITES-NEXT:     digits.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = forward_num("42");
// REWRITES-NEXT:     word.as_mut_ptr() as *mut i8;
// REWRITES-NEXT:     unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, text_len("hello")) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
