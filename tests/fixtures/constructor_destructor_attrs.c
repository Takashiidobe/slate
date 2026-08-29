#include <stdio.h>

__attribute__((constructor)) static void register_default(void) {
  printf("ctor: default\n");
}

__attribute__((destructor)) static void cleanup_default(void) {
  printf("dtor: default\n");
}

int main(void) {
  printf("main\n");
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
// LOWERING-NEXT: fn register_default() {
// LOWERING-NEXT:     let _v0: *mut i8 = b"ctor: default\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v1: i32 = unsafe { printf(_v0 as *const i8) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn cleanup_default() {
// LOWERING-NEXT:     let _v0: *mut i8 = b"dtor: default\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v1: i32 = unsafe { printf(_v0 as *const i8) };
// LOWERING-NEXT:     return;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     register_default();
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"main\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: i32 = unsafe { printf(_v1 as *const i8) };
// LOWERING-NEXT:     let _v3: i32 = 0;
// LOWERING-NEXT:     __retval = _v3;
// LOWERING-NEXT:     let _v4: i32 = __retval;
// LOWERING-NEXT:     cleanup_default();
// LOWERING-NEXT:     std::process::exit(_v4 as i32);
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
// REWRITES-NEXT: fn register_default() {
// REWRITES-NEXT: let _v0: *mut i8 = b"ctor: default\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v1: i32 = unsafe { printf(_v0 as *const i8) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn cleanup_default() {
// REWRITES-NEXT: let _v0: *mut i8 = b"dtor: default\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v1: i32 = unsafe { printf(_v0 as *const i8) };
// REWRITES-NEXT: return;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: register_default();
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"main\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: i32 = unsafe { printf(_v1 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v4: i32 = __retval;
// REWRITES-NEXT: cleanup_default();
// REWRITES-NEXT: std::process::exit(_v4 as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
