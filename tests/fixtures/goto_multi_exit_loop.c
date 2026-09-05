#include <stdio.h>

int main() {
  int i   = 0;
  int sum = 0;
loop:
  sum = sum + i;
  if (sum > 100)
    goto overflow;
  i = i + 1;
  if (i < 5)
    goto loop;
  printf("%d\n", sum);
  return 0;
overflow:
  printf("overflow\n");
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
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut i: i32 = 0;
// LOWERING-NEXT:     let mut sum: i32 = 0;
// LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-NEXT:         match {{__state[0-9]+}} {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 __retval = {{__v[0-9]+}};
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 i = {{__v[0-9]+}};
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 sum = {{__v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = sum;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:                 sum = {{__v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 2;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             2 => {
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = sum;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:                 if {{__v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 3;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 4;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             3 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 10;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             4 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 5;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             5 => {
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:                 i = {{__v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 6;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             6 => {
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:                 if {{__v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 7;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 8;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             7 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             8 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 9;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             9 => {
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = sum;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 __retval = {{__v[0-9]+}};
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT:             }
// LOWERING-NEXT:             10 => {
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = b"overflow\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = b"overflow\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char) };
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 __retval = {{__v[0-9]+}};
// LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 break '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
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
// REWRITES-NEXT:     let mut __retval: i32 = 0;
// REWRITES-NEXT:     let mut i: i32 = 0;
// REWRITES-NEXT:     let mut sum: i32 = 0;
// REWRITES-NEXT:     __retval = 0;
// REWRITES-NEXT:     i = 0;
// REWRITES-NEXT:     sum = 0;
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         sum += i;
// REWRITES-NEXT:         let {{__v[0-9]+}}: bool = sum > 100;
// REWRITES-NEXT:         if {{__v[0-9]+}} {
// REWRITES-NEXT:             unsafe { printf(c"overflow\n".as_ptr()) };
// REWRITES-NEXT:             __retval = 0;
// REWRITES-NEXT:             std::process::exit(__retval as i32);
// REWRITES-NEXT:         } else {
// REWRITES-NEXT:             i += 1;
// REWRITES-NEXT:             if !(i < 5) {
// REWRITES-NEXT:                 break;
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), sum) };
// REWRITES-NEXT:     __retval = 0;
// REWRITES-NEXT:     std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
