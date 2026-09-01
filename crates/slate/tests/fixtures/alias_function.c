
// COMMON-LABEL: {{^}}fn alias_impl(_0: i32) -> i32 {
// COMMON-DAG: real_impl(_0)
// COMMON: {{^}}}

#include <stdio.h>

int real_impl(int x) { return x * 3 + 1; }

extern int alias_impl(int x) __attribute__((alias("real_impl")));

int call_alias(int x) { return alias_impl(x) + real_impl(x); }

int main(void) {
  printf("%d %d\n", alias_impl(5), call_alias(2));
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
// LOWERING-NEXT: fn real_impl({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{arg[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn alias_impl(_0: i32) -> i32 {
// LOWERING-NEXT:     return real_impl(_0);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn call_alias({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = alias_impl({{arg[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = real_impl({{arg[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = alias_impl({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = call_alias({{_v[0-9]+}});
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
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn real_impl({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:     return {{arg[0-9]+}} * {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn alias_impl(_0: i32) -> i32 {
// REWRITES-NEXT:     return real_impl(_0);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn call_alias({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = alias_impl({{arg[0-9]+}});
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = real_impl({{arg[0-9]+}});
// REWRITES-NEXT:     return {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"%d %d\n".as_ptr(), alias_impl(5), call_alias(2)) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
