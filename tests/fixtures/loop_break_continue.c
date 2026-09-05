#include <stdio.h>

static int grid(int rows, int cols) {
  int total = 0;
  for (int r = 0; r < rows; r++) {
    for (int c = 0; c < cols; c++) {
      if (c == r) {
        continue;
      }
      if (c > 4) {
        break;
      }
      total += r * cols + c;
    }
    if (r > 6) {
      break;
    }
  }
  return total;
}

int main(void) {
  printf("%d\n", grid(8, 8));
  printf("%d\n", grid(3, 3));
  return 0;
}

// REWRITES-LABEL: {{^}}fn grid(
// REWRITES-DAG: break;
// REWRITES: {{^}}}

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
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = grid({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-AARCH64-GNU-NEXT:     let {{__v[0-9]+}}: *mut u8 = b"%d\n\0".as_ptr() as *mut u8;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = grid({{__v[0-9]+}}, {{__v[0-9]+}});
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn grid({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut rows: i32 = 0;
// LOWERING-NEXT:     let mut cols: i32 = 0;
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     rows = {{arg[0-9]+}};
// LOWERING-NEXT:     cols = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{__v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut r: i32 = 0;
// LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         r = {{__v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = r;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = rows;
// LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:             if !{{__v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let mut c: i32 = 0;
// LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                     c = {{__v[0-9]+}};
// LOWERING-NEXT:                     '__loop0: loop {
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = c;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = cols;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// LOWERING-NEXT:                         if !{{__v[0-9]+}} {
// LOWERING-NEXT:                             break;
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         '__continue0: {
// LOWERING-NEXT:                             {
// LOWERING-NEXT:                                 {
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = c;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = r;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// LOWERING-NEXT:                                     if {{__v[0-9]+}} {
// LOWERING-NEXT:                                         break '__continue0;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                                 {
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = c;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:                                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:                                     if {{__v[0-9]+}} {
// LOWERING-NEXT:                                         break '__loop0;
// LOWERING-NEXT:                                     }
// LOWERING-NEXT:                                 }
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = r;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = cols;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = c;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// LOWERING-NEXT:                                 total = {{__v[0-9]+}};
// LOWERING-NEXT:                             }
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = c;
// LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:                         c = {{__v[0-9]+}};
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 {
// LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = r;
// LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 6;
// LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// LOWERING-NEXT:                     if {{__v[0-9]+}} {
// LOWERING-NEXT:                         break;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = r;
// LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// LOWERING-NEXT:             r = {{__v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{__v[0-9]+}};
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
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), grid(8, 8)) };
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), grid(3, 3)) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn grid(mut rows: i32, mut cols: i32) -> i32 {
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     for r in 0..rows {
// REWRITES-NEXT:         let mut c: i32 = 0;
// REWRITES-NEXT:         '__loop0: while c < cols {
// REWRITES-NEXT:             if c == r {
// REWRITES-NEXT:             } else {
// REWRITES-NEXT:                 if c > 4 {
// REWRITES-NEXT:                     break '__loop0;
// REWRITES-NEXT:                 }
// REWRITES-NEXT:                 total += r * cols + c;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             c += 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         if r > 6 {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     total
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
