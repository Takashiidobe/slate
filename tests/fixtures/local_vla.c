#include <stdio.h>

static int sum_vla(int length, int (*values)[length]) {
  int total = 0;
  for (int index = 0; index < length; ++index) {
    total += (*values)[index];
  }
  return total;
}

int main(void) {
  int result;
  {
    int length = 4;
    int values[length];
    for (int index = 0; index < length; ++index) {
      values[index] = index + 3;
    }
    result = sum_vla(length, &values);
  }
  printf("%d\n", result + 1);
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
// LOWERING-NEXT: fn sum_vla({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-NEXT:     let mut length: i32 = 0;
// LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     length = {{arg[0-9]+}};
// LOWERING-NEXT:     values = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut index: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         index = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = length;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 total = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             index = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut result: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut length: i32 = 0;
// LOWERING-NEXT:         let mut saved_stack: *mut u8 = std::ptr::null_mut();
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:         length = {{_v[0-9]+}};
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = length;
// LOWERING-NEXT:         let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:         let {{_v[0-9]+}}: *mut u8 = 0usize as *mut u8;
// LOWERING-NEXT:         saved_stack = {{_v[0-9]+}};
// LOWERING-NEXT:         {
// LOWERING-NEXT:             let mut values: Vec<i32> = vec![0; {{_v[0-9]+}} as usize];
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let mut index: i32 = 0;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:                 index = {{_v[0-9]+}};
// LOWERING-NEXT:                 loop {
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = length;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:                     if !{{_v[0-9]+}} {
// LOWERING-NEXT:                         break;
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     {
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                         let {{_v[0-9]+}}: *mut i32 = unsafe { values.as_mut_ptr().offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                         unsafe {
// LOWERING-NEXT:                             *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                         }
// LOWERING-NEXT:                     }
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = index;
// LOWERING-NEXT:                     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:                     index = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = length;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = sum_vla({{_v[0-9]+}}, values.as_mut_ptr());
// LOWERING-NEXT:             result = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = result;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
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
// REWRITES-NEXT: fn sum_vla({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// REWRITES-NEXT:     let __arg1_view = unsafe { std::slice::from_raw_parts({{arg[0-9]+}} as *const i32, {{arg[0-9]+}} as usize) };
// REWRITES-NEXT:     let mut length: i32 = {{arg[0-9]+}};
// REWRITES-NEXT:     let mut values: *mut i32 = {{arg[0-9]+}};
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     let mut index: i32 = 0;
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         if !(index < length) {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i32 = values;
// REWRITES-NEXT:         unsafe { {{_v[0-9]+}}.offset((index as i64) as isize) };
// REWRITES-NEXT:         total += unsafe { __arg1_view[(index as usize)] };
// REWRITES-NEXT:         index += 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     total
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut result: i32 = 0;
// REWRITES-NEXT:     let mut length: i32 = 4;
// REWRITES-NEXT:     let mut values: Vec<i32> = vec![0; (length as u64) as usize];
// REWRITES-NEXT:     for index in 0..length {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = index + 3;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i32 = unsafe { values.as_mut_ptr().offset((index as i64) as isize) };
// REWRITES-NEXT:         unsafe {
// REWRITES-NEXT:             *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     result = sum_vla(length, values.as_mut_ptr());
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = result + 1;
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{_v[0-9]+}}) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
