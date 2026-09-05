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
// COMMON-LOWERING-NEXT:     let mut result: i32 = 0;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut length: i32 = 0;
// COMMON-LOWERING-NEXT:         let mut saved_stack: *mut u8 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 4;
// COMMON-LOWERING-NEXT:         length = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = length;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: u64 = {{__v[0-9]+}} as u64;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: *mut u8 = 0usize as *mut u8;
// COMMON-LOWERING-NEXT:         saved_stack = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         {
// COMMON-LOWERING-NEXT:             let mut values: Vec<i32> = vec![0; {{__v[0-9]+}} as usize];
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let mut index: i32 = 0;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:                 index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 loop {
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = length;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                     if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                         break;
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     {
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = 3;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                         let {{__v[0-9]+}}: *mut i32 = unsafe { values.as_mut_ptr().offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                         unsafe {
// COMMON-LOWERING-NEXT:                             *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                         }
// COMMON-LOWERING-NEXT:                     }
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:                     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:                     index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 }
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = length;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = sum_vla({{__v[0-9]+}}, values.as_mut_ptr());
// COMMON-LOWERING-NEXT:             result = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = result;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 1;
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = unsafe { printf({{__v[0-9]+}} as *const core::ffi::c_char, {{__v[0-9]+}}) };
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     std::process::exit({{__v[0-9]+}} as i32);
// COMMON-LOWERING-NEXT: }
// COMMON-LOWERING-EMPTY:
// COMMON-LOWERING-NEXT: fn sum_vla({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// COMMON-LOWERING-NEXT:     let mut length: i32 = 0;
// COMMON-LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// COMMON-LOWERING-NEXT:     let mut total: i32 = 0;
// COMMON-LOWERING-NEXT:     length = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     values = {{arg[0-9]+}};
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:     total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:     {
// COMMON-LOWERING-NEXT:         let mut index: i32 = 0;
// COMMON-LOWERING-NEXT:         let {{__v[0-9]+}}: i32 = 0;
// COMMON-LOWERING-NEXT:         index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         loop {
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = length;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: bool = {{__v[0-9]+}} < {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             if !{{__v[0-9]+}} {
// COMMON-LOWERING-NEXT:                 break;
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             {
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i64 = {{__v[0-9]+}} as i64;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: *mut i32 = unsafe { {{__v[0-9]+}}.offset({{__v[0-9]+}} as isize) };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = unsafe { *{{__v[0-9]+}} };
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:                 let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:                 total = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:             }
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = index;
// COMMON-LOWERING-NEXT:             let {{__v[0-9]+}}: i32 = {{__v[0-9]+}} + 1;
// COMMON-LOWERING-NEXT:             index = {{__v[0-9]+}};
// COMMON-LOWERING-NEXT:         }
// COMMON-LOWERING-NEXT:     }
// COMMON-LOWERING-NEXT:     let {{__v[0-9]+}}: i32 = total;
// COMMON-LOWERING-NEXT:     return {{__v[0-9]+}};
// COMMON-LOWERING-NEXT: }
// SLATE-FILECHECK-END common-lowering

// SLATE-FILECHECK-BEGIN lowering-x86_64-gnu
// LOWERING-X86_64-GNU-NEXT:     let {{__v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// SLATE-FILECHECK-END lowering-x86_64-gnu

// SLATE-FILECHECK-BEGIN lowering-aarch64-gnu
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
// COMMON-REWRITES-NEXT:     let mut result: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut length: i32 = 4;
// COMMON-REWRITES-NEXT:     let mut values: Vec<i32> = vec![0; (length as u64) as usize];
// COMMON-REWRITES-NEXT:     for index in 0..length {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: i32 = index + 3;
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = unsafe { values.as_mut_ptr().offset((index as i64) as isize) };
// COMMON-REWRITES-NEXT:         unsafe {
// COMMON-REWRITES-NEXT:             *{{__v[0-9]+}} = {{__v[0-9]+}};
// COMMON-REWRITES-NEXT:         }
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     result = sum_vla(length, values.as_mut_ptr());
// COMMON-REWRITES-NEXT:     let {{__v[0-9]+}}: i32 = result + 1;
// COMMON-REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), {{__v[0-9]+}}) };
// COMMON-REWRITES-NEXT:     std::process::exit(0 as i32);
// COMMON-REWRITES-NEXT: }
// COMMON-REWRITES-EMPTY:
// COMMON-REWRITES-NEXT: fn sum_vla({{arg[0-9]+}}: i32, {{arg[0-9]+}}: *mut i32) -> i32 {
// COMMON-REWRITES-NEXT:     let __arg1_view = unsafe { std::slice::from_raw_parts({{arg[0-9]+}} as *const i32, {{arg[0-9]+}} as usize) };
// COMMON-REWRITES-NEXT:     let mut length: i32 = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     let mut values: *mut i32 = {{arg[0-9]+}};
// COMMON-REWRITES-NEXT:     let mut total: i32 = 0;
// COMMON-REWRITES-NEXT:     let mut index: i32 = 0;
// COMMON-REWRITES-NEXT:     while index < length {
// COMMON-REWRITES-NEXT:         let {{__v[0-9]+}}: *mut i32 = values;
// COMMON-REWRITES-NEXT:         unsafe { {{__v[0-9]+}}.offset((index as i64) as isize) };
// COMMON-REWRITES-NEXT:         total += unsafe { __arg1_view[(index as usize)] };
// COMMON-REWRITES-NEXT:         index += 1;
// COMMON-REWRITES-NEXT:     }
// COMMON-REWRITES-NEXT:     total
// COMMON-REWRITES-NEXT: }
// SLATE-FILECHECK-END common-rewrites
