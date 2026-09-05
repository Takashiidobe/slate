#include <stdio.h>

int main() {
  volatile int choose_b = 0;
  int          x        = 0;
  if (choose_b)
    goto b;
a:
  x = x + 1;
  if (x < 3)
    goto b;
  goto done;
b:
  x = x + 2;
  if (x < 4)
    goto a;
done:
  printf("%d\n", x);
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
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut choose_b: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut x: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// COMMON-LOWERING-NEXT:         match {{__state[0-9]+}} {
// COMMON-LOWERING-NEXT:             0 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                 __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                 unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(choose_b), {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                 x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             1 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(choose_b)) };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} != 0;
// COMMON-LOWERING-NEXT:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 2;
// COMMON-LOWERING-NEXT:                 } else {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 3;
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             2 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 10;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             3 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 4;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             4 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 5;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             5 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 6;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             6 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 7;
// COMMON-LOWERING-NEXT:                 } else {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 8;
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             7 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 10;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             8 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 9;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             9 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 15;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             10 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 11;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             11 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 12;
// COMMON-LOWERING-NEXT:                 } else {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 13;
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             12 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 5;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             13 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 14;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             14 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 15;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             15 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                 __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                 std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             _ => {
// COMMON-LOWERING-NEXT:                 break '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:                 let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT:     let mut __retval: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut choose_b: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut x: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(choose_b), 0 as i32) };
// COMMON-REWRITES-NEXT:     x = 0;
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(choose_b)) };
// COMMON-REWRITES-NEXT:     if {{__v[0-9]+}} != 0 {
// COMMON-REWRITES-NEXT:         {{__state[0-9]+}} = 4;
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         {{__state[0-9]+}} = 2;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     loop {
// COMMON-REWRITES-NEXT:         match {{__state[0-9]+}} {
// COMMON-REWRITES-NEXT:             2 => {
// COMMON-REWRITES-NEXT:                 x += 1;
// COMMON-REWRITES-NEXT:                 if x < 3 {
// COMMON-REWRITES-NEXT:                     {{__state[0-9]+}} = 4;
// COMMON-REWRITES-NEXT:                 } else {
// COMMON-REWRITES-NEXT:                     break;
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:             _ => {
// COMMON-REWRITES-NEXT:                 x += 2;
// COMMON-REWRITES-NEXT:                 if x < 4 {
// COMMON-REWRITES-NEXT:                     {{__state[0-9]+}} = 2;
// COMMON-REWRITES-NEXT:                 } else {
// COMMON-REWRITES-NEXT:                     break;
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), x) };
// COMMON-REWRITES-NEXT:     std::process::exit(__retval as i32);
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
