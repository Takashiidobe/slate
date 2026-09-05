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

// SLATE-FILECHECK-BEGIN common-lowering
// COMMON-LOWERING: #![feature(c_variadic)]
// COMMON-LOWERING-NEXT: #![allow(
// COMMON-LOWERING-NEXT:     dead_code,
// COMMON-LOWERING-NEXT:     unused,
// COMMON-LOWERING-NEXT:     non_camel_case_types,
// COMMON-LOWERING-NEXT:     non_snake_case,
// COMMON-LOWERING-NEXT:     non_upper_case_globals,
// COMMON-LOWERING-NEXT:     arithmetic_overflow,
// COMMON-LOWERING-NEXT:     unconditional_panic,
// COMMON-LOWERING-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-LOWERING-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-LOWERING-NEXT:     unused_comparisons
// COMMON-LOWERING-NEXT: )]
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: unsafe extern "C" {
// COMMON-LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let mut i: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut steps: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     steps = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         '__loop0: loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             '__continue0: {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let mut x: i32 = 0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} % {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:                         {
// COMMON-LOWERING-NEXT:                             let __switch_value1 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                             let mut __switch_case1: i32 = match __switch_value1 {
// COMMON-LOWERING-NEXT:                                 0 => 0,
// COMMON-LOWERING-NEXT:                                 1 => 1,
// COMMON-LOWERING-NEXT:                                 2 => 2,
// COMMON-LOWERING-NEXT:                                 _ => 3,
// COMMON-LOWERING-NEXT:                             };
// COMMON-LOWERING-NEXT:                             '__switch1: loop {
// COMMON-LOWERING-NEXT:                                 match __switch_case1 {
// COMMON-LOWERING-NEXT:                                     0 => {
// COMMON-LOWERING-NEXT:                                         __switch_case1 = 1;
// COMMON-LOWERING-NEXT:                                         continue '__switch1;
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                     1 => {
// COMMON-LOWERING-NEXT:                                         __switch_case1 = 2;
// COMMON-LOWERING-NEXT:                                         continue '__switch1;
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                     2 => {
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                                         i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = steps;
// COMMON-LOWERING-NEXT:                                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                                         steps = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                         break '__continue0;
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                     3 => {
// COMMON-LOWERING-NEXT:                                         break '__switch1;
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                     _ => {
// COMMON-LOWERING-NEXT:                                         break '__switch1;
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                 }
// COMMON-LOWERING-NEXT:                             }
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = i;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                     i = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = steps;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                     steps = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = steps;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut i8 = b"unreachable %d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"steps=%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:                     let {{__v[0-9]+}}: *mut u8 = b"unreachable %d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"steps=%d\n\0".as_ptr() as *mut u8;
// SLATE-FILECHECK-END lowering-aarch64-gnu

// SLATE-FILECHECK-BEGIN common-rewrites
// COMMON-REWRITES: #![feature(c_variadic)]
// COMMON-REWRITES-NEXT: #![allow(
// COMMON-REWRITES-NEXT:     dead_code,
// COMMON-REWRITES-NEXT:     unused,
// COMMON-REWRITES-NEXT:     non_camel_case_types,
// COMMON-REWRITES-NEXT:     non_snake_case,
// COMMON-REWRITES-NEXT:     non_upper_case_globals,
// COMMON-REWRITES-NEXT:     arithmetic_overflow,
// COMMON-REWRITES-NEXT:     unconditional_panic,
// COMMON-REWRITES-NEXT:     suspicious_runtime_symbol_definitions,
// COMMON-REWRITES-NEXT:     unpredictable_function_pointer_comparisons,
// COMMON-REWRITES-NEXT:     unused_comparisons
// COMMON-REWRITES-NEXT: )]
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: unsafe extern "C" {
// COMMON-REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     let mut i: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut steps: i32 = 0;
// COMMON-REWRITES-NEXT:     '__loop0: while i < 6 {
// COMMON-REWRITES-NEXT:         '__continue0: {
// COMMON-REWRITES-NEXT:             let mut x: i32 = i % 3;
// COMMON-REWRITES-NEXT:             match x {
// COMMON-REWRITES-NEXT:                 0..=2 => {
// COMMON-REWRITES-NEXT:                     i += 1;
// COMMON-REWRITES-NEXT:                     steps += 1;
// COMMON-REWRITES-NEXT:                     break '__continue0;
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:                 _ => {}
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:             unsafe { printf(c"unreachable %d\n".as_ptr(), i) };
// COMMON-REWRITES-NEXT:             i += 1;
// COMMON-REWRITES-NEXT:             steps += 1;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { printf(c"steps=%d\n".as_ptr(), steps) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
