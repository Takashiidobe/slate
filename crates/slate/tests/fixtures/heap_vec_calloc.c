#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int *p = calloc(4, sizeof(int));
  printf("%d\n", p[0] + p[3]);
  free(p);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn calloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn free(_0: *mut core::ffi::c_void);
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: u64 = 4;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { calloc({{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 0;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(0) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i64 = 3;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(3) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = {{_v[0-9]+}} + {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = {{_v[0-9]+}} as *mut core::ffi::c_void;
// LOWERING-NEXT:     unsafe { free({{_v[0-9]+}} as *mut core::ffi::c_void) };
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, unconditional_panic, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn calloc(_0: usize, _1: usize) -> *mut core::ffi::c_void;
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn free(_0: *mut core::ffi::c_void);
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: u64 = 4;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = unsafe { calloc({{_v[0-9]+}} as usize, {{_v[0-9]+}} as usize) };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = {{_v[0-9]+}} as *mut i32;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(0) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { *{{_v[0-9]+}} };
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i32 = unsafe { {{_v[0-9]+}}.add(3) };
// REWRITES-NEXT: unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}} + unsafe { *{{_v[0-9]+}} }) };
// REWRITES-NEXT: unsafe { free(({{_v[0-9]+}} as *mut core::ffi::c_void) as *mut core::ffi::c_void) };
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = 0;
// REWRITES-NEXT: std::process::exit({{_v[0-9]+}} as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
