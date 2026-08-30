#include <stdio.h>
#include <string.h>

static int get_count(void) { return 4; }

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
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn memcmp(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> i32;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn get_count() -> i32 {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 4;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut equal_a: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut equal_b: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut equal_result: i32 = 0;
// LOWERING-NEXT:     let mut unequal_a: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut unequal_b: [u8; 4] = [0; 4];
// LOWERING-NEXT:     let mut unequal_result: i32 = 0;
// LOWERING-NEXT:     let mut partial_a: [u8; 8] = [0; 8];
// LOWERING-NEXT:     let mut partial_b: [u8; 8] = [0; 8];
// LOWERING-NEXT:     let mut partial_result: i32 = 0;
// LOWERING-NEXT:     let mut dyn_a: [u8; 8] = [0; 8];
// LOWERING-NEXT:     let mut dyn_b: [u8; 8] = [0; 8];
// LOWERING-NEXT:     let mut n: i32 = 0;
// LOWERING-NEXT:     let mut dyn_result: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     equal_a = [1, 2, 3, 4];
// LOWERING-NEXT:     equal_b = [1, 2, 3, 4];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = equal_a.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = equal_b.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { memcmp({{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     equal_result = {{_v[0-9]+}};
// LOWERING-NEXT:     unequal_a = [1, 2, 3, 4];
// LOWERING-NEXT:     unequal_b = [1, 2, 3, 9];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unequal_a.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = unequal_b.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { memcmp({{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     unequal_result = {{_v[0-9]+}};
// LOWERING-NEXT:     partial_a = [1, 2, 3, 4, 9, 9, 9, 9];
// LOWERING-NEXT:     partial_b = [1, 2, 3, 4, 0, 0, 0, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = partial_a.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = partial_b.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { memcmp({{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     partial_result = {{_v[0-9]+}};
// LOWERING-NEXT:     dyn_a = [1, 2, 3, 4, 5, 6, 7, 8];
// LOWERING-NEXT:     dyn_b = [1, 2, 3, 4, 5, 6, 7, 8];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = get_count();
// LOWERING-NEXT:     n = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = dyn_a.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut u8 = dyn_b.as_mut_ptr() as *mut u8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = n;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { memcmp({{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as *const core::ffi::c_void, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: bool = {{_v[0-9]+}} == {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} as i32;
// LOWERING-NEXT:     dyn_result = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d %d %d %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = equal_result;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unequal_result;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = partial_result;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = dyn_result;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES-DAG: fn memcmp(_0: *const core::ffi::c_void, _1: *const core::ffi::c_void, _2: usize) -> i32;
// REWRITES-NOT: safe fn memcmp(
// REWRITES-LABEL: {{^}}fn main() {
// REWRITES-DAG: unsafe { memcmp(
// REWRITES-DAG: dyn_a.as_mut_ptr()
// REWRITES: {{^}}}
