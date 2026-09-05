#include <stdio.h>

static char add_char(char a, char b) {
  char c = a + b;
  return c;
}

int main(void) {
  char          x      = 10;
  unsigned char y      = 200;
  signed char   z      = -5;
  char          letter = 'A';
  printf("%d\n", add_char(x, z));
  printf("%d\n", y);
  printf("%c\n", letter);
  printf("%c\n", letter + 1);
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
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 10;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 10;
// LOWERING-NEXT:     let {{__v[0-9]+}}: u8 = 200;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i8 = -5;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 65;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = add_char({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 65;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = add_char({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%c\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%c\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%c\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%c\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-X86_64-GNU-NEXT: fn add_char({{arg[0-9]+}}: i8, {{arg[0-9]+}}: i8) -> i8 {
// LOWERING-AARCH64-GNU-NEXT: fn add_char({{arg[0-9]+}}: u8, {{arg[0-9]+}}: u8) -> u8 {
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{arg[0-9]+}} as i32;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = {{__v[0-9]+}} as i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = {{__v[0-9]+}} as u8;
// LOWERING-NEXT:     return {{__v[0-9]+}};
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
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: i8 = 65;
// REWRITES-X86_64-GNU-NEXT:     unsafe { printf(c"%d\n".as_ptr(), add_char(10, -5) as i32) };
// REWRITES-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: u8 = 65;
// REWRITES-AARCH64-GNU-NEXT:     unsafe { printf(c"%d\n".as_ptr(), add_char(10, (-5 as i8) as u8) as i32) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), (200 as u8) as i32) };
// REWRITES-NEXT:     unsafe { printf(c"%c\n".as_ptr(), {{__v[0-9]+}} as i32) };
// REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = ({{__v[0-9]+}} as i32) + 1;
// REWRITES-NEXT:     unsafe { printf(c"%c\n".as_ptr(), {{__v[0-9]+}}) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-X86_64-GNU-NEXT: fn add_char({{arg[0-9]+}}: i8, {{arg[0-9]+}}: i8) -> i8 {
// REWRITES-X86_64-GNU-NEXT:     (({{arg[0-9]+}} as i32) + ({{arg[0-9]+}} as i32)) as i8
// REWRITES-AARCH64-GNU-NEXT: fn add_char({{arg[0-9]+}}: u8, {{arg[0-9]+}}: u8) -> u8 {
// REWRITES-AARCH64-GNU-NEXT:     (({{arg[0-9]+}} as i32) + ({{arg[0-9]+}} as i32)) as u8
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
