#include <stdio.h>

static int add_pair(int lhs, int rhs) { return lhs + rhs; }

static int mul_pair(int lhs, int rhs) { return lhs * rhs; }

static int apply_binary(int (*op)(int, int), int lhs, int rhs) {
  return op(lhs, rhs);
}

int main(void) {
  int (*chosen)(int, int) = add_pair;
  printf("%d\n", apply_binary(chosen, 4, 5));
  chosen = mul_pair;
  printf("%d\n", chosen(3, 6));
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = apply_binary(Some(add_pair), {{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { Some(mul_pair).unwrap()({{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn add_pair({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} + {{arg[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn apply_binary(
// LOWERING-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT:     {{arg[0-9]+}}: i32,
// LOWERING-NEXT: ) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { {{arg[0-9]+}}.unwrap()({{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: extern "C-unwind" fn mul_pair({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} * {{arg[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
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
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), apply_binary(Some(add_pair), 4, 5)) };
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(c"%d\n".as_ptr(), unsafe {
// REWRITES-NEXT:             Some(mul_pair).unwrap()(3 as i32, 6 as i32)
// REWRITES-NEXT:         })
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn add_pair({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     {{arg[0-9]+}} + {{arg[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn apply_binary(
// REWRITES-NEXT:     {{arg[0-9]+}}: Option<unsafe extern "C-unwind" fn(i32, i32) -> i32>,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT:     {{arg[0-9]+}}: i32,
// REWRITES-NEXT: ) -> i32 {
// REWRITES-NEXT:     unsafe { {{arg[0-9]+}}.unwrap()({{arg[0-9]+}}, {{arg[0-9]+}}) }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: extern "C-unwind" fn mul_pair({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     {{arg[0-9]+}} * {{arg[0-9]+}}
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
