#include <stdio.h>

static int classify(int value) {
  int result = 0;
  switch (value) {
  case 1 ... 4:
    result += 10;
  case 5 ... 8:
    result += 20;
    break;
  case 10 ... 12:
    result += 40;
    break;
  default:
    result = 90;
  }
  return result;
}

static int classify_direct(int value) {
  switch (value) {
  case -2 ... 2:
    return 7;
  default:
    return 9;
  }
}

int main(void) {
  printf("%d %d %d %d %d %d %d %d %d %d %d %d\n", classify(1), classify(2),
         classify(4), classify(5), classify(7), classify(8), classify(9),
         classify(10), classify(11), classify(12), classify_direct(-1),
         classify_direct(3));
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
// LOWERING-NEXT: fn classify({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     value = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     result = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = value;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let __switch_value0 = {{_v[0-9]+}};
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-NEXT:                 1..=4 => 0,
// LOWERING-NEXT:                 5..=8 => 1,
// LOWERING-NEXT:                 10..=12 => 2,
// LOWERING-NEXT:                 _ => 3,
// LOWERING-NEXT:             };
// LOWERING-NEXT:             '__switch0: loop {
// LOWERING-NEXT:                 match __switch_case0 {
// LOWERING-NEXT:                     0 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                         result = {{_v[0-9]+}};
// LOWERING-NEXT:                         __switch_case0 = 1;
// LOWERING-NEXT:                         continue '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                         result = {{_v[0-9]+}};
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     2 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 40;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                         result = {{_v[0-9]+}};
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     3 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 90;
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
// LOWERING-NEXT: fn classify_direct({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut value: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     value = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = value;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let __switch_value0 = {{_v[0-9]+}};
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-NEXT:                 -2..=2 => 0,
// LOWERING-NEXT:                 _ => 1,
// LOWERING-NEXT:             };
// LOWERING-NEXT:             '__switch0: loop {
// LOWERING-NEXT:                 match __switch_case0 {
// LOWERING-NEXT:                     0 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         return {{_v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 9;
// LOWERING-NEXT:                         __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                         return {{_v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 7;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 9;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 11;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 12;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify_direct({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify_direct({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         printf(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:             {{_v[0-9]+}},
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
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
// REWRITES-NEXT: fn classify(mut {{_v[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut result: i32 = 0;
// REWRITES-NEXT:     match {{_v[0-9]+}} {
// REWRITES-NEXT:         1..=4 => {
// REWRITES-NEXT:             result += 10;
// REWRITES-NEXT:             result += 20;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         5..=8 => {
// REWRITES-NEXT:             result += 20;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         10..=12 => {
// REWRITES-NEXT:             result += 40;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         _ => {
// REWRITES-NEXT:             result = 90;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     result
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn classify_direct(mut {{_v[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut __retval: i32 = 0;
// REWRITES-NEXT:     match {{_v[0-9]+}} {
// REWRITES-NEXT:         -2..=2 => {
// REWRITES-NEXT:             return 7;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         _ => {
// REWRITES-NEXT:             return 9;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     __retval
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d %d %d %d %d %d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             classify(1),
// REWRITES-NEXT:             classify(2),
// REWRITES-NEXT:             classify(4),
// REWRITES-NEXT:             classify(5),
// REWRITES-NEXT:             classify(7),
// REWRITES-NEXT:             classify(8),
// REWRITES-NEXT:             classify(9),
// REWRITES-NEXT:             classify(10),
// REWRITES-NEXT:             classify(11),
// REWRITES-NEXT:             classify(12),
// REWRITES-NEXT:             classify_direct(-1),
// REWRITES-NEXT:             classify_direct(3),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
