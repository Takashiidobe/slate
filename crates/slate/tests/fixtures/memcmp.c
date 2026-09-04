#include <stdio.h>
#include <string.h>

static int get_count(void) { return 4; }

// @rewrite-fn-begin
int main(void) {
  unsigned char equal_a[4]   = {1, 2, 3, 4};
  unsigned char equal_b[4]   = {1, 2, 3, 4};
  int           equal_result = memcmp(equal_a, equal_b, 4) == 0;

  unsigned char unequal_a[4]   = {1, 2, 3, 4};
  unsigned char unequal_b[4]   = {1, 2, 3, 9};
  int           unequal_result = memcmp(unequal_a, unequal_b, 4) == 0;

  unsigned char partial_a[8]   = {1, 2, 3, 4, 9, 9, 9, 9};
  unsigned char partial_b[8]   = {1, 2, 3, 4, 0, 0, 0, 0};
  int           partial_result = memcmp(partial_a, partial_b, 4) == 0;

  unsigned char dyn_a[8]   = {1, 2, 3, 4, 5, 6, 7, 8};
  unsigned char dyn_b[8]   = {1, 2, 3, 4, 5, 6, 7, 8};
  int           n          = get_count();
  int           dyn_result = memcmp(dyn_a, dyn_b, n) == 0;

  printf("%d %d %d %d\n", equal_result, unequal_result, partial_result,
         dyn_result);
  return 0;
}
// @rewrite-fn-end

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
// LOWERING-NEXT:     fn memcmp(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn get_count() -> i32 {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut equal_a: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut equal_b: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut unequal_a: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut unequal_b: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut partial_a: [u8; 8] = [0; 8];
// LOWERING-NEXT:     let mut partial_b: [u8; 8] = [0; 8];
// LOWERING-NEXT:     let mut dyn_a: [u8; 8] = [0; 8];
// LOWERING-NEXT:     let mut dyn_b: [u8; 8] = [0; 8];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     equal_a = [1, 2, 3, 4];
// LOWERING-NEXT:     equal_b = [1, 2, 3, 4];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = equal_a.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = equal_b.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         memcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     unequal_a = [1, 2, 3, 4];
// LOWERING-NEXT:     unequal_b = [1, 2, 3, 9];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unequal_a.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unequal_b.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         memcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     partial_a = [1, 2, 3, 4, 9, 9, 9, 9];
// LOWERING-NEXT:     partial_b = [1, 2, 3, 4, 0, 0, 0, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = partial_a.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = partial_b.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         memcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     dyn_a = [1, 2, 3, 4, 5, 6, 7, 8];
// LOWERING-NEXT:     dyn_b = [1, 2, 3, 4, 5, 6, 7, 8];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = get_count();
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = dyn_a.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = dyn_b.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe {
// LOWERING-NEXT:         memcmp(
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as *const core::ffi::c_void,
// LOWERING-NEXT:             {{_v[0-9]+}} as usize,
// LOWERING-NEXT:         )
// LOWERING-NEXT:     };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const core::ffi::c_char, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES-DAG: fn main() {
// REWRITES-DAG:     let mut equal_a: [u8; 4] = [1, 2, 3, 4];
// REWRITES-DAG:     let mut equal_b: [u8; 4] = [1, 2, 3, 4];
// REWRITES-DAG:     let mut unequal_a: [u8; 4] = [1, 2, 3, 4];
// REWRITES-DAG:     let mut unequal_b: [u8; 4] = [1, 2, 3, 9];
// REWRITES-DAG:     let mut partial_a: [u8; 8] = [1, 2, 3, 4, 9, 9, 9, 9];
// REWRITES-DAG:     let mut partial_b: [u8; 8] = [1, 2, 3, 4, 0, 0, 0, 0];
// REWRITES-DAG:     let mut dyn_a: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// REWRITES-DAG:     let mut dyn_b: [u8; 8] = [1, 2, 3, 4, 5, 6, 7, 8];
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut u8 = equal_a.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut u8 = equal_b.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         memcmp(
// REWRITES-DAG:             {{_v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             {{_v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             (4 as u64) as usize,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} == 0) as i32;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut u8 = unequal_a.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut u8 = unequal_b.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         memcmp(
// REWRITES-DAG:             {{_v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             {{_v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             (4 as u64) as usize,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} == 0) as i32;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut u8 = partial_a.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut u8 = partial_b.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         memcmp(
// REWRITES-DAG:             {{_v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             {{_v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             (4 as u64) as usize,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = ({{_v[0-9]+}} == 0) as i32;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = get_count();
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut u8 = dyn_a.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// REWRITES-DAG:     let {{_v[0-9]+}}: *mut u8 = dyn_b.as_mut_ptr() as *mut u8;
// REWRITES-DAG:     let {{_v[0-9]+}}: i32 = unsafe {
// REWRITES-DAG:         memcmp(
// REWRITES-DAG:             {{_v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             {{_v[0-9]+}} as *const core::ffi::c_void,
// REWRITES-DAG:             ({{_v[0-9]+}} as u64) as usize,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     unsafe {
// REWRITES-DAG:         printf(
// REWRITES-DAG:             c"%d %d %d %d\n".as_ptr(),
// REWRITES-DAG:             {{_v[0-9]+}},
// REWRITES-DAG:             {{_v[0-9]+}},
// REWRITES-DAG:             {{_v[0-9]+}},
// REWRITES-DAG:             ({{_v[0-9]+}} == 0) as i32,
// REWRITES-DAG:         )
// REWRITES-DAG:     };
// REWRITES-DAG:     std::process::exit(0 as i32);
// REWRITES-DAG: }
// SLATE-FILECHECK-END rewrites
