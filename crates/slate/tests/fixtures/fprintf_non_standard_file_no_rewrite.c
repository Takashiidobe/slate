#include <stdio.h>

int main(void) {
  FILE *f = fopen("slate_fprintf_non_standard.tmp", "w");
  if (f == NULL) {
    return 1;
  }
  fprintf(f, "value: %d\n", 7);
  fclose(f);
  remove("slate_fprintf_non_standard.tmp");
  return 0;
}
// SLATE-FILECHECK-BEGIN lowering
// LOWERING: #![feature(c_variadic)]
// LOWERING-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// LOWERING-EMPTY:
// LOWERING-NEXT: unsafe extern "C" {
// LOWERING-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// LOWERING-NEXT:     fn fprintf(_0: *mut libc::FILE, _1: *const i8, ...) -> i32;
// LOWERING-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// LOWERING-NEXT:     fn remove(_0: *const i8) -> i32;
// LOWERING-NEXT: }
// LOWERING-EMPTY:
// LOWERING-NEXT: fn main() {
// LOWERING-NEXT:     let mut __retval: i32 = 0;
// LOWERING-NEXT:     let mut f: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:     let _v0: i32 = 0;
// LOWERING-NEXT:     __retval = _v0;
// LOWERING-NEXT:     let _v1: *mut i8 = b"slate_fprintf_non_standard.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v2: *mut i8 = b"w\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v3: *mut libc::FILE = unsafe { fopen(_v1 as *const i8, _v2 as *const i8) };
// LOWERING-NEXT:     f = _v3;
// LOWERING-NEXT:     {
// LOWERING-NEXT:         let _v4: *mut libc::FILE = f;
// LOWERING-NEXT:         let _v5: *mut libc::FILE = std::ptr::null_mut();
// LOWERING-NEXT:         let _v6: bool = _v4 == _v5;
// LOWERING-NEXT:         if _v6 {
// LOWERING-NEXT:             let _v7: i32 = 1;
// LOWERING-NEXT:             __retval = _v7;
// LOWERING-NEXT:             let _v8: i32 = __retval;
// LOWERING-NEXT:             std::process::exit(_v8 as i32);
// LOWERING-NEXT:         }
// LOWERING-NEXT:     }
// LOWERING-NEXT:     let _v9: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v10: *mut i8 = b"value: %d\n\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v11: i32 = 7;
// LOWERING-NEXT:     let _v12: i32 = unsafe { fprintf(_v9 as *mut libc::FILE, _v10 as *const i8, _v11) };
// LOWERING-NEXT:     let _v13: *mut libc::FILE = f;
// LOWERING-NEXT:     let _v14: i32 = unsafe { fclose(_v13 as *mut libc::FILE) };
// LOWERING-NEXT:     let _v15: *mut i8 = b"slate_fprintf_non_standard.tmp\0".as_ptr() as *mut i8;
// LOWERING-NEXT:     let _v16: i32 = unsafe { remove(_v15 as *const i8) };
// LOWERING-NEXT:     let _v17: i32 = 0;
// LOWERING-NEXT:     __retval = _v17;
// LOWERING-NEXT:     let _v18: i32 = __retval;
// LOWERING-NEXT:     std::process::exit(_v18 as i32);
// LOWERING-NEXT: }
// SLATE-FILECHECK-END lowering

// SLATE-FILECHECK-BEGIN rewrites
// REWRITES: #![feature(c_variadic)]
// REWRITES-NEXT: #![allow(dead_code, unused, non_camel_case_types, non_snake_case, non_upper_case_globals, arithmetic_overflow, suspicious_runtime_symbol_definitions, unpredictable_function_pointer_comparisons, unused_comparisons)]
// REWRITES-EMPTY:
// REWRITES-NEXT: unsafe extern "C" {
// REWRITES-NEXT:     fn fopen(_0: *const i8, _1: *const i8) -> *mut libc::FILE;
// REWRITES-NEXT:     fn fprintf(_0: *mut libc::FILE, _1: *const i8, ...) -> i32;
// REWRITES-NEXT:     fn fclose(_0: *mut libc::FILE) -> i32;
// REWRITES-NEXT:     fn remove(_0: *const i8) -> i32;
// REWRITES-NEXT: }
// REWRITES-EMPTY:
// REWRITES-NEXT: fn main() {
// REWRITES-NEXT: let mut __retval: i32 = 0;
// REWRITES-NEXT: let mut f: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: let _v1: *mut i8 = b"slate_fprintf_non_standard.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v2: *mut i8 = b"w\0".as_ptr() as *mut i8;
// REWRITES-NEXT: f = unsafe { fopen(_v1 as *const i8, _v2 as *const i8) };
// REWRITES-NEXT: {
// REWRITES-NEXT:         let _v5: *mut libc::FILE = std::ptr::null_mut();
// REWRITES-NEXT:         let _v6: bool = f == _v5;
// REWRITES-NEXT:         if _v6 {
// REWRITES-NEXT:                     __retval = 1;
// REWRITES-NEXT:                     std::process::exit(__retval as i32);
// REWRITES-NEXT:         }
// REWRITES-NEXT: }
// REWRITES-NEXT: let _v9: *mut libc::FILE = f;
// REWRITES-NEXT: let _v10: *mut i8 = b"value: %d\n\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v11: i32 = 7;
// REWRITES-NEXT: let _v12: i32 = unsafe { fprintf(_v9 as *mut libc::FILE, _v10 as *const i8, _v11) };
// REWRITES-NEXT: let _v14: i32 = unsafe { fclose(f as *mut libc::FILE) };
// REWRITES-NEXT: let _v15: *mut i8 = b"slate_fprintf_non_standard.tmp\0".as_ptr() as *mut i8;
// REWRITES-NEXT: let _v16: i32 = unsafe { remove(_v15 as *const i8) };
// REWRITES-NEXT: __retval = 0;
// REWRITES-NEXT: std::process::exit(__retval as i32);
// REWRITES-NEXT: }
// SLATE-FILECHECK-END rewrites
