#include <stdio.h>
#include <stdlib.h>

int main(void) {
  char digits[] = "123";
  printf("%d\n", atoi(digits));
  return 0;
}

// REWRITES-NOT: mod __slate_runtime

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
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT:     fn atoi(_0: *const core::ffi::c_char) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-X86_64-GNU-NEXT:     let mut digits: [i8; 4] = [0; 4];
// LOWERING-AARCH64-GNU-NEXT:     let mut digits: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: [i8; 4] = [49, 50, 51, 0];
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: [u8; 4] = [49, 50, 51, 0];
// LOWERING-NEXT:     digits = {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = digits.as_mut_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = digits.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { atoi({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
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
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn atoi(_0: *const core::ffi::c_char) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn __slate_atoi(s: *const i8) -> i32 {
// REWRITES-AARCH64-GNU-NEXT: fn __slate_atoi(s: *const u8) -> i32 {
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
// REWRITES-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     let mut digits: [i8; 4] = [0; 4];
// REWRITES-AARCH64-GNU-NEXT:     let mut digits: [u8; 4] = [0; 4];
// REWRITES-NEXT:     digits = [49, 50, 51, 0];
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = c"%d\n".as_ptr() as *mut i8;
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = digits.as_mut_ptr() as *mut i8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = c"%d\n".as_ptr() as *mut u8;
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = digits.as_mut_ptr() as *mut u8;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf({{__v[0-9]+}} as *const core::ffi::c_char, unsafe {
// REWRITES-X86_64-GNU-NEXT:             __slate_atoi({{__v[0-9]+}} as *const i8)
// REWRITES-AARCH64-GNU-NEXT:             __slate_atoi({{__v[0-9]+}} as *const u8)
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
