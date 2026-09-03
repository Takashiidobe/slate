#include <stdio.h>

int run(void) {
  int out = 0;
  for (int i = 0; i <= 3; i++) {
    switch (i) {
    case 0:
      out += 1;
      break;
    case 1:
      continue;
    case 2:
      out += 20;
      break;
    default:
      out += 100;
      break;
    }
    out += 3;
  }
  return out;
}

int main(void) {
  printf("%d\n", run());
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
// LOWERING-NEXT: fn run() -> i32 {
// LOWERING-NEXT:     let mut out: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     out = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         '__loop0: loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} <= {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             '__continue0: {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             let __switch_value1 = {{_v[0-9]+}};
// LOWERING-NEXT:                             let mut __switch_case1: i32 = match __switch_value1 {
// LOWERING-NEXT:                                 0 => 0,
// LOWERING-NEXT:                                 1 => 1,
// LOWERING-NEXT:                                 2 => 2,
// LOWERING-NEXT:                                 _ => 3,
// LOWERING-NEXT:                             };
// LOWERING-NEXT:                             '__switch1: loop {
// LOWERING-NEXT:                                 match __switch_case1 {
// LOWERING-NEXT:                                     0 => {
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                                         out = {{_v[0-9]+}};
// LOWERING-NEXT:                                         break '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     1 => {
// LOWERING-NEXT:                                         break '__continue0;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     2 => {
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                                         out = {{_v[0-9]+}};
// LOWERING-NEXT:                                         break '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     3 => {
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                                         out = {{_v[0-9]+}};
// LOWERING-NEXT:                                         break '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     _ => {
// LOWERING-NEXT:                                         break '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                     out = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = run();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: fn run() -> i32 {
// REWRITES-NEXT:     let mut out: i32 = 0;
// REWRITES-NEXT:     let mut i: i32 = 0;
// REWRITES-NEXT:     i = 0;
// REWRITES-NEXT:     '__loop0: loop {
// REWRITES-NEXT:         if !(i <= 3) {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         '__continue0: {
// REWRITES-NEXT:             match i {
// REWRITES-NEXT:                 0 => {
// REWRITES-NEXT:                     out += 1;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 1 => {
// REWRITES-NEXT:                     break '__continue0;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 2 => {
// REWRITES-NEXT:                     out += 20;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 _ => {
// REWRITES-NEXT:                     out += 100;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:             }
// REWRITES-NEXT:             out += 3;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         i += 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     out
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), run()) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
