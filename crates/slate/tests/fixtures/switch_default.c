#include <stdio.h>

int classify(int x) {
  int out = 0;
  switch (x) {
  case 1:
    out = 11;
    break;
  case 2:
    out = 22;
    break;
  default:
    out = 99;
    break;
  }
  return out;
}

int main(void) {
  printf("%d %d %d\n", classify(1), classify(2), classify(7));
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
// COMMON-LOWERING-NEXT: fn classify({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut x: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut out: i32 = 0;
// COMMON-LOWERING-NEXT:     x = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     out = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:         {
// COMMON-LOWERING-NEXT:             let __switch_value0 = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 {
// COMMON-LOWERING-NEXT:                 1 => 0,
// COMMON-LOWERING-NEXT:                 2 => 1,
// COMMON-LOWERING-NEXT:                 _ => 2,
// COMMON-LOWERING-NEXT:             };
// COMMON-LOWERING-NEXT:             '__switch0: loop {
// COMMON-LOWERING-NEXT:                 match __switch_case0 {
// COMMON-LOWERING-NEXT:                     0 => {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 11;
// COMMON-LOWERING-NEXT:                         out = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     1 => {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 22;
// COMMON-LOWERING-NEXT:                         out = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     2 => {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 99;
// COMMON-LOWERING-NEXT:                         out = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     _ => {
// COMMON-LOWERING-NEXT:                         break '__switch0;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = out;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn main() {
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}, {{__v[0-9]+}}, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d %d %d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT: fn classify(mut {{__v[0-9]+}}: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let mut out: i32 = 0;
// COMMON-REWRITES-NEXT:     match {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:         1 => {
// COMMON-REWRITES-NEXT:             out = 11;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         2 => {
// COMMON-REWRITES-NEXT:             out = 22;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         _ => {
// COMMON-REWRITES-NEXT:             out = 99;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     out
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn main() {
// COMMON-REWRITES-NEXT:     unsafe {
// COMMON-REWRITES-NEXT:         printf(
// COMMON-REWRITES-NEXT:             c"%d %d %d\n".as_ptr(),
// COMMON-REWRITES-NEXT:             classify(1),
// COMMON-REWRITES-NEXT:             classify(2),
// COMMON-REWRITES-NEXT:             classify(7),
// COMMON-REWRITES-NEXT:         )
// COMMON-REWRITES-NEXT:     };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
