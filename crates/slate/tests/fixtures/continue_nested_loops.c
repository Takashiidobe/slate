#include <stdio.h>

static int sum_grid(int n) {
  int total = 0;
  for (int i = 0; i < n; i++) {
    if (i == 2) {
      continue;
    }
    for (int j = 0; j < n; j++) {
      if (j == 3) {
        continue;
      }
      total += i * 10 + j;
    }
  }
  return total;
}

int main(void) {
  printf("%d\n", sum_grid(5));
  printf("%d\n", sum_grid(1));
  printf("%d\n", sum_grid(0));
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
// LOWERING-NEXT: fn sum_grid({{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     n = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         '__loop0: loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = n;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             '__continue0: {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 2;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                         if {{_v[0-9]+}} {
// LOWERING-NEXT:                             break '__continue0;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let mut j: i32 = 0;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                         j = {{_v[0-9]+}};
// LOWERING-NEXT:                         '__loop1: loop {
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = n;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                                 break;
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                             '__continue1: {
// LOWERING-NEXT:                                 {
// LOWERING-NEXT:                                     {
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:                                         let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:                                         if {{_v[0-9]+}} {
// LOWERING-NEXT:                                             break '__continue1;
// LOWERING-NEXT:                                         }
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = 10;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                                     total = {{_v[0-9]+}};
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = j;
// LOWERING-NEXT:                             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                             j = {{_v[0-9]+}};
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sum_grid({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sum_grid({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sum_grid({{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
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
// REWRITES-NEXT: fn sum_grid({{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT: let mut n: i32 = {{arg[0-9]+}};
// REWRITES-NEXT: let mut total: i32 = 0;
// REWRITES-NEXT: total = 0;
// REWRITES-NEXT: {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         '__loop0: loop {
// REWRITES-NEXT:                     if !(i < n) {
// REWRITES-NEXT:                                     break;
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     '__continue0: {
// REWRITES-NEXT:                                     {
// REWRITES-NEXT:                                                         {
// REWRITES-NEXT:                                                                                 let {{_v[0-9]+}}: i32 = 2;
// REWRITES-NEXT:                                                                                 if i == {{_v[0-9]+}} {
// REWRITES-NEXT:                                                                                                             break '__continue0;
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                                         {
// REWRITES-NEXT:                                                                                 let mut j: i32 = 0;
// REWRITES-NEXT:                                                                                 j = 0;
// REWRITES-NEXT:                                                                                 '__loop1: loop {
// REWRITES-NEXT:                                                                                                             if !(j < n) {
// REWRITES-NEXT:                                                                                                                                             break;
// REWRITES-NEXT:                                                                                                             }
// REWRITES-NEXT:                                                                                                             '__continue1: {
// REWRITES-NEXT:                                                                                                                                             {
// REWRITES-NEXT:                                                                                                                                                                                 {
// REWRITES-NEXT:                                                                                                                                                                                                                         let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:                                                                                                                                                                                                                         if j == {{_v[0-9]+}} {
// REWRITES-NEXT:                                                                                                                                                                                                                                                                     break '__continue1;
// REWRITES-NEXT:                                                                                                                                                                                                                         }
// REWRITES-NEXT:                                                                                                                                                                                 }
// REWRITES-NEXT:                                                                                                                                                                                 let {{_v[0-9]+}}: i32 = 10;
// REWRITES-NEXT:                                                                                                                                                                                 total = total + (i * {{_v[0-9]+}} + j);
// REWRITES-NEXT:                                                                                                                                             }
// REWRITES-NEXT:                                                                                                             }
// REWRITES-NEXT:                                                                                                             j = j + 1;
// REWRITES-NEXT:                                                                                 }
// REWRITES-NEXT:                                                         }
// REWRITES-NEXT:                                     }
// REWRITES-NEXT:                     }
// REWRITES-NEXT:                     i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: return total;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 5;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = sum_grid({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 1;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = sum_grid({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = sum_grid({{_v[0-9]+}});
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
