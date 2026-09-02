#include <stdio.h>

int score(int x) {
  int out = 0;
  switch (x) {
  case 1:
    out += 10;
  case 2:
    out += 20;
    break;
  case 3:
  case 4:
    out += 40;
    break;
  default:
    out += 90;
  }
  return out;
}

int main(void) {
  printf("%d %d %d %d %d\n", score(1), score(2), score(3), score(4), score(8));
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
// LOWERING-NEXT: fn score({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut x: i32 = 0;
// LOWERING-NEXT:     let mut out: i32 = 0;
// LOWERING-NEXT:     x = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     out = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = x;
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let __switch_value0 = {{_v[0-9]+}};
// LOWERING-NEXT:             let mut __switch_case0: i32 = match __switch_value0 {
// LOWERING-NEXT:                 1 => 0,
// LOWERING-NEXT:                 2 => 1,
// LOWERING-NEXT:                 3 => 2,
// LOWERING-NEXT:                 4 => 3,
// LOWERING-NEXT:                 _ => 4,
// LOWERING-NEXT:             };
// LOWERING-NEXT:             '__switch0: loop {
// LOWERING-NEXT:                 match __switch_case0 {
// LOWERING-NEXT:                     0 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                         out = {{_v[0-9]+}};
// LOWERING-NEXT:                         __switch_case0 = 1;
// LOWERING-NEXT:                         continue '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     1 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                         out = {{_v[0-9]+}};
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     2 => {
// LOWERING-NEXT:                         __switch_case0 = 3;
// LOWERING-NEXT:                         continue '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     3 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 40;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                         out = {{_v[0-9]+}};
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     4 => {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 90;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                         out = {{_v[0-9]+}};
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     _ => {
// LOWERING-NEXT:                         break '__switch0;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = score({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = score({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = score({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = score({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = score({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: fn score({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut x: i32 = {{arg[0-9]+}};
// REWRITES-NEXT:     let mut out: i32 = 0;
// REWRITES-NEXT:     out = 0;
// REWRITES-NEXT:     '{{__switch[0-9]+_l[0-9]+}}: {
// REWRITES-NEXT:         '{{__switch[0-9]+_l[0-9]+}}: {
// REWRITES-NEXT:             '{{__switch[0-9]+_l[0-9]+}}: {
// REWRITES-NEXT:                 '{{__switch[0-9]+_l[0-9]+}}: {
// REWRITES-NEXT:                     '{{__switch[0-9]+_l[0-9]+}}: {
// REWRITES-NEXT:                         match x {
// REWRITES-NEXT:                             1 => {
// REWRITES-NEXT:                                 break '{{__switch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             2 => {
// REWRITES-NEXT:                                 break '{{__switch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             3 => {
// REWRITES-NEXT:                                 break '{{__switch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             4 => {
// REWRITES-NEXT:                                 break '{{__switch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:                             }
// REWRITES-NEXT:                             _ => {}
// REWRITES-NEXT:                         }
// REWRITES-NEXT:                         let {{_v[0-9]+}}: i32 = 90;
// REWRITES-NEXT:                         out = out + {{_v[0-9]+}};
// REWRITES-NEXT:                         break '{{__switch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     break '{{__switch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 let {{_v[0-9]+}}: i32 = 40;
// REWRITES-NEXT:                 out = out + {{_v[0-9]+}};
// REWRITES-NEXT:                 break '{{__switch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             let {{_v[0-9]+}}: i32 = 10;
// REWRITES-NEXT:             out = out + {{_v[0-9]+}};
// REWRITES-NEXT:             break '{{__switch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 20;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = out;
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// REWRITES-NEXT:         out = {{_v[0-9]+}};
// REWRITES-NEXT:         break '{{__switch[0-9]+_l[0-9]+}};
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return out;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             c"%d %d %d %d %d\n".as_ptr(),
// REWRITES-NEXT:             score(1),
// REWRITES-NEXT:             score(2),
// REWRITES-NEXT:             score(3),
// REWRITES-NEXT:             score(4),
// REWRITES-NEXT:             score(8),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
