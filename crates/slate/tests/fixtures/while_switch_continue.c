#include <stdio.h>

int main(void) {
  int i     = 0;
  int steps = 0;
  while (i < 6) {
    int x = i % 3;
    switch (x) {
    case 0:
    case 1:
    case 2:
      i++;
      steps++;
      continue;
    default:
      break;
    }
    printf("unreachable %d\n", i);
    i++;
    steps++;
  }
  printf("steps=%d\n", steps);
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
// LOWERING-NEXT:     let mut i: i32 = 0;
// LOWERING-NEXT:     let mut steps: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     i = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     steps = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         '__loop0: loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             '__continue0: {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let mut x: i32 = 0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} % {{_v[0-9]+}};
// LOWERING-NEXT:                     x = {{_v[0-9]+}};
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = x;
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
// LOWERING-NEXT:                                         __switch_case1 = 1;
// LOWERING-NEXT:                                         continue '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     1 => {
// LOWERING-NEXT:                                         __switch_case1 = 2;
// LOWERING-NEXT:                                         continue '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     2 => {
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                                         i = {{_v[0-9]+}};
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = steps;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                                         steps = {{_v[0-9]+}};
// LOWERING-NEXT:                                         break '__continue0;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     3 => {
// LOWERING-NEXT:                                         break '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     _ => {
// LOWERING-NEXT:                                         break '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     let {{_v[0-9]+}}: *mut i8 = b"unreachable %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                     i = {{_v[0-9]+}};
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = steps;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                     steps = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"steps=%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = steps;
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
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut i: i32 = 0;
// REWRITES-NEXT:     let mut steps: i32 = 0;
// REWRITES-NEXT:     '__loop0: loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 6;
// REWRITES-NEXT:         if !(i < {{_v[0-9]+}}) {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         '__continue0: {
// REWRITES-NEXT:             let mut x: i32 = 0;
// REWRITES-NEXT:             let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:             x = i % {{_v[0-9]+}};
// REWRITES-NEXT:             '__switch1_l4: {
// REWRITES-NEXT:                 '__switch1_l3: {
// REWRITES-NEXT:                     '__switch1_l2: {
// REWRITES-NEXT:                         '__switch1_l1: {
// REWRITES-NEXT:                             '__switch1_l0: {
// REWRITES-NEXT:                                 match x {
// REWRITES-NEXT:                                     0 => {
// REWRITES-NEXT:                                         break '__switch1_l1;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                     1 => {
// REWRITES-NEXT:                                         break '__switch1_l2;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                     2 => {
// REWRITES-NEXT:                                         break '__switch1_l3;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                     _ => {
// REWRITES-NEXT:                                         break '__switch1_l0;
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                                 }
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             break '__switch1_l4;
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         break '__switch1_l2;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     break '__switch1_l3;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 i = i + 1;
// REWRITES-NEXT:                 steps = steps + 1;
// REWRITES-NEXT:                 break '__continue0;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             unsafe { printf(c"unreachable %d\n".as_ptr(), i) };
// REWRITES-NEXT:             i = i + 1;
// REWRITES-NEXT:             steps = steps + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"steps=%d\n".as_ptr(), steps) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
