#include <stdio.h>

int classify(int n) {
  if (n < 0)
    goto neg;
  if (n == 0)
    goto zero;
  return 1;
neg:
  return -1;
zero:
  return 0;
}

int main() {
  printf("%d %d %d\n", classify(-5), classify(0), classify(42));
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
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-NEXT:         match {{__state[0-9]+}} {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 n = {{arg[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = n;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 2;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 3;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             2 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 9;
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
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = n;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 6;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 7;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             6 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 10;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             7 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 8;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             8 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 return {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             9 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 return {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             10 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 return {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             _ => {
// LOWERING-NEXT:                 unreachable!();
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = -5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 42;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: let mut n: i32 = 0;
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut {{__state[0-9]+}}: i32 = 0;
// REWRITES-NEXT: '{{__dispatch[0-9]+}}: loop {
// REWRITES-NEXT:         match {{__state[0-9]+}} {
// REWRITES-NEXT:             0 => {
// REWRITES-NEXT:                         n = {{arg[0-9]+}};
// REWRITES-NEXT:                         {{__state[0-9]+}} = 1;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             1 => {
// REWRITES-NEXT:                         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:                         if n < {{_v[0-9]+}} {
// REWRITES-NEXT:                                         {{__state[0-9]+}} = 2;
// REWRITES-NEXT:                         } else {
// REWRITES-NEXT:                                         {{__state[0-9]+}} = 3;
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             2 => {
// REWRITES-NEXT:                         {{__state[0-9]+}} = 9;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             3 => {
// REWRITES-NEXT:                         {{__state[0-9]+}} = 4;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             4 => {
// REWRITES-NEXT:                         {{__state[0-9]+}} = 5;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             5 => {
// REWRITES-NEXT:                         let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:                         if n == {{_v[0-9]+}} {
// REWRITES-NEXT:                                         {{__state[0-9]+}} = 6;
// REWRITES-NEXT:                         } else {
// REWRITES-NEXT:                                         {{__state[0-9]+}} = 7;
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             6 => {
// REWRITES-NEXT:                         {{__state[0-9]+}} = 10;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             7 => {
// REWRITES-NEXT:                         {{__state[0-9]+}} = 8;
// REWRITES-NEXT:                         continue '{{__dispatch[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             8 => {
// REWRITES-NEXT:                         __retval = 1;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             9 => {
// REWRITES-NEXT:                         __retval = -1;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             10 => {
// REWRITES-NEXT:                         __retval = 0;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             _ => {
// REWRITES-NEXT:                         unreachable!();
// REWRITES-NEXT:             }
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d %d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = -5;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 42;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
