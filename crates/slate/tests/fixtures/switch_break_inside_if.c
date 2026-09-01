#include <stdio.h>

int classify(int x, int extra) {
  int result = -1;
  switch (x) {
  case 0:
    if (extra) {
      result = 10;
      break;
    }
    result = 11;
    break;
  case 1:
    result = 20;
    break;
  default:
    result = -2;
    break;
  }
  return result;
}

int main(void) {
  printf("%d\n", classify(0, 1));
  printf("%d\n", classify(0, 0));
  printf("%d\n", classify(1, 0));
  printf("%d\n", classify(5, 0));
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
// LOWERING-NEXT: fn classify({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut extra: i32 = 0;
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     x = {{arg[0-9]+}};
// LOWERING-NEXT:     extra = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:     result = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let __switch_value0 = {{_v[0-9]+}};
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-NEXT:                 0 => 0,
// LOWERING-NEXT:                 1 => 1,
// LOWERING-NEXT:                 _ => 2,
// LOWERING-NEXT:             };
// LOWERING-NEXT:             '__switch0: loop {
// LOWERING-NEXT:                 match __switch_case0 {
// LOWERING-NEXT:                     0 => {
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = extra;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:                             if {{_v[0-9]+}} {
// LOWERING-NEXT:                                 let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:                                 result = {{_v[0-9]+}};
// LOWERING-NEXT:                                 break '__switch0;
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 11;
// LOWERING-NEXT:                         result = {{_v[0-9]+}};
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:                         result = {{_v[0-9]+}};
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     2 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = -2;
// LOWERING-NEXT:                         result = {{_v[0-9]+}};
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}}, {{_v[0-9]+}});
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
// REWRITES-NEXT: fn classify({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut x: i32 = {{arg[0-9]+}};
// REWRITES-NEXT:     let mut extra: i32 = {{arg[0-9]+}};
// REWRITES-NEXT:     let mut result: i32 = 0;
// REWRITES-NEXT:     result = -1;
// REWRITES-NEXT:     {
// REWRITES-NEXT:         {
// REWRITES-NEXT:             let __switch_value0 = x;
// REWRITES-NEXT:             let mut __switch_case0: i32 = match __switch_value0 {
// REWRITES-NEXT:                 0 => 0,
// REWRITES-NEXT:                 1 => 1,
// REWRITES-NEXT:                 _ => 2,
// REWRITES-NEXT:             };
// REWRITES-NEXT:             '__switch0: loop {
// REWRITES-NEXT:                 match __switch_case0 {
// REWRITES-NEXT:                     0 => {
// REWRITES-NEXT:                         {
// REWRITES-NEXT:                             let {{_v[0-9]+}}: bool = extra != 0;
// REWRITES-NEXT:                             if {{_v[0-9]+}} {
// REWRITES-NEXT:                                 result = 10;
// REWRITES-NEXT:                                 break '__switch0;
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         result = 11;
// REWRITES-NEXT:                         break '__switch0;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     1 => {
// REWRITES-NEXT:                         result = 20;
// REWRITES-NEXT:                         break '__switch0;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     2 => {
// REWRITES-NEXT:                         result = -2;
// REWRITES-NEXT:                         break '__switch0;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     _ => {
// REWRITES-NEXT:                         break '__switch0;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                 }
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return result;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(0, 1)) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(0, 0)) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(1, 0)) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(5, 0)) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
