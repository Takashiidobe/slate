#include <stdio.h>

static void set_data(void *ptr) {
  char *p = (char *)ptr;
  printf("%s\n", p);
}

int main(void) {
  char text[] = "hello";
  set_data(&text);
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn set_data({{arg[0-9]+}}: *mut core::ffi::c_void) {
// LOWERING-NEXT:     let mut ptr: *mut core::ffi::c_void = std::ptr::null_mut();
// LOWERING-NEXT:     let mut p: *mut i8 = std::ptr::null_mut();
// LOWERING-NEXT:     ptr = {{arg[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = ptr;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = {{_v[0-9]+}} as *mut i8;
// LOWERING-NEXT:     p = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut i8 = p;
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, {{_v[0-9]+}}) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut text: [i8; 6] = [0; 6];
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     text = [104, 101, 108, 108, 111, 0];
// LOWERING-NEXT:     let {{_v[0-9]+}}: *mut core::ffi::c_void = text.as_mut_ptr() as *mut core::ffi::c_void;
// LOWERING-NEXT:     set_data({{_v[0-9]+}} as *mut core::ffi::c_void);
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = 0;
// LOWERING-NEXT:     __retval = {{_v[0-9]+}};
// LOWERING-NEXT:     let {{_v[0-9]+}}: i32 = __retval;
// LOWERING-NEXT:     std::process::exit({{_v[0-9]+}} as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn printf(_0: *const i8, ...) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn set_data({{arg[0-9]+}}: *mut core::ffi::c_void) {
// REWRITES-NEXT: let mut ptr: *mut core::ffi::c_void = {{arg[0-9]+}};
// REWRITES-NEXT: let mut p: *mut i8 = std::ptr::null_mut();
// REWRITES-NEXT: p = ptr as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut i8 = b"%s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let {{_v[0-9]+}}: i32 = unsafe { printf({{_v[0-9]+}} as *const i8, p) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut text: [i8; 6] = [0; 6];
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: text = [104, 101, 108, 108, 111, 0];
// REWRITES-NEXT: let {{_v[0-9]+}}: *mut core::ffi::c_void = text.as_mut_ptr() as *mut core::ffi::c_void;
// REWRITES-NEXT: set_data({{_v[0-9]+}} as *mut core::ffi::c_void);
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
