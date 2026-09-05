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
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 8;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = grid({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = grid({{__v[0-9]+}}, {{__v[0-9]+}});
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn grid({{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut rows: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut cols: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     rows = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     cols = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut r: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         r = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = r;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = rows;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let mut c: i32 = 0;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                     c = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     '__loop0: loop {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = c;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = cols;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                             break;
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                         '__continue0: {
// COMMON-LOWERING-NEXT:                             {
// COMMON-LOWERING-NEXT:                                 {
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = c;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = r;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} == {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                                         break '__continue0;
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                 }
// COMMON-LOWERING-NEXT:                                 {
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = c;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:                                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                                         break '__loop0;
// COMMON-LOWERING-NEXT:                                     }
// COMMON-LOWERING-NEXT:                                 }
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = r;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = cols;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} * {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = c;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:                                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                                 total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                             }
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = c;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                         c = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:                 {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = r;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = 6;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} > {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if {{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         break;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = r;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             r = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
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
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), grid(8, 8)) };
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), grid(3, 3)) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn grid(mut rows: i32, mut cols: i32) -> i32 {
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     for r in 0..rows {
// COMMON-REWRITES-NEXT:         let mut c: i32 = 0;
// COMMON-REWRITES-NEXT:         '__loop0: while c < cols {
// COMMON-REWRITES-NEXT:             if c == r {
// COMMON-REWRITES-NEXT:             } else {
// COMMON-REWRITES-NEXT:                 if c > 4 {
// COMMON-REWRITES-NEXT:                     break '__loop0;
// COMMON-REWRITES-NEXT:                 }
// COMMON-REWRITES-NEXT:                 total += r * cols + c;
// COMMON-REWRITES-NEXT:             }
// COMMON-REWRITES-NEXT:             c += 1;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:         if r > 6 {
// COMMON-REWRITES-NEXT:             break;
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     total
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
