#include <stdio.h>

static int sum_items(int *items, int printable, int length) {
  printf("this is another number: %d %d\n", printable, length);
  int total = 0;
  for (int i = 0; i < 4; i++) {
    total += items[i];
  }
  return total;
}

int main(void) {
  int values[] = {2, 4, 6, 8};
  printf("%d\n", sum_items(values, 5, 4));
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
// LOWERING-NEXT: fn sum_items({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut items: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut total: i32 = 0;
// LOWERING-NEXT:     items = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"this is another number: %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{arg[0-9]+}}, {{arg[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     total = {{_v[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = items;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = total;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 total = {{_v[0-9]+}};
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
// LOWERING-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     *values = [2, 4, 6, 8];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sum_items({{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}});
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
// REWRITES-NEXT: fn sum_items({{arg[0-9]+}}: &[i32], {{arg[0-9]+}}: i32, {{arg[0-9]+}}: i32) -> i32 {
// REWRITES-NEXT:     let mut items: *mut i32 = {{arg[0-9]+}}.as_ptr() as *mut i32;
// REWRITES-NEXT:     let mut total: i32 = 0;
// REWRITES-NEXT:     unsafe { printf(c"this is another number: %d %d\n".as_ptr(), {{arg[0-9]+}}, {{arg[0-9]+}}) };
// REWRITES-NEXT:     total = 0;
// REWRITES-NEXT:     let mut i: i32 = 0;
// REWRITES-NEXT:     i = 0;
// REWRITES-NEXT:     loop {
// REWRITES-NEXT:         let {{_v[0-9]+}}: i32 = 4;
// REWRITES-NEXT:         if i >= {{_v[0-9]+}} {
// REWRITES-NEXT:             break;
// REWRITES-NEXT:         }
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i32 = items;
// REWRITES-NEXT:         let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-NEXT:         total = total + unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:         i = i + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return total;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let mut values: aligned::Aligned<aligned::A16, [i32; 4]> = aligned::Aligned([0; 4]);
// REWRITES-NEXT:     *values = [2, 4, 6, 8];
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = values.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         printf(
// REWRITES-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_char,
// REWRITES-NEXT:             sum_items(
// REWRITES-NEXT:                 unsafe { std::slice::from_raw_parts({{_v[0-9]+}} as *const i32, 4usize) },
// REWRITES-NEXT:                 5,
// REWRITES-NEXT:                 4,
// REWRITES-NEXT:             ),
// REWRITES-NEXT:         )
// REWRITES-NEXT:     };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
