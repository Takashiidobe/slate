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
// LOWERING-NEXT: fn classify({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut c: i32 = 0;
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let mut {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let mut {{__state[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     '{{__dispatch[0-9]+}}: loop {
// LOWERING-NEXT:         match {{__state[0-9]+}} {
// LOWERING-NEXT:             0 => {
// LOWERING-NEXT:                 c = {{arg[0-9]+}};
// LOWERING-NEXT:                 {{__state[0-9]+}} = 1;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             1 => {
// LOWERING-NEXT:                 {{_v[0-9]+}} = c;
// LOWERING-NEXT:                 {{__state[0-9]+}} = 2;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             2 => {
// LOWERING-NEXT:                 match {{_v[0-9]+}} {
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 4;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     2 => {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 6;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     3 => {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 7;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     4 => {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 8;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 9;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
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
// LOWERING-NEXT:                 {{__state[0-9]+}} = 6;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             6 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 10;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             7 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 20;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             8 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 22;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             9 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = -1;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 return {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             10 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 11;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             11 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 12;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             12 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 13;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             13 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: bool = true;
// LOWERING-NEXT:                 if {{_v[0-9]+}} {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 14;
// LOWERING-NEXT:                 } else {
// LOWERING-NEXT:                     {{__state[0-9]+}} = 28;
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             14 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 15;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             15 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 16;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             16 => {
// LOWERING-NEXT:                 {{_v[0-9]+}} = c;
// LOWERING-NEXT:                 {{__state[0-9]+}} = 17;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             17 => {
// LOWERING-NEXT:                 match {{_v[0-9]+}} {
// LOWERING-NEXT:                     5 => {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 19;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     6 => {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 21;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         {{__state[0-9]+}} = 23;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             18 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 19;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             19 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 20;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             20 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 return {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             21 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 22;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             22 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 200;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 return {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             23 => {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = -2;
// LOWERING-NEXT:                 __retval = {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:                 return {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             24 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 25;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             25 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 26;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             26 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 27;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             27 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 13;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             28 => {
// LOWERING-NEXT:                 {{__state[0-9]+}} = 29;
// LOWERING-NEXT:                 continue '{{__dispatch[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             29 => {
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
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 9;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = classify({{_v[0-9]+}});
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
// REWRITES-NEXT: fn classify({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut c: i32 = 0;
// REWRITES-NEXT:     let mut __retval: i32 = 0;
// REWRITES-NEXT:     let mut {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     let mut {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT:     '{{__dispatch[0-9]+_l[0-9]+}}: {
// REWRITES-NEXT:         '{{__dispatch[0-9]+_l[0-9]+}}: {
// REWRITES-NEXT:             '{{__dispatch[0-9]+_l[0-9]+}}: {
// REWRITES-NEXT:                 '{{__dispatch[0-9]+_l[0-9]+}}: {
// REWRITES-NEXT:                     '{{__dispatch[0-9]+_l[0-9]+}}: {
// REWRITES-NEXT:                         c = {{arg[0-9]+}};
// REWRITES-NEXT:                         {{_v[0-9]+}} = c;
// REWRITES-NEXT:                         match {{_v[0-9]+}} {
// REWRITES-NEXT:                             1 => {
// REWRITES-NEXT:                                 break '{{__dispatch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             2 => {
// REWRITES-NEXT:                                 break '{{__dispatch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             3 => {
// REWRITES-NEXT:                                 break '{{__dispatch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             4 => {
// REWRITES-NEXT:                                 break '{{__dispatch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             _ => {}
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         __retval = -1;
// REWRITES-NEXT:                         return __retval;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     break '{{__dispatch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 break '{{__dispatch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             if !true {
// REWRITES-NEXT:                 return __retval;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             {{_v[0-9]+}} = c;
// REWRITES-NEXT:             match {{_v[0-9]+}} {
// REWRITES-NEXT:                 5 => {
// REWRITES-NEXT:                     break '{{__dispatch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 6 => {
// REWRITES-NEXT:                     break '{{__dispatch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 _ => {}
// REWRITES-NEXT:             }
// REWRITES-NEXT:             __retval = -2;
// REWRITES-NEXT:             return __retval;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         __retval = 200;
// REWRITES-NEXT:         return __retval;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     __retval = 100;
// REWRITES-NEXT:     return __retval;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(1)) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(3)) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(4)) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(5)) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(6)) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), classify(9)) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
