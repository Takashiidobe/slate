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
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
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
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 { 1..=4 => 0, 5..=8 => 1, 10..=12 => 2, _ => 3 };
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
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 { -2..=2 => 0, _ => 1 };
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn classify({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut result: i32 = 0;
// REWRITES-NEXT: result = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let __switch_value0 = value;
// REWRITES-NEXT:                     let mut __switch_case0: i32 = match __switch_value0 { 1..=4 => 0, 5..=8 => 1, 10..=12 => 2, _ => 3 };
// REWRITES-NEXT:                     '__switch0: loop {
// REWRITES-NEXT:                                     match __switch_case0 {
// REWRITES-NEXT:                                         0 => {
// REWRITES-NEXT:                                                             let {{_v[0-9]+}}: i32 = 10;
// REWRITES-NEXT:                                                             result = result + {{_v[0-9]+}};
// REWRITES-NEXT:                                                             __switch_case0 = 1;
// REWRITES-NEXT:                                                             continue '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         1 => {
// REWRITES-NEXT:                                                             let {{_v[0-9]+}}: i32 = 20;
// REWRITES-NEXT:                                                             result = result + {{_v[0-9]+}};
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         2 => {
// REWRITES-NEXT:                                                             let {{_v[0-9]+}}: i32 = 40;
// REWRITES-NEXT:                                                             result = result + {{_v[0-9]+}};
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         3 => {
// REWRITES-NEXT:                                                             result = 90;
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         _ => {
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return result;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn classify_direct({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let mut value: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         {
// REWRITES-NEXT:                     let __switch_value0 = value;
// REWRITES-NEXT:                     let mut __switch_case0: i32 = match __switch_value0 { -2..=2 => 0, _ => 1 };
// REWRITES-NEXT:                     '__switch0: loop {
// REWRITES-NEXT:                                     match __switch_case0 {
// REWRITES-NEXT:                                         0 => {
// REWRITES-NEXT:                                                             __retval = 7;
// REWRITES-NEXT:                                                             return __retval;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         1 => {
// REWRITES-NEXT:                                                             __retval = 9;
// REWRITES-NEXT:                                                             return __retval;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                         _ => {
// REWRITES-NEXT:                                                             break '__switch0;
// REWRITES-NEXT:                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 7;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 9;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 10;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 11;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 12;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = -1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify_direct({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify_direct({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
