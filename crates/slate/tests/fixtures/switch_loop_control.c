#include <stdio.h>

int run(void) {
  int out = 0;
  for (int i = 0; i <= 3; i++) {
    switch (i) {
    case 0:
      out += 1;
      break;
    case 1:
      continue;
    case 2:
      out += 20;
      break;
    default:
      out += 100;
      break;
    }
    out += 3;
  }
  return out;
}

int main(void) {
  printf("%d\n", run());
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn run() -> i32 {
// LOWERING-NEXT:     let mut out: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     out = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         '__loop0: loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} <= {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             '__continue0: {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                         {
// LOWERING-NEXT:                             let __switch_value1 = {{_v[0-9]+}};
// LOWERING-NEXT:                             let mut __switch_case1: i32 = match __switch_value1 { 0 => 0, 1 => 1, 2 => 2, _ => 3 };
// LOWERING-NEXT:                             '__switch1: loop {
// LOWERING-NEXT:                                 match __switch_case1 {
// LOWERING-NEXT:                                     0 => {
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                                         out = {{_v[0-9]+}};
// LOWERING-NEXT:                                         break '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     1 => {
// LOWERING-NEXT:                                         break '__continue0;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     2 => {
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = 20;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                                         out = {{_v[0-9]+}};
// LOWERING-NEXT:                                         break '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     3 => {
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = 100;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                                         out = {{_v[0-9]+}};
// LOWERING-NEXT:                                         break '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     _ => {
// LOWERING-NEXT:                                         break '__switch1;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                     out = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = out;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = run();
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn run() -> i32 {
// REWRITES-NEXT: let mut out: i32 = 0;
// REWRITES-NEXT: out = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         '__loop0: loop {
// REWRITES-NEXT:                     let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:                     if !(i <= {{_v[0-9]+}}) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     '__continue0: {
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         {
// REWRITES-NEXT:                                                                                 {
// REWRITES-NEXT:                                                                                                             let __switch_value1 = i;
// REWRITES-NEXT:                                                                                                             let mut __switch_case1: i32 = match __switch_value1 { 0 => 0, 1 => 1, 2 => 2, _ => 3 };
// REWRITES-NEXT:                                                                                                             '__switch1: loop {
// REWRITES-NEXT:                                                                                                                                             match __switch_case1 {
// REWRITES-NEXT:                                                                                                                                                 0 => {
// REWRITES-NEXT:                                                                                                                                                                                     let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT:                                                                                                                                                                                     out = out + {{_v[0-9]+}};
// REWRITES-NEXT:                                                                                                                                                                                     break '__switch1;
// REWRITES-NEXT:                                                                                                                                                 }
// REWRITES-NEXT:                                                                                                                                                 1 => {
// REWRITES-NEXT:                                                                                                                                                                                     break '__continue0;
// REWRITES-NEXT:                                                                                                                                                 }
// REWRITES-NEXT:                                                                                                                                                 2 => {
// REWRITES-NEXT:                                                                                                                                                                                     let {{_v[0-9]+}}: i32 = 20;
// REWRITES-NEXT:                                                                                                                                                                                     out = out + {{_v[0-9]+}};
// REWRITES-NEXT:                                                                                                                                                                                     break '__switch1;
// REWRITES-NEXT:                                                                                                                                                 }
// REWRITES-NEXT:                                                                                                                                                 3 => {
// REWRITES-NEXT:                                                                                                                                                                                     let {{_v[0-9]+}}: i32 = 100;
// REWRITES-NEXT:                                                                                                                                                                                     out = out + {{_v[0-9]+}};
// REWRITES-NEXT:                                                                                                                                                                                     break '__switch1;
// REWRITES-NEXT:                                                                                                                                                 }
// REWRITES-NEXT:                                                                                                                                                 _ => {
// REWRITES-NEXT:                                                                                                                                                                                     break '__switch1;
// REWRITES-NEXT:                                                                                                                                                 }
// REWRITES-NEXT:                                                                                                                                             }
// REWRITES-NEXT:                                                                                                             }
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                                         let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:                                                         out = out + {{_v[0-9]+}};
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return out;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = run();
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
