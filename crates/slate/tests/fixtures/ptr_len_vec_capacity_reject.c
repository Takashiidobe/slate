#include <stdio.h>
#include <stdlib.h>

static int consume_extra_capacity(int *values, int len) {
  int sum = 0;
  for (int i = 0; i < len; ++i) {
    values[i] = i + 1;
    sum += values[i];
  }
  free(values);
  return sum;
}

int main(void) {
  int len = 3;
  int *values = malloc((len + 1) * sizeof(int));
  int sum = consume_extra_capacity(values, len);
  printf("%d\n", sum);
  return 0;
}

// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn consume_extra_capacity({{arg[0-9]+}}: *mut i32, {{arg[0-9]+}}: i32) -> i32 {
// LOWERING-NEXT:     let mut values: *mut i32 = std::ptr::null_mut();
// LOWERING-NEXT:     let mut len: i32 = 0;
// LOWERING-NEXT:     let mut sum: i32 = 0;
// LOWERING-NEXT:     values = {{arg[0-9]+}};
// LOWERING-NEXT:     len = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     sum = {{_v[0-9]+}};
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
// LOWERING-NEXT:             {
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                 unsafe {
// LOWERING-NEXT:                     *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:                 }
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i64 = {{_v[0-9]+}} as i64;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.offset({{_v[0-9]+}} as isize) };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = sum;
// LOWERING-NEXT:                 let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:                 sum = {{_v[0-9]+}};
// LOWERING-NEXT:             }
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = i;
// LOWERING-NEXT:             let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + 1;
// LOWERING-NEXT:             i = {{_v[0-9]+}};
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = values;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = sum;
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} as u64;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = {{_v[0-9]+}} * {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = consume_extra_capacity({{_v[0-9]+}}, {{_v[0-9]+}});
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// REWRITES: fn consume_extra_capacity(arg{{[0-9]+}}: *mut i32, arg{{[0-9]+}}: i32) -> i32
// REWRITES-NOT: Vec::from_raw_parts(
