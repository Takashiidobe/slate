#include <stdio.h>

int main(void) {
  printf("%5s|%-5s|%.1s|%6.1s\n", "hi", "hi", "hi", "hi");
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
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"%5s|%-5s|%.1s|%6.1s\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut i8 = b"hi\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: *mut i8 = b"hi\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v4: *mut i8 = b"hi\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v5: *mut i8 = b"hi\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v6: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4, _v5) };
// LOWERING-NEXT:     let _v7: i32 = 0;
// LOWERING-NEXT:     __retval = _v7;
// LOWERING-NEXT:     let _v8: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v8 as i32);
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
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"%5s|%-5s|%.1s|%6.1s\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut i8 = b"hi\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v3: *mut i8 = b"hi\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v4: *mut i8 = b"hi\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v5: *mut i8 = b"hi\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v6: i32 = unsafe { printf(_v1 as *const i8, _v2, _v3, _v4, _v5) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
