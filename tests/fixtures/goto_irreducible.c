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
// LOWERING-NEXT:     let mut choose_b: i32 = 0;
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-NEXT:         match {{__state[0-9]+}} {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(choose_b), {{_v[0-9]+}}) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 x = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(choose_b)) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 2;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 3;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             2 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 10;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             3 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 4;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             4 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 5;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             5 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 x = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 6;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             6 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 7;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 8;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             7 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 10;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             8 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 9;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             9 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 15;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             10 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 x = {{_v[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 11;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             11 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 12;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 13;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             12 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 5;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             13 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 14;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             14 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 15;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             15 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 std::process::exit({{_v[0-9]+}} as i32);
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
// REWRITES-NEXT:     let mut choose_b: i32 = 0;
// REWRITES-NEXT:     let mut x: i32 = 0;
// REWRITES-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     '{{__dispatch[0-9]+}}: loop {
// REWRITES-NEXT:         match {{__state[0-9]+}} {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                 __retval = 0;
// REWRITES-NEXT:                 unsafe { std::ptr::write_volatile(std::ptr::addr_of_mut!(choose_b), 0 as i32) };
// REWRITES-NEXT:                 x = 0;
// REWRITES-NEXT:                 {{__state[0-9]+}} = 1;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { std::ptr::read_volatile(std::ptr::addr_of!(choose_b)) };
// REWRITES-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:                 if {{_v[0-9]+}} {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 2;
// REWRITES-NEXT:                 } else {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 3;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             2 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 10;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             3 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 4;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             4 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 5;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             5 => {
// REWRITES-NEXT:                 x += 1;
// REWRITES-NEXT:                 {{__state[0-9]+}} = 6;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             6 => {
// REWRITES-NEXT:                 let {{_v[0-9]+}}: bool = x < 3;
// REWRITES-NEXT:                 if {{_v[0-9]+}} {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 7;
// REWRITES-NEXT:                 } else {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 8;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             7 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 10;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             8 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 9;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             9 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 15;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             10 => {
// REWRITES-NEXT:                 x += 2;
// REWRITES-NEXT:                 {{__state[0-9]+}} = 11;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             11 => {
// REWRITES-NEXT:                 let {{_v[0-9]+}}: bool = x < 4;
// REWRITES-NEXT:                 if {{_v[0-9]+}} {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 12;
// REWRITES-NEXT:                 } else {
// REWRITES-NEXT:                     {{__state[0-9]+}} = 13;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             12 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 5;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             13 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 14;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             14 => {
// REWRITES-NEXT:                 {{__state[0-9]+}} = 15;
// REWRITES-NEXT:                 continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             15 => {
// REWRITES-NEXT:                 unsafe { printf(c"%d\n".as_ptr(), x) };
// REWRITES-NEXT:                 __retval = 0;
// REWRITES-NEXT:                 std::process::exit(__retval as i32);
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                 break '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
