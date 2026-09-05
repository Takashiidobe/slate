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
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 9;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 11;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 12;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify_direct({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify_direct({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe {
// COMMON-LOWERING-NEXT:         printf(
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}} as *const core::ffi::c_char,
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:             {{__v[0-9]+}},
// COMMON-LOWERING-NEXT:         )
// COMMON-LOWERING-NEXT:     };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn classify({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut result: i32 = 0;
// COMMON-LOWERING-NEXT:     value = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-NEXT:         {
// COMMON-LOWERING-NEXT:             let __switch_value0 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 {
// COMMON-LOWERING-NEXT:                 1..=4 => 0,
// COMMON-LOWERING-NEXT:                 5..=8 => 1,
// COMMON-LOWERING-NEXT:                 10..=12 => 2,
// COMMON-LOWERING-NEXT:                 _ => 3,
// COMMON-LOWERING-NEXT:             };
// COMMON-LOWERING-NEXT:             '__switch0: loop {
// COMMON-LOWERING-NEXT:                 match __switch_case0 {
// COMMON-LOWERING-NEXT:                     0 => {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         __switch_case0 = 1;
// COMMON-LOWERING-NEXT:                         continue '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     1 => {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 20;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     2 => {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 40;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     3 => {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 90;
// COMMON-LOWERING-NEXT:                         result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     _ => {
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn classify_direct({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut value: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     value = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = value;
// COMMON-LOWERING-NEXT:         {
// COMMON-LOWERING-NEXT:             let __switch_value0 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 {
// COMMON-LOWERING-NEXT:                 -2..=2 => 0,
// COMMON-LOWERING-NEXT:                 _ => 1,
// COMMON-LOWERING-NEXT:             };
// COMMON-LOWERING-NEXT:             '__switch0: loop {
// COMMON-LOWERING-NEXT:                 match __switch_case0 {
// COMMON-LOWERING-NEXT:                     0 => {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     1 => {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 9;
// COMMON-LOWERING-NEXT:                         __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                         return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     _ => {
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d %d %d %d %d %d %d %d %d %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d %d %d %d %d %d %d %d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             classify(1),
// COMMON-REWRITES-NEXT:             classify(2),
// COMMON-REWRITES-NEXT:             classify(4),
// COMMON-REWRITES-NEXT:             classify(5),
// COMMON-REWRITES-NEXT:             classify(7),
// COMMON-REWRITES-NEXT:             classify(8),
// COMMON-REWRITES-NEXT:             classify(9),
// COMMON-REWRITES-NEXT:             classify(10),
// COMMON-REWRITES-NEXT:             classify(11),
// COMMON-REWRITES-NEXT:             classify(12),
// COMMON-REWRITES-NEXT:             classify_direct(-1),
// COMMON-REWRITES-NEXT:             classify_direct(3),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn classify(mut {{__v[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let mut result: i32 = 0;
// COMMON-REWRITES-NEXT:     match {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         1..=4 => {
// COMMON-REWRITES-NEXT:             result += 10;
// COMMON-REWRITES-NEXT:             let _v13: i32 = 20;
// COMMON-REWRITES-NEXT:             let _v14: i32 = result;
// COMMON-REWRITES-NEXT:             let _v15: i32 = _v14 + _v13;
// COMMON-REWRITES-NEXT:             result = _v15;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         5..=8 => {
// COMMON-REWRITES-NEXT:             result += 20;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         10..=12 => {
// COMMON-REWRITES-NEXT:             result += 40;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         _ => {
// COMMON-REWRITES-NEXT:             result = 90;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     result
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn classify_direct(mut {{__v[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let mut __retval: i32 = 0;
// COMMON-REWRITES-NEXT:     match {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         -2..=2 => {
// COMMON-REWRITES-NEXT:             return 7;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         _ => {
// COMMON-REWRITES-NEXT:             return 9;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     __retval
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
