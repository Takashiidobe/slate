#include <stdio.h>

static int countdown(int n) {
  int steps = 0;
  while (1) {
    if (n <= 0) {
      break;
    }
    n /= 2;
    steps++;
  }
  return steps;
}

int main(void) {
  printf("%d\n", countdown(100));
  printf("%d\n", countdown(0));
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
// LOWERING-NEXT: fn countdown({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut steps: i32 = 0;
// LOWERING-NEXT:     n = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     steps = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = n;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} <= {{_v[0-9]+}};
// LOWERING-NEXT:                     if {{_v[0-9]+}} {
// LOWERING-NEXT:                         break;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = n;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} / {{_v[0-9]+}};
// LOWERING-NEXT:                 n = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = steps;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                 steps = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = steps;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = countdown({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = countdown({{_v[0-9]+}});
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
// REWRITES-NEXT: fn countdown(mut n: i32) -> i32 {
// REWRITES-NEXT:     let mut steps: i32 = 0;
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} != 0;
// REWRITES-NEXT:         if !{{_v[0-9]+}} {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: bool = n <= 0;
// REWRITES-NEXT:         if {{_v[0-9]+}} {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         n /= 2;
// REWRITES-NEXT:         steps += 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     steps
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), countdown(100)) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), countdown(0)) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
