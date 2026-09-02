#include <stdio.h>
#include <stdlib.h>

void touch(int *y) { *y += 1; }

static int use_and_free(int *y) {
  touch(y);
  int v = *y;
  free(y);
  return v;
}

int main(void) {
  int *y = malloc(sizeof(int));
  *y     = 41;
  printf("%d\n", use_and_free(y));
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
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe fn touch({{arg[0-9]+}}: *mut i32) {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 1;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{arg[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn use_and_free({{arg[0-9]+}}: *mut i32) -> i32 {
// LOWERING-NEXT:     unsafe { touch({{arg[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{arg[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     return {{_v[0-9]+}};
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc({{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 41;
// LOWERING-NEXT:     unsafe {
// LOWERING-NEXT:         *{{_v[0-9]+}} = {{_v[0-9]+}};
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = use_and_free({{_v[0-9]+}});
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
// REWRITES-NEXT:     fn malloc(_0: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const core::ffi::c_char, ...) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe fn touch({{arg[0-9]+}}: *mut i32) {
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{arg[0-9]+}} = (unsafe { *{{arg[0-9]+}} }) + 1;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn use_and_free({{arg[0-9]+}}: *mut i32) -> i32 {
// REWRITES-NEXT:     unsafe { touch({{arg[0-9]+}}) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{arg[0-9]+}} };
// REWRITES-NEXT:     unsafe { free({{arg[0-9]+}} as *mut core::ffi::c_void) };
// REWRITES-NEXT:     {{_v[0-9]+}}
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { malloc((4 as u64) as usize) };
// REWRITES-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// REWRITES-NEXT:     unsafe {
// REWRITES-NEXT:         *{{_v[0-9]+}} = 41;
// REWRITES-NEXT:     }
// REWRITES-NEXT:     unsafe { printf(c"%d\n".as_ptr(), use_and_free({{_v[0-9]+}})) };
// REWRITES-NEXT:     std::process::exit(0 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
