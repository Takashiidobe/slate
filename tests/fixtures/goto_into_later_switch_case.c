#include <stdio.h>

static int classify(int c) {
  switch (c) {
  case 1:
  case 2:
    break;
  case 3:
    goto low;
  case 4:
    goto high;
  default:
    return -1;
  }
  for (;;) {
    switch (c) {
    case 5:
    low:
      return 100;
    case 6:
    high:
      return 200;
    default:
      return -2;
    }
  }
}

int main() {
  printf("%d\n", classify(1));
  printf("%d\n", classify(3));
  printf("%d\n", classify(4));
  printf("%d\n", classify(5));
  printf("%d\n", classify(6));
  printf("%d\n", classify(9));
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
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 5;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 9;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = classify({{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn classify({{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut c: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut __retval: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// COMMON-LOWERING-NEXT:         match {{__state[0-9]+}} {
// COMMON-LOWERING-NEXT:             0 => {
// COMMON-LOWERING-NEXT:                 c = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             1 => {
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} = c;
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 2;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             2 => {
// COMMON-LOWERING-NEXT:                 match {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     1 => {
// COMMON-LOWERING-NEXT:                         {{__state[0-9]+}} = 4;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     2 => {
// COMMON-LOWERING-NEXT:                         {{__state[0-9]+}} = 6;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     3 => {
// COMMON-LOWERING-NEXT:                         {{__state[0-9]+}} = 7;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     4 => {
// COMMON-LOWERING-NEXT:                         {{__state[0-9]+}} = 8;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     _ => {
// COMMON-LOWERING-NEXT:                         {{__state[0-9]+}} = 9;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
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
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 6;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             6 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 10;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             7 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 20;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             8 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 22;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             9 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = -1;
// COMMON-LOWERING-NEXT:                 __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                 return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             10 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 11;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             11 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 12;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             12 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 13;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             13 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: bool = true;
// COMMON-LOWERING-NEXT:                 if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 14;
// COMMON-LOWERING-NEXT:                 } else {
// COMMON-LOWERING-NEXT:                     {{__state[0-9]+}} = 28;
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             14 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 15;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             15 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 16;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             16 => {
// COMMON-LOWERING-NEXT:                 {{__v[0-9]+}} = c;
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 17;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             17 => {
// COMMON-LOWERING-NEXT:                 match {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                     5 => {
// COMMON-LOWERING-NEXT:                         {{__state[0-9]+}} = 19;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     6 => {
// COMMON-LOWERING-NEXT:                         {{__state[0-9]+}} = 21;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     _ => {
// COMMON-LOWERING-NEXT:                         {{__state[0-9]+}} = 23;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             18 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 19;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             19 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 20;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             20 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 100;
// COMMON-LOWERING-NEXT:                 __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                 return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             21 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 22;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             22 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 200;
// COMMON-LOWERING-NEXT:                 __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                 return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             23 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = -2;
// COMMON-LOWERING-NEXT:                 __retval = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                 return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             24 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 25;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             25 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 26;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             26 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 27;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             27 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 13;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             28 => {
// COMMON-LOWERING-NEXT:                 {{__state[0-9]+}} = 29;
// COMMON-LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             29 => {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = __retval;
// COMMON-LOWERING-NEXT:                 return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             _ => {
// COMMON-LOWERING-NEXT:                 unreachable!();
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
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
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(1)) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(3)) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(4)) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(5)) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(6)) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(9)) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn classify(mut c: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let mut __retval: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut {{__v[0-9]+}}: i32 = c;
// COMMON-REWRITES-NEXT:     let mut {{__v[0-9]+}}: i32 = 0;
// COMMON-REWRITES-NEXT:     '__join0_7: {
// COMMON-REWRITES-NEXT:         '__join0_8: {
// COMMON-REWRITES-NEXT:             match {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:                 1 => {}
// COMMON-REWRITES-NEXT:                 2 => {}
// COMMON-REWRITES-NEXT:                 3 => {
// COMMON-REWRITES-NEXT:                     break '__join0_7;
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:                 4 => {
// COMMON-REWRITES-NEXT:                     break '__join0_8;
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:                 _ => {
// COMMON-REWRITES-NEXT:                     __retval = -1;
// COMMON-REWRITES-NEXT:                     return __retval;
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:             if true {
// COMMON-REWRITES-NEXT:                 {{__v[0-9]+}} = c;
// COMMON-REWRITES-NEXT:                 match {{__v[0-9]+}} {
// COMMON-REWRITES-NEXT:                     5 => {
// COMMON-REWRITES-NEXT:                         break '__join0_7;
// COMMON-REWRITES-NEXT:                     }
// COMMON-REWRITES-NEXT:                     6 => {}
// COMMON-REWRITES-NEXT:                     _ => {
// COMMON-REWRITES-NEXT:                         __retval = -2;
// COMMON-REWRITES-NEXT:                         return __retval;
// COMMON-REWRITES-NEXT:                     }
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:             } else {
// COMMON-REWRITES-NEXT:                 return __retval;
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         __retval = 200;
// COMMON-REWRITES-NEXT:         return __retval;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     __retval = 100;
// COMMON-REWRITES-NEXT:     __retval
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
