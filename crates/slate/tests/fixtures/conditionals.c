#include <stdio.h>

static int classify(int n) {
  if (n < 0) {
    return -1;
  } else if (n == 0) {
    return 0;
  } else {
    return 1;
  }
}

static int clamp_low(int n) {
  int r = n;
  if (n < 10) {
    r = 10;
  }
  return r;
}

int main(void) {
  printf("%d\n", classify(-5));
  printf("%d\n", classify(0));
  printf("%d\n", classify(42));
  printf("%d\n", clamp_low(3));
  printf("%d\n", clamp_low(20));

  int x = 7;
  int label;
  if (x % 2 == 0) {
    label = 100;
  } else {
    label = 200;
  }
  printf("%d\n", label);
  return 0;
}

// REWRITES-LABEL: {{^}}fn clamp_low(
// REWRITES-NOT: {{^    \{$}}
// REWRITES: {{^}}}

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
// COMMON-LOWERING-NEXT:     let mut x: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut label: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = -5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 42;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = clamp_low({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 20;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = clamp_low({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 7;
// COMMON-LOWERING-NEXT:     x = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = x;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 2;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} % {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 100;
// COMMON-LOWERING-NEXT:             label = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 200;
// COMMON-LOWERING-NEXT:             label = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = label;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn classify({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut n: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     n = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = n;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:             __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:             return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         } else {
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = n;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 } else {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:                     __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn clamp_low({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut n: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut r: i32 = 0;
// COMMON-LOWERING-NEXT:     n = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = n;
// COMMON-LOWERING-NEXT:     r = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = n;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = 10;
// COMMON-LOWERING-NEXT:             r = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = r;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
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
// COMMON-REWRITES-NEXT:     let mut x: i32 = 7;
// COMMON-REWRITES-NEXT:     let mut label: i32 = 0;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(-5)) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(0)) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(42)) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), clamp_low(3)) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), clamp_low(20)) };
// COMMON-REWRITES-NEXT:     if x % 2 == 0 {
// COMMON-REWRITES-NEXT:         label = 100;
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         label = 200;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), label) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn classify(mut n: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let mut __retval: i32 = 0;
// COMMON-REWRITES-NEXT:     if n < 0 {
// COMMON-REWRITES-NEXT:         return -1;
// COMMON-REWRITES-NEXT:     } else {
// COMMON-REWRITES-NEXT:         if n == 0 {
// COMMON-REWRITES-NEXT:             return 0;
// COMMON-REWRITES-NEXT:         } else {
// COMMON-REWRITES-NEXT:             return 1;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     __retval
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn clamp_low(mut n: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let mut r: i32 = n;
// COMMON-REWRITES-NEXT:     if n < 10 {
// COMMON-REWRITES-NEXT:         r = 10;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     r
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
