#include <stdio.h>

int map(int x) {
  int out = 0;
  switch (x) {
  case -3:
    out = 13;
    break;
  case 100:
    out = 1000;
    break;
  default:
    out = -1;
    break;
  }
  return out;
}

int main(void) {
  printf("%d %d %d\n", map(-3), map(100), map(0));
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
// LOWERING-NEXT: fn map({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut out: i32 = 0;
// LOWERING-NEXT:     x = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     out = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let __switch_value0 = {{_v[0-9]+}};
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-NEXT:                 -3 => 0,
// LOWERING-NEXT:                 100 => 1,
// LOWERING-NEXT:                 _ => 2,
// LOWERING-NEXT:             };
// LOWERING-NEXT:             '__switch0: loop {
// LOWERING-NEXT:                 match __switch_case0 {
// LOWERING-NEXT:                     0 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 13;
// LOWERING-NEXT:                         out = {{_v[0-9]+}};
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 1000;
// LOWERING-NEXT:                         out = {{_v[0-9]+}};
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     2 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:                         out = {{_v[0-9]+}};
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = map({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = map({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = map({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: fn map(mut {{_v[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut out: i32 = 0;
// REWRITES-NEXT:     match {{_v[0-9]+}} {
// REWRITES-NEXT:         -3 => {
// REWRITES-NEXT:             out = 13;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         100 => {
// REWRITES-NEXT:             out = 1000;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         _ => {
// REWRITES-NEXT:             out = -1;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     out
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"%d %d %d\n".as_ptr(), map(-3), map(100), map(0)) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
