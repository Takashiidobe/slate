#include <stdio.h>
#include <stdlib.h>

static void fill_values(int *values, int len) {
  for (int i = 0; i < len; ++i)
    values[i] = i * 3;
}

static void forward_fill(int *values, int len) {
  fill_values(values, len);
}

int main(void) {
  int len = 5;
  int *values = malloc(len * sizeof(int));
  forward_fill(values, len);
  printf("%d %d\n", values[1], values[4]);
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
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn fill_values({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut len: i32 = 0;
// LOWERING-NEXT:     values = {{arg[0-9]+}};
// LOWERING-NEXT:     len = {{arg[0-9]+}};
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let mut i: i32 = 0;
// LOWERING-NEXT:         let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:         i = {{_v[0-9]+}};
// LOWERING-NEXT:         loop {
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = len;
// LOWERING-NEXT:             let {{_v[0-9]+}}: bool = {{_v[0-9]+}} < {{_v[0-9]+}};
// LOWERING-NEXT:             if !{{_v[0-9]+}} {
// LOWERING-NEXT:                 break;
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-NEXT:             let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:             unsafe {
// LOWERING-NEXT:                 *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn forward_fill({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) {
// LOWERING-NEXT:     fill_values({{arg[0-9]+}}, {{arg[0-9]+}});
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-NEXT:     forward_fill({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(4) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}) };
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
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn fill_values({{arg[0-9]+}}: &mut [i32]) {
// REWRITES-NEXT:     let mut values: *mut i32 = {{arg[0-9]+}}.as_mut_ptr() as *mut i32;
// REWRITES-NEXT:     let mut len: i32 = {{arg[0-9]+}}.len() as i32;
// REWRITES-NEXT:     {
// REWRITES-NEXT:         let mut i: i32 = 0;
// REWRITES-NEXT:         i = 0;
// REWRITES-NEXT:         loop {
// REWRITES-NEXT:             if !(i < len) {
// REWRITES-NEXT:                 break;
// REWRITES-NEXT:             }
// REWRITES-NEXT:             let {{_v[0-9]+}}: i32 = 3;
// REWRITES-NEXT:             let {{_v[0-9]+}}: i32 = i * {{_v[0-9]+}};
// REWRITES-NEXT:             let {{_v[0-9]+}}: *mut i32 = values;
// REWRITES-NEXT:             let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset((i as i64) as isize) };
// REWRITES-NEXT:             unsafe {
// REWRITES-NEXT:                 *{{_v[0-9]+}} = {{_v[0-9]+}};
// REWRITES-NEXT:             }
// REWRITES-NEXT:             i = i + 1;
// REWRITES-NEXT:         }
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn forward_fill({{arg[0-9]+}}: &mut [i32]) {
// REWRITES-NEXT:     fill_values({{arg[0-9]+}});
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = 5;
// REWRITES-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc((({{_v[0-9]+}} as u64) * {{_v[0-9]+}}) as usize) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// REWRITES-NEXT:     forward_fill(unsafe { std::slice::from_raw_parts_mut({{_v[0-9]+}} as *mut i32, {{_v[0-9]+}} as usize) });
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(1) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(4) };
// REWRITES-NEXT:     unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, unsafe { *{{_v[0-9]+}} }) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
